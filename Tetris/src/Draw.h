#pragma once
#include "TetrisV2/Board.h"

class Draw
{
public:
	Draw(Board* board);

public:
	void drawBoard();
	void drawNextPieces();
private:
	void drawGhostPiece();
	void showStats();
private:
	Board* board = nullptr;
};