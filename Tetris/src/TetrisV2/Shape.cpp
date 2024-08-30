#include "Shape.h"
#include <algorithm>

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
            if (direction > 0) {
                // Clockwise rotation
                rotated[j][n - 1 - i] = matrix[i][j];
            }
            else {
                // Counterclockwise rotation
                rotated[n - 1 - j][i] = matrix[i][j];
            }
        }
    }

    // Update the rotation angle (assuming direction = 1 for 90 degrees clockwise, -1 for 90 degrees counterclockwise)
    rotation += direction;
    if (rotation > 3) {
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

int Shape::getRotationState() const
{
    return rotation;
}

ShapeType Shape::getShapeType()
{
    return type;
}

void Shape::setYPos(const int& value)
{
    posY = value;
}

std::pair<int, int> Shape::getCurrentDimensions() const
{
    int height = matrix.size(); // Number of rows
    int width = 0; // Initialize width

    // Calculate width (maximum number of filled cells in any row)
    for (const auto& row : matrix) {
        int rowWidth = std::count_if(row.begin(), row.end(), [](int cell) { return cell != 0; });
        if (rowWidth > width) {
            width = rowWidth;
        }
    }

    // Calculate height (count number of non-empty rows)
    height = std::count_if(matrix.begin(), matrix.end(), [](const std::vector<int>& row) {
        return std::any_of(row.begin(), row.end(), [](int cell) { return cell != 0; });
        });

    return { width, height };
}
