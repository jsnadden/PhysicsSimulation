#pragma once
#include "Assets.hpp"

class Audio
{
public:

	static Audio* GetInstance();
	static void Release();

	void PlayMusic(std::string path, int loops = -1);
	void PauseMusic();
	void ResumeMusic();

	void PlaySFX(std::string path, int loops = 0, int channel = 0);

private:

	static Audio* instance;

	Audio();
	~Audio();
	
	Assets* assets;

};

