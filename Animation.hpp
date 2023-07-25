#pragma once

struct Animation
{
	int row;
	int nFrames;
	float speed;

	Animation()
	{
		row = 0;
		nFrames = 1;
		speed = 0;
	}

	Animation(int r, int f, float s)
	{
		row = r;
		nFrames = f;
		speed = s;
	}
};