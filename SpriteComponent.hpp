#pragma once
#include <map>
#include "SDL.h"
#include "Components.hpp"
#include "Graphics.hpp"
#include "Assets.hpp"
#include "Animation.hpp"

class SpriteComponent : public Component
{

private:

	Graphics* graphics;
	Assets* assets;

	TransformComponent* transform;
	SDL_Texture* texture;

	SDL_Rect destRect;

public:

	SpriteComponent()
	{
		graphics = Graphics::GetInstance();
		assets = Assets::GetInstance();

		setTexture("assets/disk.png");
		
	}

	~SpriteComponent()
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
		transform = &entity->getComponent<TransformComponent>();

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

