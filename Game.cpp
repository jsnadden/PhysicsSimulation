#include "Game.hpp"

Game* Game::instance = nullptr;
ECSManager manager;

auto& polys(manager.getGroup(Game::polyGroup));
auto& disks(manager.getGroup(Game::diskGroup));
auto& rects(manager.getGroup(Game::rectGroup));


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

    if (input->MouseButtonPressed(Input::left) || (input->KeyDown(SDL_SCANCODE_LSHIFT) && input->MouseButtonDown(Input::left)))
    {
        int n = rand() % 10 + 3;

        auto& poly(manager.addEntity());
        Polygon p(Vector2D(input->MousePosition().x, input->MousePosition().y), n, 30);
        poly.addComponent<PolyTransformComponent>(p, 1.0f);
        poly.addGroup(polyGroup);
    }

    if (input->MouseButtonPressed(Input::right))
    {
        auto& rect(manager.addEntity());
        rect.addComponent<RectTransformComponent>(input->MousePosition().x - 20.0f, input->MousePosition().y - 20.0f, 40.0f, 40.f, 1.0f);
        rect.addComponent<RectSpriteComponent>();
        rect.getComponent<RectTransformComponent>().SetVelocity(Vector2D());
        rect.addGroup(rectGroup);
    }

    manager.refresh();
    manager.EarlyUpdate();
}

void Game::Update()
{
    // Antigravity??
    if (input->KeyDown(SDL_SCANCODE_SPACE))
    {
        for (auto& c : disks)
        {
            c->getComponent<DiskTransformComponent>().ApplyForce(Vector2D(0.0f, -20.0f));
        }

        for (auto& r : rects)
        {
            r->getComponent<RectTransformComponent>().ApplyForce(Vector2D(0.0f, -20.0f));
        }
    }
    
    HandleCollision();

    manager.Update();

}

void Game::LateUpdate()
{
    manager.LateUpdate();
    
    input->UpdatePrevious();

    float energy = 0;


    // Compute total energy
    /*for (auto& c : colliders)
    {
        energy += (c->getComponent<TransformComponent>().Energy()) / 1000000.0f;
    }*/
    // std::cout << "Current total energy: " << energy << std::endl;

}

void Game::Render()
{
    graphics->ClearRenderer();

    // DRAW CALLS GO HERE

    for (auto& c : disks)
    {
        c->draw();
    }

    for (auto& r : rects)
    {
        r->draw();
    }

    for (auto& p : polys)
    {
        p->draw();
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
        //std::cout << timer->DeltaTime() << std::endl;
    }
}



void Game::HandleCollision()
{
    HandlePolyCollision();
    HandleDiskCollision();
    HandleRectCollision();
}

void Game::HandlePolyCollision()
{
    for (auto first = polys.begin(); first != polys.end(); ++first)
    {
        for (auto second = first + 1; second != polys.end(); ++second)
        {
            Collision::ResolveSAT_Static((*first)->getComponent<PolyTransformComponent>().polygon,
                (*second)->getComponent<PolyTransformComponent>().polygon);
        }
    }
}

