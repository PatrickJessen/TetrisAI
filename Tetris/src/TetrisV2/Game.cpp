#include "Game.h"
#include "../SDL/Window/WindowsWindow.h"

void Game::update()
{
	Window* window = WindowsWindow::Create({ "Tetris" });
	Board* board = new Board();
	SDLController controller(new Controller(board));
	Draw draw = Draw(board);

	const int FPS = 30;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	while (isRunning)
	{
		frameStart = SDL_GetTicks();
		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
		controller.Update();
		draw.drawBoard();
		window->OnUpdate();
		window->Clear();
	}
	window->Clean();
}
