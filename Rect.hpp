#pragma once
#include <SDL.h>
#include "Vector2D.hpp"

// Dynamic floating-point rectangles
struct Rect
{
	float x;
	float y;
	float w;
	float h;
	float vx;
	float vy;

	Rect(float xpos = 0.0f, float ypos = 0.0f, float width = 1.0f, float height = 1.0f, float velx = 0.0f, float vely = 0.0f)
	{
		x = xpos;
		y = ypos;
		w = width;
		h = height;
		vx = velx;
		vy = vely;
	}

	Rect(SDL_Rect R)
	{
		x = R.x;
		y = R.y;
		w = R.w;
		h = R.h;
		vx = 0.0f;
		vy = 0.0f;
	}

	SDL_Rect SDLCast()
	{
		SDL_Rect rect{ static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
		return rect;
	}

	Vector2D GetPosition() const
	{
		return Vector2D(x, y);
	}

	Vector2D Centre() const
	{
		return Vector2D(x + w / 2, y + h / 2);
	}

	Vector2D GetSize() const
	{
		return Vector2D(w, h);
	}

	Vector2D GetVelocity() const
	{
		return Vector2D(vx, vy);
	}

	void Scale(float s)
	{
		w *= s;
		h *= s;
	}

	float Area()
	{
		return w * h;
	}

	Rect operator+(const Rect& R) const
	{
		return Rect(this->x - R.w / 2, this->y - R.h / 2, this->w + R.w, this->h + R.h, this->vx, this->vy);
	}
};