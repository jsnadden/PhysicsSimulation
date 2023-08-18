#pragma once
#include <iostream>
#include <math.h>

#define PI 3.14159265

struct Vector2D
{
	float x;
	float y;

	// Default constructor
	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}
	// Full constructor
	Vector2D(float xIn, float yIn)
	{
		x = xIn;
		y = yIn;
	}
	// Copy constructor
	Vector2D(const Vector2D& v)
	{
		x = v.x;
		y = v.y;
	}

	float Dot(const Vector2D& v) const
	{
		return this->x * v.x + this->y * v.y;
	}

	float Cross(const Vector2D& v) const
	{
		return this->x * v.y - this->y * v.x;
	}

	Vector2D operator+(const Vector2D& v) const
	{
		return Vector2D(x + v.x, y + v.y);
	}

	Vector2D operator-(const Vector2D& v) const
	{
		return Vector2D(x - v.x, y - v.y);
	}

	Vector2D operator*(const Vector2D& v) const
	{
		return Vector2D(x * v.x, y * v.y);
	}

	Vector2D& operator+=(const Vector2D& v)
	{
		this->x += v.x;
		this->y += v.y;
		return *this;
	}

	Vector2D& operator-=(const Vector2D& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}

	Vector2D& operator*=(const Vector2D& v)
	{
		this->x *= v.x;
		this->y *= v.y;
		return *this;
	}

	Vector2D& operator*=(const float& s)
	{
		this->x *= s;
		this->y *= s;
		return *this;
	}

	Vector2D& operator/=(const float& s)
	{
		this->x /= s;
		this->y /= s;
		return *this;
	}

	void Zero()
	{
		x = 0.0f;
		y = 0.0f;
	}

	bool IsZero() const
	{
		return x == 0.0f && y == 0.0f;
	}

	float NormSquared()
	{
		return x * x + y * y;
	}

	float Norm()
	{
		return static_cast<float>(sqrt(x * x + y * y));
	}

	void Normalise()
	{
		float n = Norm();
		if (n != 0.f)
		{
			x = x / n;
			y = y / n;
		}
	}

	Vector2D Normalised()
	{
		float n = Norm();
		return Vector2D(x / n, y / n);
	}

	Vector2D Orth() const
	{
		return Vector2D(-this->y, this->x);
	}

	static Vector2D RotateVector(Vector2D& v, float theta)
	{
		return Vector2D(static_cast<float>(v.x * cos(theta) - v.y * sin(theta)), static_cast<float>(v.x * sin(theta) + v.y * cos(theta)));
	}

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
	{
		stream << "(" << vec.x << "," << vec.y << ")";
		return stream;
	}

	bool operator==(const Vector2D& v) const
	{
		return (x == v.x && y == v.y);
	}

	Vector2D  operator+() const
	{
		return { +x, +y };
	}
	
	Vector2D  operator-() const
	{
		return { -x, -y };
	}

};



inline Vector2D operator*(const float& s, const Vector2D& v)
{
	return Vector2D(s * v.x, s * v.y);
}

inline Vector2D operator*(const Vector2D& v, const float& s)
{
	return Vector2D(s * v.x, s * v.y);
}

inline Vector2D operator/(const Vector2D& v, const float& s)
{
	return Vector2D(v.x / s, v.y / s);
}

const Vector2D VEC_RIGHT = Vector2D(1.0f, 0.0f);
const Vector2D VEC_UP = Vector2D(0.0f, -1.0f);
const Vector2D VEC_LEFT = Vector2D(-1.0f, 0.0f);
const Vector2D VEC_DOWN = Vector2D(0.0f, 1.0f);
const Vector2D VEC_ZERO = Vector2D(0.0f, 0.0f);