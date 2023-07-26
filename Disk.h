#pragma once
#include "Vector2D.hpp"

struct Disk
{
	float radius;
	Vector2D centre;

	Disk()
	{
		radius = 1.0f;
		centre.x = 0.0f;
		centre.y = 0.0f;
	}

	Disk(float r, float x, float y)
	{
		radius = r;
		centre.x = x;
		centre.y = y;
	}

	float Area()
	{
		return PI * radius * radius;
	}
};