void Game::HandleDiskCollision()
{
    for (auto first = disks.begin(); first != disks.end(); ++first) {
        for (auto second = first + 1; second != disks.end(); ++second) {

            if (Collision::Disks((*first)->getComponent<DiskTransformComponent>().disk, (*second)->getComponent<DiskTransformComponent>().disk))
            {
                //std::cout << "Collision" << std::endl;

                // Get collision parameters
                Vector2D q0 = (*first)->getComponent<DiskTransformComponent>().disk.centre;
                Vector2D q1 = (*second)->getComponent<DiskTransformComponent>().disk.centre;
                Vector2D u0 = *(*first)->getComponent<DiskTransformComponent>().GetVelocity();
                Vector2D u1 = *(*second)->getComponent<DiskTransformComponent>().GetVelocity();
                float m0 = (*first)->getComponent<DiskTransformComponent>().Mass();
                float m1 = (*second)->getComponent<DiskTransformComponent>().Mass();
                float invM = 1.0f / (m0 + m1);
                float r0 = (*first)->getComponent<DiskTransformComponent>().Radius();
                float r1 = (*second)->getComponent<DiskTransformComponent>().Radius();

                // Set up normal+tangent basis at approximate contact point
                Vector2D normal = q1 - q0;
                float distance = normal.Norm();
                normal.Normalise();
                Vector2D tangent = normal.Orth();

                // Project velocity vectors onto our basis
                float n0 = u0.Dot(normal);
                float t0 = u0.Dot(tangent);
                float n1 = u1.Dot(normal);
                float t1 = u1.Dot(tangent);

                // Solve 1D elastic collision in normal direction
                float v0 = ((m0 - m1) * invM) * n0 + (2.0f * m1 * invM) * n1;
                float v1 = (2.0f * m0 * invM) * n0 + ((m1 - m0) * invM) * n1;
                (*first)->getComponent<DiskTransformComponent>().SetVelocity(v0 * normal + t0 * tangent);
                (*second)->getComponent<DiskTransformComponent>().SetVelocity(v1 * normal + t1 * tangent);

                // Separate colliders
                (*first)->getComponent<DiskTransformComponent>().Translate(-0.5f * (r0 + r1 - distance) * normal);
                (*second)->getComponent<DiskTransformComponent>().Translate(0.5f * (r0 + r1 - distance) * normal);
            }

            // Attractive force
            /*Vector2D sep = (*second)->getComponent<TransformComponent>().disk.centre - (*first)->getComponent<TransformComponent>().disk.centre;
            Vector2D force = 100.0f * sep;
            (*first)->getComponent<TransformComponent>().ApplyForce(force);
            (*second)->getComponent<TransformComponent>().ApplyForce(-1.0f * force);*/


        }
    }
}
void Game::HandleRectCollision()
{
    for (auto first = rects.begin(); first != rects.end(); ++first) {
        for (auto second = first + 1; second != rects.end(); ++second) {

            Vector2D contactPos, normal;
            float contactTime = 0.0f;

            Rect firstRect = (*first)->getComponent<RectTransformComponent>().rect;
            Rect secondRect = (*second)->getComponent<RectTransformComponent>().rect;

            if (Collision::SweptAABB(firstRect, secondRect, timer->DeltaTime(), contactPos, normal, contactTime))
            {
                // HANDLE COLLISION
                // Get collision parameters
                Vector2D q0 = firstRect.Centre();
                Vector2D q1 = secondRect.Centre();
                Vector2D u0 = firstRect.GetVelocity();
                Vector2D u1 = secondRect.GetVelocity();
                float m0 = (*first)->getComponent<RectTransformComponent>().Mass();
                float m1 = (*second)->getComponent<RectTransformComponent>().Mass();
                float invM = 1.0f / (m0 + m1);
                float size0 = std::abs(firstRect.GetSize().Dot(normal));
                float size1 = std::abs(secondRect.GetSize().Dot(normal));

                Vector2D tangent = normal.Orth();

                // Project velocity vectors onto our basis
                float n0 = u0.Dot(normal);
                float t0 = u0.Dot(tangent);
                float n1 = u1.Dot(normal);
                float t1 = u1.Dot(tangent);

                // Solve 1D elastic collision in normal direction
                float v0 = ((m0 - m1) * invM) * n0 + (2.0f * m1 * invM) * n1;
                float v1 = (2.0f * m0 * invM) * n0 + ((m1 - m0) * invM) * n1;
                (*first)->getComponent<RectTransformComponent>().SetVelocity(v0 * normal + t0 * tangent);
                (*second)->getComponent<RectTransformComponent>().SetVelocity(v1 * normal + t1 * tangent);

                // Separate colliders
                /*(*first)->getComponent<DiskTransformComponent>().Translate(-0.5f * (r0 + r1 - distance) * normal);
                (*second)->getComponent<DiskTransformComponent>().Translate(0.5f * (r0 + r1 - distance) * normal);*/

            }
        }
    }
}
