#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Graphics
{
public:

	static const int SCREEN_WIDTH = 800;
	static const int SCREEN_HEIGHT = 640;

	static Graphics* GetInstance();
	static void Release();
	static bool HasInitialised();

	SDL_Texture* LoadTexture(std::string path);
	void DrawTexture(SDL_Texture* tex, SDL_Rect* sRect = nullptr, SDL_Rect* dRect = nullptr, float rot = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
	SDL_Texture* LoadText(TTF_Font* font, std::string text, SDL_Color colour);


	void ClearRenderer();
	void Render();

private:

	SDL_Window* window;
	SDL_Renderer* renderer;

	static Graphics* instance;
	static bool initialised;

	Graphics();
	~Graphics();

	bool Init();

};

