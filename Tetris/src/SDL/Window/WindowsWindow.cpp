#include "WindowsWindow.h"

Window* Window::Create(const WindowProps& props)
{
	return new WindowsWindow(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
	Init(props);
}

WindowsWindow::~WindowsWindow()
{

}

void WindowsWindow::OnUpdate()
{

	Input.Update();
	Renderer::Render();
}

void WindowsWindow::Clear()
{
	Renderer::Clear();
	Renderer::SetColor(Renderer::GetClearColor().r, Renderer::GetClearColor().g, Renderer::GetClearColor().b, Renderer::GetClearColor().a);
	SDL_Rect rect = { 0, 0, data.Width, data.Height };

	SDL_RenderFillRect(Renderer::GetRenderer(), &rect);
}

void WindowsWindow::Clean()
{
	SDL_DestroyWindow(window);
	Renderer::Destroy();
	SDL_Quit();
}

void WindowsWindow::Init(const WindowProps& props)
{
	data.Title = props.Title;
	data.Width = props.Width;
	data.Height = props.Height;
	data.Fullscreen = props.Fullscreen;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(data.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, data.Width, data.Height, data.Fullscreen);

		SDL_GL_MakeCurrent(window, context);

		Renderer::Init(window, data.Width, data.Height);
		Renderer::SetBlendMode(SDL_BLENDMODE_BLEND);
		SDL_Color color = { 44, 40, 42, 255 };
		Renderer::SetClearColor(color);
		SDL_SetWindowResizable(window, SDL_TRUE);
	}
	Input.Update();
}