#pragma once
#include "Board.h"

class Controller
{
public:
    Controller(Board* board);
public:
    void moveShape(Direction dir);
    void rotateShape();
    void dropShape();
    void handleInput();

private:
    bool tryWallKick(int dx, int dy);

private:
    Board* board = nullptr;
    Shape* currentShape = nullptr;

    int currentIterations = 0;
    int maxIterations = 20;
};