#pragma once
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include <string>
#include "../Render/Renderer.h"
#include "Input.h"

struct WindowProps
{
	std::string Title;
	unsigned int Width;
	unsigned int Height;
	bool Fullscreen;

	WindowProps(const std::string& title = "Editor", unsigned int width = 1280, unsigned int height = 720, bool fullscreen = false)
		: Title(title), Width(width), Height(height), Fullscreen(fullscreen) {}
};

class Window
{
public:
	virtual void OnUpdate() = 0;
	virtual void Clear() = 0;
	virtual void Clean() = 0;
	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	static Window* Create(const WindowProps& props = WindowProps());
};