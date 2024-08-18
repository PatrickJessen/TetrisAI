#include "Renderer.h"

static SDL_Renderer* renderer;
static SDL_Color ClearColor;

static int windowWidth;
static int windowHeight;

void Renderer::Init(SDL_Window* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Renderer* Renderer::GetRenderer()
{
	return renderer;
}

void Renderer::SetBlendMode(SDL_BlendMode blendMode)
{
	SDL_SetRenderDrawBlendMode(renderer, blendMode);
}

void Renderer::SetColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Renderer::SetClearColor(SDL_Color color)
{
	ClearColor = { color.r, color.g, color.b, color.a };
}

void Renderer::SetAlphaColor(Uint8 a)
{
	ClearColor.a = a;
}

SDL_Color Renderer::GetClearColor()
{
	return ClearColor;
}

void Renderer::Render()
{
	SDL_RenderPresent(renderer);
}

void Renderer::Clear()
{
	SDL_RenderClear(renderer);
}

void Renderer::Destroy()
{
	SDL_DestroyRenderer(renderer);
}

int Renderer::GetWindowWidth()
{
	return windowWidth;
}

int Renderer::GetWindowHeight()
{
	return windowHeight;
}
