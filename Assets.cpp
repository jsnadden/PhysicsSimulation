#include "Assets.hpp"

Assets* Assets::instance = nullptr;

Assets::Assets()
{}

Assets::~Assets()
{
	for (auto& tex : textures)
	{
		if (tex.second != nullptr)
		{
			SDL_DestroyTexture(tex.second);
		}
	}
	textures.clear();

	for (auto& t : texts)
	{
		if (t.second != nullptr)
		{
			SDL_DestroyTexture(t.second);
		}
	}
	texts.clear();

	for (auto& f : fonts)
	{
		if (f.second != nullptr)
		{
			TTF_CloseFont(f.second);
		}
	}
	fonts.clear();

	for (auto& m : music)
	{
		if (m.second != nullptr)
		{
			Mix_FreeMusic(m.second);
		}
	}
	music.clear();

	for (auto& c : SFX)
	{
		if (c.second != nullptr)
		{
			Mix_FreeChunk(c.second);
		}
	}
	SFX.clear();
}

Assets* Assets::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Assets();
	}

	return instance;
}

void Assets::Release()
{
	delete instance;
	instance = nullptr;
}

SDL_Texture* Assets::GetTexture(std::string path)
{
	// This makes sure we don't load the same image more than once!
	if (textures[path] == nullptr)
	{
		textures[path] = Graphics::GetInstance()->LoadTexture(path);
	}

	return textures[path];
}

SDL_Texture* Assets::GetText(std::string text, std::string fontPath, int size, SDL_Color colour)
{
	TTF_Font* font = GetFont(fontPath, size);

	std::string key = text + fontPath + std::to_string(size) + (char)colour.r + (char)colour.b + (char)colour.g;

	if (texts[key] == nullptr)
	{
		texts[key] = Graphics::GetInstance()->LoadText(font, text, colour);
	}

	return texts[key];
}

TTF_Font* Assets::GetFont(std::string path, int size)
{
	std::string key = path + std::to_string(size);

	if (fonts[key] == nullptr)
	{
		fonts[key] = TTF_OpenFont(path.c_str(), size);
		if (fonts[key] == nullptr)
		{
			printf("Font %s failed to load! TTF_Error: %s\n", path.c_str(), TTF_GetError());
		}
	}

	return fonts[key];
}

Mix_Music* Assets::GetMusic(std::string path)
{
	if (music[path] == nullptr)
	{
		music[path] = Mix_LoadMUS(path.c_str());
		if (music[path] == nullptr)
		{
			printf("Music file %s failed to load! Mix_Error: %s\n", path.c_str(), Mix_GetError());
		}
	}

	return music[path];
}

Mix_Chunk* Assets::GetSFX(std::string path)
{
	if (SFX[path] == nullptr)
	{
		SFX[path] = Mix_LoadWAV(path.c_str());
		if (SFX[path] == nullptr)
		{
			printf("SFX file %s failed to load! Mix_Error: %s\n", path.c_str(), Mix_GetError());
		}
	}

	return SFX[path];
}