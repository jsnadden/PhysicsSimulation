#pragma once
#include <SDL.h>
#include "Disk.h"
#include "Rect.hpp"
#include "Polygon.hpp"

class Collision
{
private:

	Collision();
	~Collision();

	static Collision* instance;

public:

	static Collision* GetInstance();
	static void Release();

	static bool AABB(const Rect rectA, const Rect rectB);

	static bool Disks(Disk dA, Disk dB);

	// Check collision for a ray and an axis-aligned rectangle, passback collision data
	static bool RayRect(const Vector2D& rayOrigin, const Vector2D& rayDirection,
		const Rect& target, Vector2D& contactPoint, Vector2D& contactNormal, float& contactTime);

	// Check collision for two axis-aligned rectangular ColliderComponents, passback collision data
	static bool SweptAABB(const Rect& colliderA, const Rect& colliderB, float dt,
		Vector2D& contactPos, Vector2D& contactNormal, float& contactTime);

	static bool ResolveSAT_Static(Polygon& p1, Polygon& p2);

};