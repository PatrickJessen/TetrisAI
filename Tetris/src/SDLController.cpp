#include "SDLController.h"
#include "SDL/Core/Input.h"

SDLController::SDLController(Controller* controller)
	:controller(controller)
{
}

void SDLController::Update(Direction dir)
{
	if (Input::KeyState(Key::S)) {
		dir = Direction::DOWN;
	}
	else if (Input::KeyState(Key::A)) {
		dir = Direction::LEFT;
	}
	else if (Input::KeyState(Key::D)) {
		dir = Direction::RIGHT;
	}
	if (Input::KeyPressed(Key::SPACE)) {
		dir = Direction::SPACE;
	}
	if (Input::KeyPressed(Key::W)) {
		dir = Direction::UP;
	}
	controller->moveShape(dir);
}

void SDLController::Update()
{
	Direction dir = Direction::DOWN;
	if (Input::KeyState(Key::S)) {
		dir = Direction::DOWN;
	}
	else if (Input::KeyState(Key::A)) {
		dir = Direction::LEFT;
	}
	else if (Input::KeyState(Key::D)) {
		dir = Direction::RIGHT;
	}
	if (Input::KeyPressed(Key::SPACE)) {
		dir = Direction::SPACE;
	}
	if (Input::KeyPressed(Key::W)) {
		dir = Direction::UP;
	}
	controller->moveShape(dir);
}
