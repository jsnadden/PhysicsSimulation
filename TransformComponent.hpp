#pragma once
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Timer.hpp"
#include "Disk.h"

class TransformComponent : public Component
{
private:

	Timer* timer;

	Vector2D velocity;
	float theta;
	float omega;
	float density;
	float mass;

	const float GRAV_ACC = 1000.0f;

public:

	Disk disk;

	TransformComponent()
	{}

	TransformComponent(float x, float y, float r, float d)
	{
		disk.centre.x = x;
		disk.centre.y = y;
		disk.radius = r;
		omega = theta = 0.0f;
		density = d;
		mass = d * disk.Area();
	}

	~TransformComponent()
	{
		timer = nullptr;
	}

	void init() override
	{
		velocity.Zero();
		timer = Timer::GetInstance();
	}

	void EarlyUpdate() override
	{
		disk.centre.x += 0.5f * velocity.x * timer->DeltaTime();
		disk.centre.y += 0.5f * velocity.y * timer->DeltaTime();
	}

	void Update() override
	{
		// Gravity
		velocity.y += GRAV_ACC * timer->DeltaTime();

		disk.centre.x += 0.5f * velocity.x * timer->DeltaTime();
		disk.centre.y += 0.5f * velocity.y * timer->DeltaTime();

		// Reflect off the screen boundaries
		if (disk.centre.x - disk.radius < 0)
		{
			disk.centre.x = disk.radius;
			velocity.x = -velocity.x;
		}
			
		if (disk.centre.y - disk.radius < 0)
		{
			disk.centre.y = disk.radius;
			velocity.y = -velocity.y;
		}

		if (disk.centre.x + disk.radius > 800)
		{
			disk.centre.x = 800.0f - disk.radius;
			velocity.x = -velocity.x;
		}
			
		if (disk.centre.y + disk.radius > 640)
		{
			disk.centre.y = 640.0f - disk.radius;
			velocity.y = -velocity.y;
		}
			
	}

	void ApplyForce(Vector2D F)
	{
		velocity += (1 / mass) * F;
	}

	float Radius()
	{
		return disk.radius;
	}

	void SetPosition(Vector2D pos)
	{
		disk.centre = pos;
	}

	void Translate(Vector2D disp)
	{
		disk.centre += disp;
	}

	Vector2D* Centre()
	{
		return &disk.centre;
	}

	Vector2D* GetVelocity()
	{
		return &velocity;
	}

	void SetVelocity(Vector2D vel)
	{
		velocity.x = vel.x;
		velocity.y = vel.y;
	}

	void SetRotation(float rot)
	{
		theta = rot;
	}

	void Rotate(float rot)
	{
		theta += rot;
	}

	float GetRotation()
	{
		return theta;
	}

	float Energy()
	{
		return (0.5f * mass * velocity.NormSquared()) + GRAV_ACC * mass * (640 - disk.centre.y);
	}

	float Mass()
	{
		return mass;
	}

};