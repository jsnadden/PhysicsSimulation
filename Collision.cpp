#include "Collision.hpp"

Collision* Collision::instance = nullptr;

Collision::Collision()
{

}

Collision::~Collision()
{

}

Collision* Collision::GetInstance()
{
	if (instance = nullptr)
	{
		instance = new Collision();
	}

	return instance;
}

void Collision::Release()
{
	delete instance;
	instance = nullptr;
}

bool Collision::AABB(const SDL_Rect rectA, const SDL_Rect rectB)
{
	return (rectA.x + rectA.w >= rectB.x)
		&& (rectB.x + rectB.w >= rectA.x)
		&& (rectA.y + rectA.h >= rectB.y)
		&& (rectB.y + rectB.h >= rectA.y);
		
} 

bool Collision::Disks(Disk dA, Disk dB)
{
	Vector2D separation = dA.centre - dB.centre;
	return (separation.Norm() <= dA.radius + dB.radius);
}

