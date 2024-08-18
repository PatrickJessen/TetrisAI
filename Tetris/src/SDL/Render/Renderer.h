#pragma once
#include "SDL.h"

class Renderer
{
public:
	static void Init(SDL_Window* window, int width, int height);
	static SDL_Renderer* GetRenderer();
	static void SetBlendMode(SDL_BlendMode blendMode);
	static void SetColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	static void SetClearColor(SDL_Color color);
	static void SetAlphaColor(Uint8 a);
	static SDL_Color GetClearColor();
	static void Render();
	static void Clear();
	static void Destroy();

	static int GetWindowWidth();
	static int GetWindowHeight();

};