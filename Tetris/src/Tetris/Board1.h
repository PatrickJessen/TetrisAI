#pragma once
#include "Shape.h"
#include <array>

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPACE,
	NONE,
};

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
	std::array<int, 10> heights;      // Heights of each column
	std::array<int, 9> height_diffs;   // Differences between adjacent column heights
	int num_holes;                     // Total number of holes
	int max_height;                    // Maximum height of the columns
	int bumpiness;                     // Total bumpiness
	int lines_cleared;                 // Lines cleared in the last move
	int landing_height;                // Height where the current piece landed

	bool operator==(const TetrisState& other) const {
		return heights == other.heights &&
			height_diffs == other.height_diffs &&
			num_holes == other.num_holes &&
			max_height == other.max_height &&
			bumpiness == other.bumpiness &&
			lines_cleared == other.lines_cleared &&
			landing_height == other.landing_height;
	}

	// Function to convert the state into a vector for Q-learning
	std::vector<float> to_vector() const {
		std::vector<float> state_vector;
		state_vector.insert(state_vector.end(), heights.begin(), heights.end());
		state_vector.insert(state_vector.end(), height_diffs.begin(), height_diffs.end());
		state_vector.push_back(num_holes);
		state_vector.push_back(max_height);
		state_vector.push_back(bumpiness);
		state_vector.push_back(lines_cleared);
		state_vector.push_back(landing_height);
		return state_vector;
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
	void setAction(int value);
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
	int action = 0;
	Shape* currentShape;
	Shape* holdShape;
	std::vector<Shape*> nextShapes;
	Stats stats;
	//std::vector<std::vector<std::vector<int>>> shapes{
	//	{ // I
	//	{0, 0, 0, 0},
	//	{0, 0, 0, 0},
	//	{0, 0, 0, 0},
	//	{1, 1, 1, 1}
	//	},

	//	{ // O
	//		{1, 1},
	//		{1, 1}
	//	},

	//	{ // T
	//		{0, 0, 0},
	//		{0, 1, 0},
	//		{1, 1, 1}
	//	},

	//	{ // L
	//		{0, 0, 0},
	//		{0, 0, 1},
	//		{1, 1, 1}
	//	},

	//	{ // J
	//		{0, 0, 0},
	//		{1, 0, 0},
	//		{1, 1, 1}
	//	},

	//	{ // S
	//		{0, 0, 0},
	//		{0, 1, 1},
	//		{1, 1, 0}
	//	},

	//	{ // Z
	//		{0, 0, 0},
	//		{1, 1, 0},
	//		{0, 1, 1}
	//	}
	//};
};