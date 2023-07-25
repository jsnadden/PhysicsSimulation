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

    Vector2D v0, v1;

    for (auto first = colliders.begin(); first != colliders.end(); ++first) {
        for (auto second = first + 1; second != colliders.end(); ++second) {
            
            if (Collision::Disks((*first)->getComponent<TransformComponent>().disk, (*second)->getComponent<TransformComponent>().disk))
            {
                //std::cout << "Collision detected!" << std::endl;
                v0 = (*first)->getComponent<TransformComponent>().disk.centre;
                v1 = (*second)->getComponent<TransformComponent>().disk.centre;
                (*first)->getComponent<TransformComponent>().ApplyForce(10.0f*(v0-v1));
                (*second)->getComponent<TransformComponent>().ApplyForce(10.0f * (v1 - v0));
            }

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
        energy += (c->getComponent<TransformComponent>().Energy()) / 1000.0f;
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
