//#include "Controller.h"
//#include <iostream>
//#include <random>
//
//Controller::Controller(Board* board)
//	:board(board)
//{
//	currentShape = board->getCurrentShape();
//	movePieceIteration = 2;
//}
//
//Controller::~Controller()
//{
//}
//
//void Controller::setCurrentShape(Shape* shape)
//{
//	currentShape = shape;
//}
//
//void Controller::moveShape(Direction dir, bool clamp, TetrisState prevState)
//{
//	canUpdate = false;
//	reward = 0.0;
//	board->clearShapeFromBoard();
//	board->setAction((int)dir);
//	if (!isGameover()) {
//		int posX = 0;
//		int posY = 0;
//
//		// Move piece down automatically based on iteration count
//		if (currentIterations++ >= movePieceIteration) {
//			currentShape->setPos(0, 1);
//			currentIterations = 0;
//		}
//		// Handle collision and place the piece if necessary
//		if (hasCollided(currentShape->getXPos(), currentShape->getYPos() + 1)) {
//			placePiece();
//			//updateRewards(board->getCurrentState(), prevState);
//			canUpdate = true;
//			return;
//		}
//
//		// Process input direction
//		if (currentShape != nullptr) {
//			switch (dir) {
//			case Direction::UP:
//				currentShape->rotate();
//				// Check collision after moving/rotating the piece
//				break;
//			case Direction::DOWN:
//				posY++;
//				break;
//			case Direction::LEFT:
//				posX--;
//				break;
//			case Direction::RIGHT:
//				posX++;
//				break;
//			case Direction::SPACE:
//				// Drop the piece instantly
//				while (!hasCollided(currentShape->getXPos(), currentShape->getYPos() + 1)) {
//					currentShape->setPos(0, 1);
//				}
//				placePiece();
//				updateRewards(board->getCurrentState(), prevState);
//				canUpdate = true;
//				return;
//			default:
//				break;
//			}
//			// Update the ghost position
//			currentShape->resetGhostPos();
//			while (!hasCollided(currentShape->getXPos(), currentShape->getGhostYPos() + 1)) {
//				currentShape->setGhostYPos(1);
//			}
//
//			// Check collision after moving/rotating the piece
//			if (!hasCollided(currentShape->getXPos() + posX, currentShape->getYPos() + posY)) {
//				currentShape->setPos(posX, posY);
//			}
//			if (!hasCollided(currentShape->getXPos(), currentShape->getYPos() + posY)) {
//				board->insertShapeToBoard();
//			}
//
//
//			// Insert the shape back into the board
//		}
//	}
//	else {
//		board->setIsGameOver(true);
//		canUpdate = true;
//		board->restart();
//	}
//}
//
//
//
//const double Controller::getReward(TetrisState previousState)
//{
//	return reward;
//}
//
//double Controller::updateRewards(TetrisState currentState, TetrisState prevState)
//{
//	double reward = 0.0;
//
//	// Reward for clearing lines
//	int lines_cleared = currentState.lines_cleared - prevState.lines_cleared;
//	switch (lines_cleared) {
//	case 1:
//		reward += 10;
//		break;
//	case 2:
//		reward += 30;
//		break;
//	case 3:
//		reward += 50;
//		break;
//	case 4:
//		reward += 100;
//		break;
//	}
//
//	// Penalty for increasing the number of holes
//	int holes_created = currentState.num_holes - prevState.num_holes;
//	if (holes_created > 0) {
//		reward -= 5 * holes_created;
//	}
//	else if (holes_created < 0) {
//		reward += 5 * (-holes_created); // Reward for filling holes
//	}
//
//	// Penalty for increased bumpiness
//	int bumpiness_increase = currentState.bumpiness - prevState.bumpiness;
//	if (bumpiness_increase > 0) {
//		reward -= bumpiness_increase;
//	}
//
//	// Small penalty for landing height (to encourage lower placements)
//	reward -= 0.1 * currentState.landing_height;
//
//	// Penalty if the game is over
//	if (board->getIsGameOver()) {
//		reward -= 1000;
//		return reward;
//	}
//	reward += 0.1;
//	return reward;
//}
//
//bool Controller::canUpdateRewards()
//{
//	return canUpdate;
//}
//
//
//const bool Controller::hasCollided(int nextX, int nextY)
//{
//	const auto& shapeMatrix = currentShape->getShape();
//	int shapeHeight = shapeMatrix.size();
//	int shapeWidth = shapeMatrix[0].size();
//
//	const auto boardMatrix = board->getBoard();
//
//	for (int i = 0; i < shapeHeight; ++i) {
//		for (int j = 0; j < shapeWidth; ++j) {
//			if (shapeMatrix[i][j] == 1) {
//				int boardX = nextX + j;
//				int boardY = nextY + i;
//
//				if (boardX < 0 || boardX >= board->getWidth() ||
//					boardY < 0 || boardY >= board->getHeight() ||
//					boardMatrix[boardX][boardY] == 1) {
//					return true;
//				}
//			}
//		}
//	}
//	return false;
//}
//
//void Controller::removeLines()
//{
//	int lines = 0;
//	// Ensure the row indices are within the board boundaries
//	int startRow = std::max(0, currentShape->getYPos());
//	int f = currentShape->getYPos() + currentShape->getShape().size() - 1;
//	int endRow = std::min(board->getHeight() - 1, f);
//
//	// Traverse only the affected rows from bottom to top
//	for (int i = endRow; i >= startRow; --i) {
//		bool isFullLine = true;
//
//		// Check if the current row is full
//		for (int j = 0; j < board->getWidth(); ++j) {
//			if (board->getBoard()[j][i] == 0) {
//				isFullLine = false;
//				break;
//			}
//		}
//
//		// If the row is full, remove it and shift the rows above it down
//		if (isFullLine) {
//			lines++;
//			for (int k = i; k > 0; --k) {
//				// Copy the row above to the current row
//				for (int j = 0; j < board->getWidth(); ++j) {
//					board->getBoard()[j][k] = board->getBoard()[j][k - 1];
//				}
//			}
//
//			// Clear the top row since everything has shifted down
//			for (int j = 0; j < board->getWidth(); ++j) {
//				board->getBoard()[j][0] = 0;
//			}
//
//			// After shifting, check the same row again (which now contains the row above it)
//			i++;
//		}
//	}
//	if (lines > 0) {
//		std::cout << "Lines cleared: " << lines << "\n";
//	}
//	board->setLinesCleared(lines);
//	addStats(lines);
//}
//
//void Controller::addStats(int lines)
//{
//	int score = 0;
//	if (lines == 1) {
//		//reward = 1.0;
//		score = 100;
//	}
//	else if (lines == 2) {
//		//reward = 2.0;
//		score = 300;
//	}
//	else if (lines == 3) {
//		//reward = 5.0;
//		score = 500;
//	}
//	else if (lines == 4) {
//		//reward = 10.0;
//		score = 800;
//	}
//	board->setStats(lines, score);
//}
//
//const bool Controller::isGameover()
//{
//	const auto& matrix = currentShape->getShape();
//	int height = currentShape->getShape().size();
//	int width = currentShape->getShape()[0].size();
//
//	for (int i = 0; i < height; ++i) {
//		for (int j = 0; j < width; ++j) {
//			if (matrix[j][i] == 1) {  // There is a block in the shape
//				int boardX = 4 + j;
//				int boardY = 0 + i;
//
//				// Check if the block overlaps with an existing block on the board
//				if (board->getBoard()[boardX][boardY] != 0) {
//					reward = -10.0;
//					board->setIsGameOver(true);
//					return true; // Game over condition met
//				}
//			}
//		}
//	}
//	return false;
//}
//
//void Controller::placePiece()
//{
//	board->insertShapeToBoard();
//	removeLines();
//	board->setCurrentShape();
//	currentShape = board->getCurrentShape();
//	//board->insertShapeToBoard();
//}
//
//int Controller::calculateHoles(TetrisState state)
//{
//	int holes = 0;
//	for (int c = 0; c < board->getHeight(); ++c) { // Iterate over each column
//		bool blockFound = false;
//		for (int r = 0; r < board->getWidth(); ++r) { // Iterate over each row in the column
//			if (board->getBoard()[r][c] != 0) {
//				blockFound = true; // Found a block, mark it
//			}
//			else if (blockFound) {
//				holes++; // Count a hole if it's below a block
//			}
//		}
//	}
//	return holes;
//}
