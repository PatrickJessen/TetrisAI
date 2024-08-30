#pragma once
#include "TetrisV2/Controller.h"

class SDLController
{
public:
	SDLController(Controller* controller);

public:
	void Update(Action action);
	void Update();
	Controller* getController() { return controller; }

private:
	Controller* controller = nullptr;
	int currentIterations = 0;
	int maxIterations = 1;
};