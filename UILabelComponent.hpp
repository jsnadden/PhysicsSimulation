#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "ECS.hpp"
#include "Graphics.hpp"
#include "Assets.hpp"
#include "Game.hpp"


class UILabelComponent : public Component
{
private:

	Graphics* graphics;
	Assets* assets;

	SDL_Rect location;
	std::string textString;
	std::string font;
	SDL_Color textColour;
	SDL_Texture* texture;
	int textSize;

public:

	UILabelComponent(int xpos, int ypos, std::string text, std::string fontPath, int size, SDL_Color& colour)
	{
		graphics = Graphics::GetInstance();
		assets = Assets::GetInstance();

		location.x = xpos;
		location.y = ypos;
		textString = text;
		font = fontPath;
		textSize = size;
		textColour = colour;

		texture = assets->GetText(textString, font, textSize, textColour);
		SDL_QueryTexture(texture, NULL, NULL, &location.w, &location.h);
	}

	~UILabelComponent()
	{
		graphics = nullptr;
		assets = nullptr;
		texture = nullptr;
	}

	void draw() override
	{
		graphics->DrawTexture(texture, nullptr, &location);
	}

};