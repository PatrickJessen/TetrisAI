#pragma once
#include "Shape.h"

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

    void printBoard();
public:
    Shape* getNextShape();
    int getWidth() const;
    int getHeight() const;
    const std::vector<std::vector<int>>& getGrid();

private:
    void generateNextShape();
    bool isPartOfCurrentShape(int boardX, int boardY, const Shape& shape) const;
private:
    int width;
    int height;
    std::vector<std::vector<int>> grid; // 2D grid representing the board
    Shape* nextShape;
    int score;
    int linesCleared;
};