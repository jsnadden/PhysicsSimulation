#pragma once
#include "Components.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "Vector2D.hpp"

class ControlComponent : public Component
{
private:

    Timer* timer;
    Input* input;

    TransformComponent* transform;
    SpriteComponent* sprite;

public:

    ControlComponent()
    {
        timer = Timer::GetInstance();
        input = Input::GetInstance();
    }

    ~ControlComponent()
    {
        timer = nullptr;
        input = nullptr;
        transform = nullptr;
        sprite = nullptr;
    }

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void Update() override
	{
        

       
	}

};