#pragma once
#include "Graphics.hpp"
#include "Assets.hpp"
#include "Input.hpp"
#include "Timer.hpp"
#include "Audio.hpp"
#include "ECS.hpp"
#include "Components.hpp"
#include "Vector2D.hpp"
#include "Collision.hpp"

class Game
{
public:

	static Game* GetInstance();
	static void Release();

	void Run();

	enum groupLabels : std::size_t
	{
		polyGroup,
		diskGroup,
		rectGroup
	};

private:

	const int FRAME_RATE = 120;
	const float FRAME_SECS = 1.0f / FRAME_RATE;

	static Game* instance;

	bool quit;

	Graphics* graphics;
	Assets* assets;
	Input* input;
	Audio* audio;
	Collision* collision;

	SDL_Rect viewRect;

	Timer* timer;

	SDL_Event event;

	void EarlyUpdate();
	void Update();
	void LateUpdate();
	void Render();

	Game();
	~Game();

	void HandleCollision();
	void HandlePolyCollision();
	void HandleDiskCollision();
	void HandleRectCollision();

};

