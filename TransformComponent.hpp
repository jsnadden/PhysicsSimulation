#pragma once
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Timer.hpp"
#include "Disk.h"
#include "Rect.hpp"

class PolyTransformComponent : public Component
{
private:

	Graphics* graphics;
	Timer* timer;
	const float GRAV_ACC = 1.0f;

	SDL_Color colour;

public:

	Vector2D velocity;
	Polygon polygon;
	float theta;
	float omega;
	float density;
	float mass;

	PolyTransformComponent() = default;

	PolyTransformComponent(Polygon p, float d)
	{
		polygon = p;
		omega = theta = 0.0f;
		density = d;
		mass = d * polygon.Area();

		Uint8 r = rand() % 255;
		Uint8 g = rand() % 255;
		Uint8 b = rand() % 255;
		colour = { r,g,b, 0xff };
	}

	~PolyTransformComponent()
	{
		timer = nullptr;
		graphics = nullptr;
	}

	void init() override
	{
		velocity.Zero();
		timer = Timer::GetInstance();
		graphics = Graphics::GetInstance();
	}

	void EarlyUpdate() override
	{
		polygon.centre.x += 0.5f * velocity.x * timer->DeltaTime();
		polygon.centre.y += 0.5f * velocity.y * timer->DeltaTime();
	}

	void Update() override
	{
		polygon.centre.x += 0.5f * velocity.x * timer->DeltaTime();
		polygon.centre.y += 0.5f * velocity.y * timer->DeltaTime();
	}

	void draw() override
	{
		int n = polygon.Size();

		for (int i = 0; i < n; i++)
		{
			graphics->DrawLine(colour, polygon.centre, polygon.centre + polygon.vertices[i]); 
			graphics->DrawLine(colour, polygon.centre + polygon.vertices[i], polygon.centre + polygon.vertices[i+1]);
		}

	}

	void ApplyForce(Vector2D F)
	{
		velocity += (1 / mass) * F;
	}

	void Translate(Vector2D disp)
	{
		polygon.centre += disp;
	}

	float Energy()
	{
		return 0.5f * mass * velocity.NormSquared();
	}

	float Mass()
	{
		return mass;
	}

};

class DiskTransformComponent : public Component
{
private:

	Timer* timer;

	Vector2D velocity;
	float theta;
	float omega;
	float density;
	float mass;

	const float GRAV_ACC = 1.0f;

public:

	Disk disk;

	DiskTransformComponent()
	{}

	DiskTransformComponent(float x, float y, float r, float d)
	{
		disk.centre.x = x;
		disk.centre.y = y;
		disk.radius = r;
		omega = theta = 0.0f;
		density = d;
		mass = d * disk.Area();
	}

	~DiskTransformComponent()
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
		velocity.y += GRAV_ACC * mass * timer->DeltaTime();

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

class RectTransformComponent : public Component
{
private:

	Timer* timer;

	Vector2D velocity;
	float theta;
	float omega;
	float density;
	float mass;

	const float GRAV_ACC = 1.0f;

public:

	Rect rect;
	Vector2D centre;

	RectTransformComponent()
	{}

	RectTransformComponent(float x, float y, float w, float h, float d)
	{
		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;
		omega = theta = 0.0f;
		density = d;
		mass = d * rect.Area();
	}

	~RectTransformComponent()
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
		rect.x += 0.5f * velocity.x * timer->DeltaTime();
		rect.y += 0.5f * velocity.y * timer->DeltaTime();
	}

	void Update() override
	{
		// Gravity
		velocity.y += GRAV_ACC * mass * timer->DeltaTime();

		rect.x += 0.5f * velocity.x * timer->DeltaTime();
		rect.y += 0.5f * velocity.y * timer->DeltaTime();
		rect.vx = velocity.x;
		rect.vy = velocity.y;

		centre = rect.Centre();

		// Reflect off the screen boundaries
		if (rect.x < 0)
		{
			rect.x = 0;
			velocity.x = -velocity.x;
		}

		if (rect.y < 0)
		{
			rect.y = 0;
			velocity.y = -velocity.y;
		}

		if (rect.x + rect.w > 800)
		{
			rect.x = 800.0f - rect.w;
			velocity.x = -velocity.x;
		}

		if (rect.y + rect.h > 640)
		{
			rect.y = 640.0f - rect.h;
			velocity.y = -velocity.y;
		}

	}

	void ApplyForce(Vector2D F)
	{
		velocity += (1 / mass) * F;
	}

	float Width()
	{
		return rect.w;
	}

	float Height()
	{
		return rect.h;
	}

	void SetPosition(Vector2D pos)
	{
		rect.x = pos.x - rect.w / 2;
		rect.y = pos.y - rect.h / 2;
	}

	void Translate(Vector2D disp)
	{
		rect.x += disp.x;
		rect.y += disp.y;
	}

	Vector2D* Centre()
	{
		return &centre;
	}

	Vector2D* GetVelocity()
	{
		return &velocity;
	}

	void SetVelocity(Vector2D vel)
	{
		velocity.x = rect.vx = vel.x;
		velocity.y = rect.vy = vel.y;
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
		return (0.5f * mass * velocity.NormSquared()) + GRAV_ACC * mass * (640 - rect.Centre().y);
	}

	float Mass()
	{
		return mass;
	}

};