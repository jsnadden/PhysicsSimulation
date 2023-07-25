#pragma once
#include <SDL.h>

class Timer
{
public:

	static Timer* GetInstance();
	static void Release();

	void Reset();
	float DeltaTime();

	void SetTimeScale(float t);
	float TimeScale();

	void Update();

private:

	static Timer* instance;

	unsigned int startTicks;
	unsigned int elapsedTicks;
	float deltaTime;
	float timeScale;

	Timer();
	~Timer();

};

