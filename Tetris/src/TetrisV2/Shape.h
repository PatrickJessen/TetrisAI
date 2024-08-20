#pragma once
#include <vector>

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPACE,
};

enum class ShapeType
{
    I,
    O,
    T,
    L,
    J,
    S,
    Z
};

class Shape
{
public:
    Shape(ShapeType type);
	Shape() = default;
public:
	void move(int x, int y);
    void rotate(int direction);
    void moveLeft();
    void moveRight();
    void moveDown();
    void drop();
	void setPos(int x, int y);

public:
    const std::vector<std::vector<int>> getShapeMatrix() const;
	int getXPos() const;
	int getYPos() const;

private:
    std::vector<std::vector<int>> matrix; // 2D matrix representing the shape
    int posX = 4;
    int posY = 0;
	int rotation = 0;
    ShapeType type = ShapeType::I; // Enum representing the type (I, O, T, etc.)
	std::vector<std::vector<std::vector<int>>> shapes{
		{ // I
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{1, 1, 1, 1}
		},

		{ // O
			{1, 1},
			{1, 1}
		},

		{ // T
			{0, 0, 0},
			{0, 1, 0},
			{1, 1, 1}
		},

		{ // L
			{0, 0, 0},
			{0, 0, 1},
			{1, 1, 1}
		},

		{ // J
			{0, 0, 0},
			{1, 0, 0},
			{1, 1, 1}
		},

		{ // S
			{0, 0, 0},
			{0, 1, 1},
			{1, 1, 0}
		},

		{ // Z
			{0, 0, 0},
			{1, 1, 0},
			{0, 1, 1}
		}
	};
};