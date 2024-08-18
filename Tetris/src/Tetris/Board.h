#pragma once
#include "Shape.h"
#include <array>

struct Stats
{
public:
	int Score;
	int Lines;

	bool operator==(const Stats& other) const {
		return Score == other.Score && Lines == other.Lines;
	}
};

struct TetrisState {
	std::array<int, 200> grid; // Flattened grid (20x10)
	int currentPiece;          // 0 = I, 1 = O, 2 = T, 3 = S, 4 = Z, 5 = J, 6 = L
	int rotationState;         // 0 = 0°, 1 = 90°, 2 = 180°, 3 = 270°
	std::array<int, 20> columnHeights; // Heights of each column
	int maxHeight;             // Maximum height of the columns
	int holes;                 // Number of holes in the grid
	int linesCleared;          // Number of lines cleared by the last action

	bool operator==(const TetrisState& other) const {
		return grid == other.grid &&
			currentPiece == other.currentPiece &&
			rotationState == other.rotationState &&
			columnHeights == other.columnHeights &&
			maxHeight == other.maxHeight &&
			holes == other.holes &&
			linesCleared == other.linesCleared;
	}
};

class Board
{
public:
	Board(int width, int height, int size);

public:
	void insertShapeToBoard();
public:
	Shape* getCurrentShape();
	Shape* getHoldShape();
	Shape* getNextShapeByIndex(const int& index);
	const int& getWidth() const;
	const int& getHeight() const;
	Stats getStats();
	int** getBoard();
	const int& getSize() const;
	const std::vector<Shape*>& getNextShapes();
	void restart();
	const bool getIsGameOver();
public:
	void setBoard(const int& x, const int& y, const int& value);
	void setCurrentShape();
	Shape* setNewShape();
	void setNextShapes();
	void setStats(const int& lines, const int& score);
	void clearShapeFromBoard();
	TetrisState getCurrentState();
	void setState();
	void setLinesCleared(const int& value);
	void setIsGameOver(const bool value);
private:
	void initializeBoard();
	std::array<int, 200> flattenBoard();
	void calculateBoardProperties(std::array<int, 20>& columnHeights, int& maxHeight, int& holes);

private:
	int width; // 10
	int height; // 20
	int size;
	int linesCleared = 0;
	int** board = nullptr;
	bool isGameOver = false;
	Shape* currentShape;
	Shape* holdShape;
	std::vector<Shape*> nextShapes;
	Stats stats;
	std::vector<std::vector<std::vector<int>>> shapes{
		{ // I
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{1, 1, 1, 1}
		},

		{ // O
			{1, 1},
			{1, 1}
		},

		{ // T
			{0, 0, 0},
			{0, 1, 0},
			{1, 1, 1}
		},

		{ // L
			{0, 0, 0},
			{0, 0, 1},
			{1, 1, 1}
		},

		{ // J
			{0, 0, 0},
			{1, 0, 0},
			{1, 1, 1}
		},

		{ // S
			{0, 0, 0},
			{0, 1, 1},
			{1, 1, 0}
		},

		{ // Z
			{0, 0, 0},
			{1, 1, 0},
			{0, 1, 1}
		}
	};
};