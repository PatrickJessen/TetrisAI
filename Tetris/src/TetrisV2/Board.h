#pragma once
#include "Shape.h"
#include "TetrisState.h"

class Board
{
public:
    Board(int width = 10, int height = 20);
public:
    void initializeBoard();
    void insertShapeToBoard(const Shape& shape);
    int clearLines();
    bool checkGameOver(const Shape& shape);
    void reset();
    bool hasCollision(const Shape& shape) const;
    void clearShapeFromBoard(const Shape& shape);
    void updateBoard(const Shape& shape);
    void setGameOver(const bool value);
    void printBoard();
public:
    Shape* getNextShape();
    int getWidth() const;
    int getHeight() const;
    const std::vector<std::vector<int>>& getGrid();
    const std::vector<std::vector<int>>& getPreviousGrid();
    TetrisState extractState() const;
    int calculateBumpiness(const std::vector<std::vector<int>>& grid) const;
    int calculateAggregateHeight(const std::vector<std::vector<int>>& grid) const;
    bool isPieceOnSolidGround(const Shape& currentShape) const;
    const bool isGameOver();
    int getLinesCleared();
    bool doesPieceFit(const Shape& currentShape, int targetX, int targetY) const;

private:
    void generateNextShape();
    bool isPartOfCurrentShape(int boardX, int boardY, const Shape& shape) const;
private:
    int width;
    int height;
    std::vector<std::vector<int>> grid; // 2D grid representing the board
    std::vector<std::vector<int>> previousGrid; // 2D grid representing the board
    Shape* nextShape;
    Shape* currentShape;
    int score;
    int linesCleared;
    bool gameOver;
};