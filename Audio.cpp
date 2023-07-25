#include "Audio.hpp"

Audio* Audio::instance = nullptr;

Audio::Audio()
{
	assets = Assets::GetInstance();

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		printf("SDL Mix could not initialize! Mix_Error: %s\n", Mix_GetError());
	}
}

Audio::~Audio()
{
	assets = nullptr;
	Mix_Quit();
}

Audio* Audio::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Audio();
	}

	return instance;
}

void Audio::Release()
{
	delete instance;
	instance = nullptr;
}

void Audio::PlayMusic(std::string path, int loops)
{
	Mix_PlayMusic(assets->GetMusic(path), loops);
}

void Audio::PauseMusic()
{
	if (Mix_PlayingMusic() != 0)
	{
		Mix_PauseMusic();
	}
}

void Audio::ResumeMusic()
{
	if (Mix_PausedMusic() != 0)
	{
		Mix_ResumeMusic();
	}
}

void Audio::PlaySFX(std::string path, int loops, int channel)
{
	Mix_PlayChannel(channel, assets->GetSFX(path), loops);
}