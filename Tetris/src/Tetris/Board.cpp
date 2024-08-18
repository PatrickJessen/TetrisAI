#include "Board.h"
#include <time.h>
#include <iostream>
#include <random>

Board::Board(int width, int height, int size)
	:width(width), height(height), size(size)
{
	initializeBoard();
	srand((unsigned)time(NULL));
	currentShape = setNewShape();
	currentShape->setPos(0, 1);
	setNextShapes();
}

void Board::insertShapeToBoard()
{
	for (int i = 0; i < currentShape->getShape().size(); i++) {
		for (int j = 0; j < currentShape->getShape()[0].size(); j++) {
			while (currentShape->getXPos() + j < 0) {
				currentShape->setPos(1, 0);
			}
			while (currentShape->getXPos() + j > width - 1) {
				currentShape->setPos(-1, 0);
			}
			if (currentShape->getShape()[j][i] == 1) {
				board[currentShape->getXPos() + j][currentShape->getYPos() + i] = 1;
			}
		}
	}
}

Shape* Board::getCurrentShape()
{
	return currentShape;
}

Shape* Board::getHoldShape()
{
	return holdShape;
}

Shape* Board::getNextShapeByIndex(const int& index)
{
	return nextShapes[index];
}

const int& Board::getWidth() const
{
	return width;
}

const int& Board::getHeight() const
{
	return height;
}

Stats Board::getStats()
{
	return stats;
}

int** Board::getBoard()
{
	return board;
}

const int& Board::getSize() const
{
	return size;
}

const std::vector<Shape*>& Board::getNextShapes()
{
	return nextShapes;
}

void Board::restart()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			board[j][i] = 0;
		}
	}
	isGameOver = false;
	/*currentShape = setNewShape();
	currentShape->setPos(0, 1);
	setNextShapes();*/
}

const bool Board::getIsGameOver()
{
	return isGameOver;
}

void Board::setBoard(const int& x, const int& y, const int& value)
{
	board[y][x] = value;
}

void Board::setCurrentShape()
{
	currentShape = nextShapes[0];
	nextShapes.erase(nextShapes.begin() + 0);
	setNextShapes();
}

void Board::initializeBoard()
{
	board = new int* [width];
	for (int i = 0; i < width; i++) {
		board[i] = new int[height];
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			board[j][i] = 0;
		}
	}
}

std::array<int, 200> Board::flattenBoard()
{
	std::array<int, 200> flattenedGrid;
	for (int r = 0; r < height; ++r) {
		for (int c = 0; c < width; ++c) {
			flattenedGrid[r * width + c] = board[c][r];
		}
	}
	return flattenedGrid;
}

void Board::calculateBoardProperties(std::array<int, 20>& columnHeights, int& maxHeight, int& holes)
{
	holes = 0;
	maxHeight = 0;

	for (int c = 0; c < height; ++c) {
		bool blockFound = false;
		int columnHeight = 0;
		int columnHoles = 0;

		for (int r = 0; r < width; ++r) {
			if (board[r][c] != 0) {
				if (!blockFound) {
					columnHeight = height - c;
					blockFound = true;
				}
			}
			else if (blockFound) {
				// Count holes after the first block in the column
				columnHoles++;
			}
		}

		columnHeights[c] = columnHeight;
		holes += columnHoles;
	}

	// Find the maximum height among all columns
	maxHeight = *std::max_element(columnHeights.begin(), columnHeights.end());
}

Shape* Board::setNewShape()
{
	int randNum = rand() % shapes.size();
	return new Shape(shapes[randNum], 4, -1, randNum);
}

void Board::setNextShapes()
{
	while (nextShapes.size() < 3)
	{
		nextShapes.push_back(setNewShape());
	}
}

void Board::setStats(const int& lines, const int& score)
{
	stats.Lines += lines;
	stats.Score += score;
}

void Board::clearShapeFromBoard()
{
	for (int i = 0; i < currentShape->getShape().size(); i++) {
		for (int j = 0; j < currentShape->getShape()[0].size(); j++) {
			if (currentShape->getShape()[j][i] == 1) {
				while (currentShape->getXPos() + j < 0) {
					currentShape->setPos(1, 0);
				}
				while (currentShape->getXPos() + j > width - 1) {
					currentShape->setPos(-1, 0);
				}
				board[currentShape->getXPos() + j][currentShape->getYPos() + i] = 0;
			}
		}
	}
}

TetrisState Board::getCurrentState()
{
	TetrisState state;
	//state.grid = flattenBoard();
	state.currentPiece = currentShape->getType();
	state.rotationState = currentShape->getRotationState();
	calculateBoardProperties(state.columnHeights, state.maxHeight, state.holes);
	state.linesCleared = linesCleared;
	return state;
}

void Board::setState()
{
}

void Board::setLinesCleared(const int& value)
{
	linesCleared = value;
}

void Board::setIsGameOver(const bool value)
{
	isGameOver = value;
}
