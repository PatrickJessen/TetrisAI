#include "Shape.h"

Shape::Shape(std::vector<std::vector<int>> shape, int x, int y, int type)
	:shape(shape), x(x), y(y), type(type)
{
    rotate();
}

void Shape::rotate()
{
    int n = shape.size();
    std::vector<std::vector<int>> rotated(n, std::vector<int>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            rotated[j][n - 1 - i] = shape[i][j];
        }
    }
    rotationState += 90;
    if (rotationState >= 270) {
        rotationState = 0;
    }
    shape = rotated;
}

std::vector<std::vector<int>> Shape::getShape()
{
	return shape;
}

const int& Shape::getXPos() const
{
    return x;
}

const int& Shape::getYPos() const
{
    return y;
}

const int& Shape::getGhostYPos() const
{
    return ghostY;
}

void Shape::setPos(const int& x, const int& y)
{
    this->x += x;
    this->y += y;
}

void Shape::setGhostYPos(const int& y)
{
    ghostY += y;
}

void Shape::setShape(const std::vector<std::vector<int>>& value)
{
    shape = value;
}

void Shape::resetGhostPos()
{
    ghostY = y;
}

int Shape::getType()
{
    return type;
}

int Shape::getRotationState()
{
    return rotationState;
}
