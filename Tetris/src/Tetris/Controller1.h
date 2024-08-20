//#pragma once
//#include "Board.h"
//#include <string>
//
//
//class Controller
//{
//public:
//	Controller(Board* board);
//	~Controller();
//
//public:
//	void setCurrentShape(Shape* shape);
//	void moveShape(Direction dir, bool clamp, TetrisState prevState);
//	const double getReward(TetrisState previousState);
//	double updateRewards(TetrisState currentState, TetrisState prevState);
//	bool canUpdateRewards();
//
//private:
//	const bool hasCollided(int nextX, int nextY);
//	void removeLines();
//	void addStats(int lines);
//	const bool isGameover();
//	void placePiece();
//	int calculateHoles(TetrisState state);
//private:
//	Shape* currentShape = nullptr;
//	Board* board;
//	bool gameOver = false;
//	int movePieceIteration = 0;
//	int currentIterations = 0;
//	bool canUpdate = false;
//	double reward = 0;
//};