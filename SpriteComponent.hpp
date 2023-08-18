#pragma once
#include <map>
#include "SDL.h"
#include "Components.hpp"
#include "Graphics.hpp"
#include "Assets.hpp"
#include "Animation.hpp"

class DiskSpriteComponent : public Component
{

private:

	Graphics* graphics;
	Assets* assets;

	DiskTransformComponent* transform;
	SDL_Texture* texture;

	SDL_Rect destRect;

public:

	DiskSpriteComponent()
	{
		graphics = Graphics::GetInstance();
		assets = Assets::GetInstance();

		setTexture("assets/disk.png");
		
	}

	~DiskSpriteComponent()
	{
		graphics = nullptr;
		assets = nullptr;

		transform = nullptr;
		texture = nullptr;

	}

	void setTexture(std::string path)
	{
		texture = assets->GetTexture(path);
	}



	void init() override
	{
		transform = &entity->getComponent<DiskTransformComponent>();

	}

	void Update() override
	{
		destRect.x = static_cast<int>(transform->Centre()->x - transform->Radius());
		destRect.y = static_cast<int>(transform->Centre()->y - transform->Radius());

		destRect.h = destRect.w = static_cast<int>(2 * transform->Radius());
	}

	void draw() override
	{
		graphics->DrawTexture(texture, NULL, &destRect, transform->GetRotation());
	}

};

class RectSpriteComponent : public Component
{

private:

	Graphics* graphics;
	Assets* assets;

	RectTransformComponent* transform;
	SDL_Texture* texture1;
	SDL_Texture* texture2;

	SDL_Rect destRect;

public:

	bool change = false;

	RectSpriteComponent()
	{
		graphics = Graphics::GetInstance();
		assets = Assets::GetInstance();

		setTextures("assets/rect1.png", "assets/rect2.png");

	}

	~RectSpriteComponent()
	{
		graphics = nullptr;
		assets = nullptr;

		transform = nullptr;
		texture1 = nullptr;

	}

	void setTextures(std::string path1, std::string path2)
	{
		texture1 = assets->GetTexture(path1);
		texture2 = assets->GetTexture(path2);
	}



	void init() override
	{
		transform = &entity->getComponent<RectTransformComponent>();

	}

	void EarlyUpdate() override
	{
		change = false;
	}

	void Update() override
	{
		destRect = transform->rect.SDLCast();
	}

	void draw() override
	{
		graphics->DrawTexture(change ? texture2 : texture1, NULL, &destRect, transform->GetRotation());
	}

};
