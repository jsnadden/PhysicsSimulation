#include "Timer.hpp"

Timer* Timer::instance = nullptr;

Timer* Timer::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Timer();
	}

	return instance;
}

void Timer::Release()
{
	delete instance;
	instance = nullptr;
}

float Timer::DeltaTime()
{
	return deltaTime;
}

void Timer::SetTimeScale(float t)
{
	timeScale = t;
}

float Timer::TimeScale()
{
	return timeScale;
}

Timer::Timer()
{
	Reset();
	timeScale = 1.0f;
	elapsedTicks = 0;
	deltaTime = 0.0f;
}

Timer::~Timer()
{

}

void Timer::Reset()
{
	startTicks = SDL_GetTicks();
}

void Timer::Update()
{
	elapsedTicks = SDL_GetTicks() - startTicks;
	deltaTime = elapsedTicks * 0.001f; // convert from ticks (~1ms) to seconds
}