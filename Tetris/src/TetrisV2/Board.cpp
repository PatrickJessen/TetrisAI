#include "Board.h"
#include <iostream>
#include <algorithm>

Board::Board(int width, int height)
{
    this->width = width;
    this->height = height;
	initializeBoard();
    srand((unsigned)time(NULL));
}

void Board::initializeBoard()
{
	grid.resize(height, std::vector<int>(width, 0));
}

void Board::insertShapeToBoard(const Shape& shape)
{
    const auto& shapeMatrix = shape.getShapeMatrix();
    int shapeHeight = shapeMatrix.size();
    int shapeWidth = shapeMatrix[0].size();

    int xPos = shape.getXPos();
    int yPos = shape.getYPos();

    // Insert the shape into the board grid
    for (int i = 0; i < shapeHeight; ++i) {
        for (int j = 0; j < shapeWidth; ++j) {
            if (shapeMatrix[i][j] != 0) { // There's a block in the shape at (i, j)
                int boardX = xPos + j;
                int boardY = yPos + i;

                // Ensure the indices are within the board boundaries
                if (boardX >= 0 && boardX < width && boardY >= 0 && boardY < height) {
                    grid[boardY][boardX] = shapeMatrix[i][j]; // Place the shape's block on the board
                }
            }
        }
    }
}

int Board::clearLines()
{
    int linesCleared = 0;

    // Iterate through the rows from bottom to top
    for (int y = height - 1; y >= 0; --y) {
        bool isFullLine = true;

        // Check if the current row is full
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == 0) {
                isFullLine = false;
                break;
            }
        }

        if (isFullLine) {
            // If the row is full, clear it and shift all rows above down by one
            linesCleared++;

            // Shift all rows above this one down
            for (int k = y; k > 0; --k) {
                grid[k] = grid[k - 1];
            }

            // Clear the top row (which has now been shifted down)
            grid[0] = std::vector<int>(width, 0);

            // After shifting rows, check the same row again
            y++;
        }
    }

    return linesCleared;
}

bool Board::checkGameOver(const Shape& shape)
{
    const auto& shapeMatrix = shape.getShapeMatrix();
    int shapeHeight = shapeMatrix.size();
    int shapeWidth = shapeMatrix[0].size();

    int xPos = shape.getXPos(); // Typically, the spawn position is at the top center
    int yPos = shape.getYPos(); // Usually at the top of the board, e.g., yPos = 0

    // Check if the shape can be placed at the starting position
    for (int i = 0; i < shapeHeight; ++i) {
        for (int j = 0; j < shapeWidth; ++j) {
            if (shapeMatrix[i][j] != 0) { // There is a block in the shape
                int boardX = xPos + j;
                int boardY = yPos + i;

                // Check for collision with existing blocks
                if (boardX >= 0 && boardX < width && boardY >= 0 && boardY < height) {
                    if (grid[boardY][boardX] != 0) {
                        return true; // Collision detected, game over
                    }
                }
                else if (boardY >= height) {
                    return true; // If the shape is trying to place below the board
                }
            }
        }
    }

    return false; // No collision, game continues
}

void Board::reset()
{
	for (auto& row : grid) {
		std::fill(row.begin(), row.end(), 0);
	}
}

bool Board::hasCollision(const Shape& shape) const
{
    const auto& shapeMatrix = shape.getShapeMatrix();
    int shapeHeight = shapeMatrix.size();
    int shapeWidth = shapeMatrix[0].size();

    int xPos = shape.getXPos();
    int yPos = shape.getYPos();

    // Iterate through the shape's matrix
    for (int i = 0; i < shapeHeight; ++i) {
        for (int j = 0; j < shapeWidth; ++j) {
            if (shapeMatrix[i][j] != 0) { // There's a block in the shape at (i, j)
                int boardX = xPos + j;
                int boardY = yPos + i;

                // Check if the shape is out of bounds (left, right, bottom)
                if (boardX < 0 || boardX >= width || boardY >= height) {
                    return true; // Collision with the boundary
                }

                // Check if the shape collides with an existing block on the board
                if (boardY >= 0 && grid[boardY][boardX] != 0) {
                    return true; // Collision with another block
                }
            }
        }
    }

    return false; // No collision detected
}

void Board::clearShapeFromBoard(const Shape& shape)
{
    const auto& shapeMatrix = shape.getShapeMatrix();
    int shapeHeight = shapeMatrix.size();
    int shapeWidth = shapeMatrix[0].size();

    int xPos = shape.getXPos();
    int yPos = shape.getYPos();

    // Iterate through the shape's matrix and clear its position on the board
    for (int i = 0; i < shapeHeight; ++i) {
        for (int j = 0; j < shapeWidth; ++j) {
            if (shapeMatrix[i][j] != 0) { // There's a block in the shape at (i, j)
                int boardX = xPos + j;
                int boardY = yPos + i;

                // Ensure the indices are within the board boundaries
                if (boardX >= 0 && boardX < width && boardY >= 0 && boardY < height) {
                    grid[boardY][boardX] = 0; // Clear the shape's block from the board
                }
            }
        }
    }
}

void Board::updateBoard(const Shape& shape)
{
    // First, clear the shape's previous position from the board
    clearShapeFromBoard(shape);

    // Then, insert the shape at its new position
    insertShapeToBoard(shape);
}

void Board::printBoard()
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << grid[y][x] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

Shape* Board::getNextShape()
{
    generateNextShape();
    return nextShape;
}

int Board::getWidth() const
{
    return width;
}

int Board::getHeight() const
{
    return height;
}

const std::vector<std::vector<int>>& Board::getGrid()
{
    return grid;
}

void Board::generateNextShape()
{
    int randNum = rand() % 6;
    Shape* shape = new Shape((ShapeType)randNum);
    nextShape = shape;
}

bool Board::isPartOfCurrentShape(int boardX, int boardY, const Shape& shape) const
{
    const auto& shapeMatrix = shape.getShapeMatrix();
    int shapeHeight = shapeMatrix.size();
    int shapeWidth = shapeMatrix[0].size();

    int xPos = shape.getXPos();
    int yPos = shape.getYPos();

    for (int i = 0; i < shapeHeight; ++i) {
        for (int j = 0; j < shapeWidth; ++j) {
            if (shapeMatrix[i][j] != 0) { // There's a block in the shape at (i, j)
                int currentX = xPos + j;
                int currentY = yPos + i;

                if (currentX == boardX && currentY == boardY) {
                    return true; // This block belongs to the shape being checked
                }
            }
        }
    }

    return false; // The block does not belong to the shape being checked
}
