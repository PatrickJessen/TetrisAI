#pragma once
#include "TetrisV2/Controller.h"

class SDLController
{
public:
	SDLController(Controller* controller);

public:
	void Update(Direction dir);
	void Update();
	Controller* getController() { return controller; }

private:
	Controller* controller = nullptr;
};