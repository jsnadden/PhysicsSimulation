#include "Graphics.hpp"

Graphics* Graphics::instance = nullptr;
bool Graphics::initialised = false;

Graphics* Graphics::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Graphics();
	}

	return instance;
}

void Graphics::Release()
{
	delete instance;
	instance = nullptr;

	initialised = false;
}

bool Graphics::HasInitialised()
{
	return initialised;
}

Graphics::Graphics()
{
	renderer = nullptr;
	initialised = Init();
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(window);
	window = nullptr;

	SDL_DestroyRenderer(renderer);
	renderer = nullptr;

	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

bool Graphics::Init()
{
	// Start SDL subsystems
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL Video could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create window, error check
	window = SDL_CreateWindow("Physics Sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Create renderer, error check, set background to white
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Start image library, error check
	int imgflags = IMG_INIT_PNG;
	if (!(IMG_Init(imgflags) & imgflags))
	{
		printf("Renderer could not be created! IMG_Error: %s\n", IMG_GetError());
		return false;
	}

	if (TTF_Init() < 0)
	{
		printf("SDL TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

SDL_Texture* Graphics::LoadTexture(std::string path)
{
	SDL_Texture* tex = nullptr;


	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == nullptr) {
		printf("Image %s could not be loaded! IMG_Error: %s\n", path.c_str(), IMG_GetError());
		return nullptr;
	}

	tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (tex == nullptr) {
		printf("Text surface creation failed! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(tempSurface);

	return tex;
}

SDL_Texture* Graphics::LoadText(TTF_Font* font, std::string text, SDL_Color colour)
{
	SDL_Texture* tex = nullptr;

	SDL_Surface* tempSurface = TTF_RenderText_Solid(font, text.c_str(), colour);
	if (tempSurface == nullptr) {
		printf("Text surface creation failed! TTF_Error: %s\n", TTF_GetError());
		return nullptr;
	}

	tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (tex == nullptr) {
		printf("Text surface creation failed! SDL_Error: %s\n", SDL_GetError());
	}

	SDL_FreeSurface(tempSurface);

	return tex;
}

void Graphics::ClearRenderer()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);
}

void Graphics::Render()
{
	SDL_RenderPresent(renderer);
}

void Graphics::DrawTexture(SDL_Texture* tex, SDL_Rect* sRect, SDL_Rect* dRect, float rot, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(renderer, tex, sRect, dRect, rot, NULL, flip);
}

void Graphics::DrawRectangle(SDL_Color colour, SDL_Rect* rect)
{
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderFillRect(renderer, rect);
}

void Graphics::DrawLine(SDL_Color colour, Vector2D start, Vector2D end)
{
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
	SDL_RenderDrawLine(renderer, (int)start.x, (int)start.y, (int)end.x, (int)end.y);
}

void Graphics::DrawPolygon(SDL_Color colour, Polygon poly)
{
	SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);

	int n = poly.Size();

	for (int i = 0; i < n; i++)
	{
		SDL_RenderDrawLine(renderer, poly.vertices[i].x, poly.vertices[i].y, poly.vertices[i + 1].x, poly.vertices[i + 1].y);
	}
}

