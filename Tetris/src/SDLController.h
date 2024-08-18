#pragma once
#include "Tetris/Controller.h"

class SDLController
{
public:
	SDLController(Controller* controller);

public:
	void Update(Direction dir, TetrisState prevState);
	void Update();
	Controller* getController() { return controller; }

private:
	Controller* controller = nullptr;
};