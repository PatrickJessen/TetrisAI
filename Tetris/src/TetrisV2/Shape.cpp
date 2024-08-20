#include "Shape.h"

Shape::Shape(ShapeType type)
	:type(type)
{
	matrix = shapes[(int)type];
}

void Shape::move(int x, int y)
{
	posX += x;
	posY += y;
}

void Shape::rotate(int direction)
{
	int n = matrix.size();
	    std::vector<std::vector<int>> rotated(n, std::vector<int>(n));
	
	    for (int i = 0; i < n; ++i) {
	        for (int j = 0; j < n; ++j) {
	            rotated[j][n - direction - i] = matrix[i][j];
	        }
	    }
	    rotation += 90;
	    if (rotation >= 270) {
			rotation = 0;
	    }
		matrix = rotated;
}

void Shape::moveLeft()
{
	posX--;
}

void Shape::moveRight()
{
	posX++;
}

void Shape::moveDown()
{
	posY++;
}

void Shape::drop()
{
	posY++;
}

void Shape::setPos(int x, int y)
{
	posX = x;
	posY = y;
}

const std::vector<std::vector<int>> Shape::getShapeMatrix() const
{
	return matrix;
}

int Shape::getXPos() const
{
	return posX;
}

int Shape::getYPos() const
{
	return posY;
}
