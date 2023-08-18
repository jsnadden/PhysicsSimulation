#pragma once
#include <vector>
#include <iostream>
#include <initializer_list>
#include "Vector2D.hpp"

struct Polygon
{
	Vector2D centre;
	std::vector<Vector2D> vertices;

	Polygon()
	{
		vertices.emplace_back(VEC_ZERO);
	}

	Polygon(Vector2D cent, std::initializer_list<Vector2D> pts)
	{
		centre = cent;
		for (auto p : pts)
		{
			vertices.emplace_back(p);
		}

		vertices.emplace_back(vertices[0]);
	}

	Polygon(Vector2D cent, int n, float r)
	{
		centre = cent;

		Vector2D p(r, 0);

		for (int i = 0; i < n; i++)
		{
			vertices.emplace_back(Vector2D::RotateVector(p, i * 2 * PI / n));
		}

		vertices.emplace_back(vertices[0]);
	}

	float Area()
	{
		// need to implement shoelace formula
		return 1;
	}

	int Size()
	{
		return vertices.size() - 1;
	}
};