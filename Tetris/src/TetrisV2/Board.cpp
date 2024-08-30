#include "Board.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <array>

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
    previousGrid = grid;
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
    linesCleared = 0;

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
    gameOver = false;
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

void Board::setGameOver(const bool value)
{
    gameOver = value;
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
    currentShape = nextShape;
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

const std::vector<std::vector<int>>& Board::getPreviousGrid()
{
    return previousGrid;
}

TetrisState Board::extractState() const
{
    TetrisState state;

    // Calculate the heights of each column
    std::array<int, 10> heights = { 0 };
    for (int col = 0; col < width; ++col) {
        for (int row = 0; row < height; ++row) {
            if (grid[row][col] != 0) {
                heights[col] = height - row;
                break;
            }
        }
    }

    // Calculate aggregate height
    state.aggregate_height = std::accumulate(heights.begin(), heights.end(), 0);

    // Calculate the number of holes
    state.holes = 0;
    for (int col = 0; col < width; ++col) {
        bool foundBlock = false;
        for (int row = 0; row < height; ++row) {
            if (grid[row][col] != 0) {
                foundBlock = true;
            }
            else if (foundBlock && grid[row][col] == 0) {
                state.holes++;
            }
        }
    }

    // Calculate bumpiness
    state.bumpiness = 0;
    for (int col = 0; col < width - 1; ++col) {
        state.bumpiness += std::abs(heights[col] - heights[col + 1]);
    }

    // Set the number of lines cleared
    state.lines_cleared = linesCleared;

    // Optional: Calculate row and column transitions
    state.row_transitions = 0;
    state.column_transitions = 0;
    for (int row = 0; row < height; ++row) {
        for (int col = 1; col < width; ++col) {
            if ((grid[row][col] == 0 && grid[row][col - 1] != 0) ||
                (grid[row][col] != 0 && grid[row][col - 1] == 0)) {
                state.row_transitions++;
            }
        }
    }
    for (int col = 0; col < width; ++col) {
        for (int row = 1; row < height; ++row) {
            if ((grid[row][col] == 0 && grid[row - 1][col] != 0) ||
                (grid[row][col] != 0 && grid[row - 1][col] == 0)) {
                state.column_transitions++;
            }
        }
    }

    return state;
}

int Board::calculateBumpiness(const std::vector<std::vector<int>>& grid) const
{
    int width = grid[0].size();
    int height = grid.size();
    std::vector<int> heights(width, 0);

    // Calculate the height of each column
    for (int col = 0; col < width; ++col) {
        for (int row = 0; row < height; ++row) {
            if (grid[row][col] != 0) {
                heights[col] = height - row;
                break;
            }
        }
    }

    // Calculate bumpiness as the sum of the absolute differences between adjacent column heights
    int bumpiness = 0;
    for (int col = 0; col < width - 1; ++col) {
        bumpiness += std::abs(heights[col] - heights[col + 1]);
    }

    return bumpiness;
}

int Board::calculateAggregateHeight(const std::vector<std::vector<int>>& grid) const
{
    int width = grid[0].size();
    int height = grid.size();
    int aggregateHeight = 0;

    for (int col = 0; col < width; ++col) {
        for (int row = 0; row < height; ++row) {
            if (grid[row][col] != 0) {
                aggregateHeight += (height - row);
                break;
            }
        }
    }

    return aggregateHeight;
}

const bool Board::isGameOver()
{
    return gameOver;
}

bool Board::isPieceOnSolidGround(const Shape& currentShape) const
{
    // Get the matrix representation of the current shape
    const std::vector<std::vector<int>>& shapeMatrix = currentShape.getShapeMatrix();
    int shapeHeight = shapeMatrix.size();
    int shapeWidth = shapeMatrix[0].size();

    // Get the current position of the shape on the board
    int shapeX = currentShape.getXPos();
    int shapeY = currentShape.getYPos();

    // Iterate through each block in the shape matrix
    for (int row = 0; row < shapeHeight; ++row) {
        for (int col = 0; col < shapeWidth; ++col) {
            // Only check cells that are part of the shape
            if (shapeMatrix[row][col] != 0) {
                int boardX = shapeX + col;
                int boardY = shapeY + row;

                // Check if there's a block below the current block on the board
                if (grid[boardY + 1][boardX] != 0) {
                    return true; // There's a block directly below
                }
            }
        }
    }

    // No blocks below the current piece
    return false;
}

int Board::getLinesCleared()
{
    return linesCleared;
}

bool Board::doesPieceFit(const Shape& currentShape, int targetX, int targetY) const
{
    // Get the matrix representation of the current shape
    const std::vector<std::vector<int>>& shapeMatrix = currentShape.getShapeMatrix();
    int shapeHeight = shapeMatrix.size();
    int shapeWidth = shapeMatrix[0].size();

    // Iterate through each block in the shape matrix
    for (int row = 0; row < shapeHeight; ++row) {
        for (int col = 0; col < shapeWidth; ++col) {
            // Only check cells that are part of the shape
            if (shapeMatrix[row][col] != 0) {
                int boardX = targetX + col;
                int boardY = targetY + row;

                // Check if the piece is out of bounds
                if (boardX < 0 || boardX >= grid[0].size() || boardY < 0 || boardY >= grid.size()) {
                    return false; // The piece is out of bounds
                }

                // Check if the position on the board is already occupied
                if (grid[boardY][boardX] != 0) {
                    return false; // The piece collides with another piece
                }
            }
        }
    }

    // No collision, the piece fits
    return true;
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
