//#include "Board.h"
//#include <time.h>
//#include <iostream>
//#include <random>
//#include <functional>
//#include <numeric>
//
//
//Board::Board(int width, int height, int size)
//	:width(width), height(height), size(size)
//{
//	initializeBoard();
//	srand((unsigned)time(NULL));
//	currentShape = setNewShape();
//	currentShape->setPos(0, 1);
//	setNextShapes();
//}
//
//void Board::insertShapeToBoard()
//{
//	// Get the shape data
//	const auto& shape = currentShape->getShape();
//	int shapeHeight = shape.size();
//	int shapeWidth = shape[0].size();
//
//	// Calculate the actual occupied bounds of the shape
//	int minX = shapeWidth;
//	int maxX = 0;
//	for (int i = 0; i < shapeHeight; ++i) {
//		for (int j = 0; j < shapeWidth; ++j) {
//			if (shape[j][i] == 1) {
//				if (j < minX) minX = j;
//				if (j > maxX) maxX = j;
//			}
//		}
//	}
//
//	// Get the current X position of the shape
//	int xPos = currentShape->getXPos();
//
//	// Adjust position to ensure the shape stays within the board's bounds after rotation
//	if (xPos + minX < 0) {
//		// If the leftmost part of the shape is out of bounds, move it to the right
//		currentShape->setPos(-minX, 0);
//		xPos = currentShape->getXPos(); // Update xPos after adjustment
//	}
//
//	if (xPos + maxX >= width) {
//		// If the rightmost part of the shape is out of bounds, move it to the left
//		currentShape->setPos(width - (xPos + maxX + 1), 0);
//		xPos = currentShape->getXPos(); // Update xPos after adjustment
//	}
//
//	// Insert shape into the board using the adjusted X position
//	for (int i = 0; i < shapeHeight; i++) {
//		for (int j = 0; j < shapeWidth; j++) {
//			if (shape[j][i] == 1) {
//				// Ensure that the indices are valid before inserting
//				int boardX = xPos + j;
//				int boardY = currentShape->getYPos() + i;
//
//				if (boardX >= 0 && boardX < width && boardY >= 0 && boardY < height) {
//					board[boardX][boardY] = 1;
//				}
//			}
//		}
//	}
//}
//
//Shape* Board::getCurrentShape()
//{
//	return currentShape;
//}
//
//Shape* Board::getHoldShape()
//{
//	return holdShape;
//}
//
//Shape* Board::getNextShapeByIndex(const int& index)
//{
//	return nextShapes[index];
//}
//
//const int& Board::getWidth() const
//{
//	return width;
//}
//
//const int& Board::getHeight() const
//{
//	return height;
//}
//
//Stats Board::getStats()
//{
//	return stats;
//}
//
//int** Board::getBoard()
//{
//	return board;
//}
//
//const int& Board::getSize() const
//{
//	return size;
//}
//
//const std::vector<Shape*>& Board::getNextShapes()
//{
//	return nextShapes;
//}
//
//void Board::restart()
//{
//	for (int i = 0; i < height; i++) {
//		for (int j = 0; j < width; j++) {
//			board[j][i] = 0;
//		}
//	}
//	isGameOver = false;
//	/*currentShape = setNewShape();
//	currentShape->setPos(0, 1);
//	setNextShapes();*/
//}
//
//const bool Board::getIsGameOver()
//{
//	return isGameOver;
//}
//
//void Board::setBoard(const int& x, const int& y, const int& value)
//{
//	board[y][x] = value;
//}
//
//void Board::setCurrentShape()
//{
//	currentShape = nextShapes[0];
//	nextShapes.erase(nextShapes.begin() + 0);
//	setNextShapes();
//}
//
//void Board::initializeBoard()
//{
//	board = new int* [width];
//	for (int i = 0; i < width; i++) {
//		board[i] = new int[height];
//	}
//
//	for (int i = 0; i < width; ++i) {
//		for (int j = 0; j < height; ++j) {
//			board[i][j] = 0; // Initialize each cell to 0
//		}
//	}
//}
//
//std::array<int, 200> Board::flattenBoard()
//{
//	std::array<int, 200> flattenedGrid;
//	for (int r = 0; r < height; ++r) {
//		for (int c = 0; c < width; ++c) {
//			flattenedGrid[r * width + c] = board[c][r];
//		}
//	}
//	return flattenedGrid;
//}
//
//void Board::calculateBoardProperties(std::array<int, 20>& columnHeights, int& maxHeight, int& holes)
//{
//	holes = 0;
//	maxHeight = 0;
//
//	for (int c = 0; c < height; ++c) {
//		bool blockFound = false;
//		int columnHeight = 0;
//		int columnHoles = 0;
//
//		for (int r = 0; r < width; ++r) {
//			if (board[r][c] != 0) {
//				if (!blockFound) {
//					columnHeight = height - r;
//					blockFound = true;
//				}
//			}
//			else if (blockFound) {
//				// Count holes below the first block in the column
//				columnHoles++;
//			}
//		}
//
//		columnHeights[c] = columnHeight;
//		holes += columnHoles;
//	}
//
//	// Find the maximum height among all columns
//	maxHeight = *std::max_element(columnHeights.begin(), columnHeights.end());
//}
//
//Shape* Board::setNewShape()
//{
//	int randNum = rand() % shapes.size();
//	return new Shape(shapes[randNum], 4, -1, randNum);
//}
//
//void Board::setNextShapes()
//{
//	while (nextShapes.size() < 3)
//	{
//		nextShapes.push_back(setNewShape());
//	}
//}
//
//void Board::setStats(const int& lines, const int& score)
//{
//	stats.Lines += lines;
//	stats.Score += score;
//}
//
//void Board::clearShapeFromBoard()
//{
//	for (int i = 0; i < currentShape->getShape().size(); i++) {
//		for (int j = 0; j < currentShape->getShape()[0].size(); j++) {
//			if (currentShape->getShape()[j][i] == 1) {
//				while (currentShape->getXPos() + j < 0) {
//					currentShape->setPos(1, 0);
//				}
//				while (currentShape->getXPos() + j > width - 1) {
//					currentShape->setPos(-1, 0);
//				}
//				board[currentShape->getXPos() + j][currentShape->getYPos() + i] = 0;
//			}
//		}
//	}
//}
//
//TetrisState Board::getCurrentState()
//{
//	TetrisState state;
//	// Compute column heights
//	for (int i = 0; i < width; ++i) {
//		state.heights[i] = 0;
//		for (int j = 0; j < height; ++j) { // Assuming a standard board height of 20
//			if (board[i][j] != 0) {
//				state.heights[i] = height - j;
//				break;
//			}
//		}
//	}
//	// Compute height differences
//	for (int i = 0; i < width -1; ++i) {
//		state.height_diffs[i] = std::abs(state.heights[i] - state.heights[i + 1]);
//	}
//	// Compute the number of holes
//	state.num_holes = 0;
//	for (int i = 0; i < width; ++i) {
//		bool found_block = false;
//		for (int j = 0; j < height; ++j) {
//			if (board[i][j] != 0) {
//				found_block = true;
//			}
//			else if (found_block) {
//				state.num_holes++;
//			}
//		}
//	}
//	// Maximum height
//	state.max_height = *std::max_element(state.heights.begin(), state.heights.end());
//
//	// Bumpiness
//	state.bumpiness = std::accumulate(state.height_diffs.begin(), state.height_diffs.end(), 0);
//
//	// Landing height: Assuming `piece` has a method to give its bottommost row
//	state.landing_height = 20 - currentShape->getYPos(); // Adjust based on where the piece lands
//
//	// Lines cleared (needs to be calculated based on the resultant board after placing the piece)
//	state.lines_cleared = linesCleared; // Implement this function based on your board logic
//
//	return state;
//}
//
//void Board::setState()
//{
//}
//
//void Board::setLinesCleared(const int& value)
//{
//	linesCleared = value;
//}
//
//void Board::setIsGameOver(const bool value)
//{
//	isGameOver = value;
//}
//
//void Board::setAction(int value)
//{
//	action = value;
//}
