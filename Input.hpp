#pragma once
#include <SDL.h>
#include <string>
#include "Vector2D.hpp"

class Input
{
public:

	enum MOUSE_BUTTON {left = 0, right, middle, back, forward};

	static Input* GetInstance();
	static void Release();

	bool KeyDown(SDL_Scancode scancode);
	bool KeyPressed(SDL_Scancode scancode);
	bool KeyReleased(SDL_Scancode scancode);

	bool MouseButtonDown(MOUSE_BUTTON button);
	bool MouseButtonPressed(MOUSE_BUTTON button);
	bool MouseButtonReleased(MOUSE_BUTTON button);

	Vector2D MousePosition();

	void Update();
	void UpdatePrevious();

private:

	static Input* instance;

	Uint8* previousKeyboardState;
	const Uint8* keyboardState;
	int keyLength;

	Uint32 previousMouseState;
	Uint32 mouseState;
	int mouseX;
	int mouseY;

	Input();
	~Input();

};

