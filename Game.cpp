#include "Game.hpp"

Game* Game::instance = nullptr;
ECSManager manager;

auto& colliders(manager.getGroup(Game::colliderGroup));


Game::Game()
{
    quit = false;

    graphics = Graphics::GetInstance();
    if (!Graphics::HasInitialised())
    {
        quit = true;
    }

    assets = Assets::GetInstance();
    input = Input::GetInstance();
    audio = Audio::GetInstance();
    timer = Timer::GetInstance();
    collision = Collision::GetInstance();

}
Game::~Game()
{
    Assets::Release();
    assets = nullptr;

    Graphics::Release();
    graphics = nullptr;

    Audio::Release();
    audio = nullptr;

    Timer::Release();
    timer = nullptr;

    Collision::Release();
    collision = nullptr;
}

Game* Game::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Game();
    }

    return instance;
}

void Game::Release()
{
    delete instance;
    instance = nullptr;
}

void Game::EarlyUpdate()
{
    input->Update();

    if (input->MouseButtonPressed(Input::left))
    {
        auto& disk(manager.addEntity());
        disk.addComponent<TransformComponent>(input->MousePosition().x, input->MousePosition().y, 20.0f, 1.0f);
        disk.addComponent<SpriteComponent>();
        disk.getComponent<TransformComponent>().SetVelocity(Vector2D());
        disk.addGroup(colliderGroup);
    }

    manager.refresh();
    manager.EarlyUpdate();
}

void Game::Update()
{
    // Antigravity??
    if (input->KeyDown(SDL_SCANCODE_SPACE))
    {
        for (auto& c : colliders)
        {
            c->getComponent<TransformComponent>().ApplyForce(Vector2D(0.0f, -20.0f));
        }
    }
    
    // Collision handling

     for (auto first = colliders.begin(); first != colliders.end(); ++first) {
        for (auto second = first + 1; second != colliders.end(); ++second) {
            
            if (Collision::Disks((*first)->getComponent<TransformComponent>().disk, (*second)->getComponent<TransformComponent>().disk))
            {
                std::cout << "Collision" << std::endl;

                // Get collision parameters
                Vector2D q0 = (*first)->getComponent<TransformComponent>().disk.centre;
                Vector2D q1 = (*second)->getComponent<TransformComponent>().disk.centre;
                Vector2D u0 = *(*first)->getComponent<TransformComponent>().GetVelocity();
                Vector2D u1 = *(*second)->getComponent<TransformComponent>().GetVelocity();
                float m0 = (*first)->getComponent<TransformComponent>().Mass();
                float m1 = (*second)->getComponent<TransformComponent>().Mass();
                float M = m0 + m1;
                float r0 = (*first)->getComponent<TransformComponent>().Radius();
                float r1 = (*second)->getComponent<TransformComponent>().Radius();

                // Set up normal+tangent basis at approximate contact point
                Vector2D normal = q1 - q0;
                float distance = normal.Norm();
                normal.Normalise();
                Vector2D tangent = normal.Orth();

                // Project velocity vectors onto our basis
                float n0 = u0.Dot(normal);
                float t0 = u0.Dot(tangent);;
                float n1 = u1.Dot(normal);;
                float t1 = u1.Dot(tangent);;

                // Solve 1D elastic collision in normal direction
                float v0 = ((m0 - m1) / M) * n0 + (2.0f * m1 / M) * n1;
                float v1 = (2.0f * m0 / M) * n0 + ((m1 - m0) / M) * n1;
                (*first)->getComponent<TransformComponent>().SetVelocity( v0 * normal + t0 * tangent);
                (*second)->getComponent<TransformComponent>().SetVelocity(v1 * normal + t1 * tangent);

                // Separate colliders
                (*first)->getComponent<TransformComponent>().Translate( -0.5f * (r0 + r1 - distance) * normal);
                (*second)->getComponent<TransformComponent>().Translate(0.5f * (r0 + r1 - distance) * normal);
            }

            // Attractive force
            /*Vector2D sep = (*second)->getComponent<TransformComponent>().disk.centre - (*first)->getComponent<TransformComponent>().disk.centre;
            Vector2D force = 100.0f * sep;
            (*first)->getComponent<TransformComponent>().ApplyForce(force);
            (*second)->getComponent<TransformComponent>().ApplyForce(-1.0f * force);*/


        }
    }

    manager.Update();

}

void Game::LateUpdate()
{
    manager.LateUpdate();
    
    input->UpdatePrevious();

    float energy = 0;

    for (auto& c : colliders)
    {
        energy += (c->getComponent<TransformComponent>().Energy()) / 1000000.0f;
    }

    std::cout << "Current total energy: " << energy << std::endl;

}

void Game::Render()
{
    graphics->ClearRenderer();

    // DRAW CALLS GO HERE

    for (auto& c : colliders)
    {
        c->draw();
    }

    graphics->Render();
}


void Game::Run()
{
    while (!quit)
    {
        timer->Reset();

        while (SDL_PollEvent(&event) != 0)
        {
            if ( event.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // TODO remove this in favour of a menu
        if (input->KeyPressed(SDL_SCANCODE_ESCAPE))
        {
            quit = true;
        }

        EarlyUpdate();

        Update();
        
        LateUpdate();

        Render();

        // CRUDE FRAME RATE LIMITER
        timer->Update();
        if (timer->DeltaTime() < FRAME_SECS)
        {
            SDL_Delay(1000.f * (FRAME_SECS - timer->DeltaTime()));
        }

        timer->Update();
        
    }
}
