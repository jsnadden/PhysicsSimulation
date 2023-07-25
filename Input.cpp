#include "Input.hpp"

Input* Input::instance = nullptr;

Input::Input()
{
	keyboardState = SDL_GetKeyboardState(&keyLength);
	previousKeyboardState = new Uint8[keyLength];
	memcpy(previousKeyboardState,keyboardState, keyLength);
}

Input::~Input()
{
	delete[] previousKeyboardState;
	previousKeyboardState = nullptr;
}



Input* Input::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Input();
	}

	return instance;
}

void Input::Release()
{
	delete instance;
	instance = nullptr;
}

bool Input::KeyDown(SDL_Scancode scancode)
{
	return keyboardState[scancode];
}

bool Input::KeyPressed(SDL_Scancode scancode)
{
	return keyboardState[scancode] && !previousKeyboardState[scancode];
}

bool Input::KeyReleased(SDL_Scancode scancode)
{
	return !keyboardState[scancode] && previousKeyboardState[scancode];
}

bool Input::MouseButtonDown(Input::MOUSE_BUTTON button)
{
	Uint32 bitmask = 0;

	switch (button)
	{
	case left :
		bitmask = SDL_BUTTON_LMASK;
		break;

	case right:
		bitmask = SDL_BUTTON_RMASK;
		break;

	case middle:
		bitmask = SDL_BUTTON_MMASK;
		break;

	case back:
		bitmask = SDL_BUTTON_X1MASK;
		break;

	case forward:
		bitmask = SDL_BUTTON_X2MASK;
		break;

	default:
		break;
	}

	return (mouseState & bitmask);
}

bool Input::MouseButtonPressed(Input::MOUSE_BUTTON button)
{
	Uint32 bitmask = 0;

	switch (button)
	{
	case left:
		bitmask = SDL_BUTTON_LMASK;
		break;

	case right:
		bitmask = SDL_BUTTON_RMASK;
		break;

	case middle:
		bitmask = SDL_BUTTON_MMASK;
		break;

	case back:
		bitmask = SDL_BUTTON_X1MASK;
		break;

	case forward:
		bitmask = SDL_BUTTON_X2MASK;
		break;

	default:
		break;
	}

	return (mouseState & bitmask) && !(previousMouseState & bitmask);
}

bool Input::MouseButtonReleased(Input::MOUSE_BUTTON button)
{
	Uint32 bitmask = 0;

	switch (button)
	{
	case left:
		bitmask = SDL_BUTTON_LMASK;
		break;

	case right:
		bitmask = SDL_BUTTON_RMASK;
		break;

	case middle:
		bitmask = SDL_BUTTON_MMASK;
		break;

	case back:
		bitmask = SDL_BUTTON_X1MASK;
		break;

	case forward:
		bitmask = SDL_BUTTON_X2MASK;
		break;

	default:
		break;
	}

	return !(mouseState & bitmask) && (previousMouseState & bitmask);
}

Vector2D Input::MousePosition()
{
	return Vector2D(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

void Input::Update()
{
	mouseState = SDL_GetMouseState(&mouseX, &mouseY);
}

void Input::UpdatePrevious()
{
	memcpy(previousKeyboardState, keyboardState, keyLength);
	previousMouseState = mouseState;
}