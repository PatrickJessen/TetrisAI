#include "Controller.h"

Controller::Controller(Board* board)
	:board(board)
{
	currentShape = board->getNextShape();
}

void Controller::moveShape(Direction dir)
{
    int xOffset = 0;
    int yOffset = 0;

    // Clear the shape's current position
    board->clearShapeFromBoard(*currentShape);
    switch (dir) {
    case Direction::LEFT:
        xOffset = -1;
        break;
    case Direction::RIGHT:
        xOffset = 1;
        break;
    case Direction::DOWN:
        if (currentIterations > maxIterations) {
            yOffset = 1;
            currentIterations = 0;
        }
        break;
    case Direction::UP:
        rotateShape();
        break;
    case Direction::SPACE:
        dropShape();
        break;
    default:
        break;
    }


    // Move the shape
    currentShape->move(xOffset, yOffset);

    if (board->hasCollision(*currentShape)) {
        // Undo the move if there's a collision
        currentShape->move(-xOffset, -yOffset);
    }

    // Insert the shape at its new position
    board->insertShapeToBoard(*currentShape);
    currentIterations++;
}

void Controller::rotateShape()
{
    // Save the current position in case we need to revert
    auto originalXPos = currentShape->getXPos();
    auto originalYPos = currentShape->getYPos();

    // Attempt to rotate the shape
    currentShape->rotate(1);

    // Check for collision or out-of-bounds after rotation
    if (board->hasCollision(*currentShape)) {
        // Try wall kick: move left, right, or up
        if (!tryWallKick(-1, 0) &&  // Move left
            !tryWallKick(1, 0) &&   // Move right
            !tryWallKick(0, -1)) {  // Move up
            // If all kicks fail, revert the rotation
            currentShape->rotate(-1); // Rotate back to original
            currentShape->setPos(originalXPos, originalYPos); // Revert position
        }
    }

    // If no collision, update the board with the rotated shape
    board->insertShapeToBoard(*currentShape);
}

void Controller::dropShape()
{
    // Logic to drop the current shape
    while (!board->hasCollision(*currentShape)) {
        currentShape->move(0, 1);
    }

    // Move back up one step to the last valid position
    currentShape->move(0, -1);

    // Place the shape in the board when it hits the bottom
    board->insertShapeToBoard(*currentShape);
    board->clearLines();
    currentShape = board->getNextShape(); // Get the next shape
}

void Controller::handleInput()
{
}

bool Controller::tryWallKick(int dx, int dy)
{
    // Move the shape by the given offset
    currentShape->move(dx, dy);

    // Check for collision or out-of-bounds
    if (!board->hasCollision(*currentShape)) {
        return true; // The wall kick succeeded
    }

    // If it still collides, revert the move
    currentShape->move(-dx, -dy);
    return false; // The wall kick failed
}
