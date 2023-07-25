#pragma once
#include <SDL.h>
#include "Disk.h"

class Collision
{
private:

	Collision();
	~Collision();

	static Collision* instance;

public:

	static Collision* GetInstance();
	static void Release();

	static bool AABB(const SDL_Rect rectA, const SDL_Rect rectB);

	static bool Disks(Disk dA, Disk dB);

};