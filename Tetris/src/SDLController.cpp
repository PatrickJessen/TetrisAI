#include "SDLController.h"
#include "SDL/Core/Input.h"

SDLController::SDLController(Controller* controller)
	:controller(controller)
{
}

void SDLController::Update(Action action)
{
	controller->runEpisode(action);
}

void SDLController::Update()
{
	Action dir = Action::DOWN;
	if (currentIterations > maxIterations) {
		currentIterations = 0;
		if (Input::KeyState(Key::S)) {
			dir = Action::DOWN;
		}
		else if (Input::KeyState(Key::A)) {
			dir = Action::LEFT;
		}
		else if (Input::KeyState(Key::D)) {
			dir = Action::RIGHT;
		}
		if (Input::KeyPressed(Key::SPACE)) {
			dir = Action::SPACE;
		}
		if (Input::KeyPressed(Key::W)) {
			dir = Action::UP;
		}
	}
	controller->moveShape(dir);
	currentIterations++;
}
