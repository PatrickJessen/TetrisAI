#pragma once
#include "Board.h"
#include "../SDLController.h"
#include "../Draw.h"

struct Stats
{
public:
	int Score;
	int Lines;

	bool operator==(const Stats& other) const {
		return Score == other.Score && Lines == other.Lines;
	}
};

class Game
{
public:
	Game(std::string gen) {
		this->gen = gen;
	}
public:
	void update();
private:
	void startGame();
	void pauseGame();
	void endGame();
	void render();

private:
	std::string gen;
	bool isRunning;
};