#include "Controller.h"
#include <iostream>
#include <fstream>

Controller::Controller(Board* board)
	:board(board)
{
	currentShape = board->getNextShape();
}

void Controller::moveShape(Action dir)
{
    if (!board->isGameOver()) {

        int xOffset = 0;
        int yOffset = 0;

        // Clear the shape's current position
        board->clearShapeFromBoard(*currentShape);
        if (currentIterations > maxIterations) {
            yOffset = 1;
            currentIterations = 0;
        }

        // Automatic downward movement after a certain number of iterations

        // Determine the movement based on the action
        switch (dir) {
        case Action::LEFT:
            xOffset = -1;
            break;
        case Action::RIGHT:
            xOffset = 1;
            break;
        case Action::DOWN:
            yOffset = 1;
            break;
        case Action::UP:
            rotateShape();
            break;
        case Action::SPACE:
            //dropShape();
            return; // Drop shape already handles placement and next shape, so return here
        default:
            break;
        }

        // Move the shape
        currentShape->move(xOffset, yOffset);

        // Check for collision after the move
        if (board->hasCollision(*currentShape)) {
            // Undo the move if there's a collision
            currentShape->move(-xOffset, -yOffset);

            // If the collision is vertical (hitting the ground or another piece from above)
            if (yOffset > 0) {
                //dropShape();

                // Check if the new shape collides immediately, which would mean game over
                if (board->hasCollision(*currentShape)) {
                    board->setGameOver(true);
                }

                return; // Stop further movement since the shape has been placed
            }
        }

        // Insert the shape at its new position if no collision occurred
        board->insertShapeToBoard(*currentShape);
        currentIterations++;
    }
}

void Controller::runEpisode(Action action)
{
    if (!board->isGameOver()) {
        dropped = false;
        // Choose an action based on the current state
        int xOffset = 0;
        int yOffset = 0;
        if (currentIterations >= maxIterations) {
            currentIterations = 0;
            //previousState = board->extractState(action);
        }
        yOffset = 1;
        // Clear the shape's current position
        board->clearShapeFromBoard(*currentShape);
        switch (action) {
        case Action::LEFT:
            xOffset = -1;
            break;
        case Action::RIGHT:
            xOffset = 1;
            break;
        case Action::DOWN:
            yOffset = 1;
            break;
        case Action::UP:
        {
            rotateShape();
            break;
        }
        case Action::SPACE:
            dropShape();
            return;
            break;
        default:
            break;
        }


        currentShape->move(xOffset, yOffset);

        // Check for collision after the move
        if (board->hasCollision(*currentShape)) {
            // Undo the move if there's a collision
            currentShape->move(-xOffset, -yOffset);

            // If the collision is vertical (hitting the ground or another piece from above)
            if (yOffset > 0) {
                dropShape();

                // Check if the new shape collides immediately, which would mean game over
                if (board->hasCollision(*currentShape)) {
                    board->setGameOver(true);
                    //dropShape(agent, previousState, action);
                    board->insertShapeToBoard(*currentShape);
                }

                return; // Stop further movement since the shape has been placed
            }
        }
        // Insert the shape at its new position if no collision occurred
        board->insertShapeToBoard(*currentShape);
    }
    else {
        board->reset();  // Reset the game
        previousState = {};
    }
    // Decay exploration rate (epsilon)
    currentIterations++;
    runsOfEpisodes++;

}

void Controller::rotateShape()
{
    // Attempt to rotate the shape
    currentShape->rotate(1);

    // Check if rotation causes a collision or goes out of bounds
    if (board->hasCollision(*currentShape)) {
        bool kicked = false;
        for (const auto& offset : wallKickOffsets) {
            // Try each wall kick offset
            if (tryWallKick(offset.first, offset.second)) {
                kicked = true;
                break;
            }
        }

        // If all kicks failed, revert the rotation
        if (!kicked) {
            currentShape->rotate(-1);  // Rotate back to the original position
        }
    }

    // After rotation (and potential wall kicks), insert the shape back into the board
    //board->insertShapeToBoard(*currentShape);
}

void Controller::dropShape()
{
    while (!board->hasCollision(*currentShape)) {
        currentShape->move(0, 1);
    }
    dropped = true;
    currentShape->move(0, -1);  // Move back up to the last valid position
    board->insertShapeToBoard(*currentShape);
    board->clearLines();
    currentShape = board->getNextShape();
}

void Controller::handleInput()
{
}

double Controller::calculateReward(const TetrisState& currentState)
{
    double reward = 0;

    // Reward for clearing lines
    int linesCleared = currentState.lines_cleared;
    if (linesCleared > 0) {
        reward += 800 * linesCleared;  // Increase the reward for clearing lines significantly
        totalLinesCleared += linesCleared;
        std::cout << "Lines cleared: " << linesCleared << " Number of episodes: " << runsOfEpisodes << "\n";
    }

    // Penalty for increasing holes
    int holes = currentState.holes;
    reward -= holes * 5;  // Reduced penalty for holes to encourage line-clearing

    // Penalty for row transitions
    int rowTransitions = currentState.row_transitions;
    reward -= rowTransitions * 1;  // Slight penalty for row transitions

    // Penalty for column transitions
    int columnTransitions = currentState.column_transitions;
    reward -= columnTransitions * 1;  // Slight penalty for column transitions

    // Penalty for increasing bumpiness
    int bumpiness = currentState.bumpiness;
    reward -= bumpiness * 0.5;  // Reduced penalty for bumpiness

    // Penalty for increasing aggregate height
    int height = currentState.aggregate_height;
    reward -= height * 1.0;  // Reduced penalty for height

    // Large penalty if the game is over (max height reached)
    if (currentState.aggregate_height >= board->getHeight()) {
        reward -= 500;  // Maintain a significant penalty for reaching the top
    }

    //std::cout << "\n";
    //board->printBoard();
    //std::cout << "Reward: " << reward /*<< ", bumpinessIncrease: " << bumpinessIncrease << ", HolesCreated: " << holesCreated*/ << "\n";
    //std::cout << "Current State: Bumpiness: " << currentState.bumpiness << ", Num holes: " << currentState.num_holes << ", Landing Height: " << currentState.landing_height << ", Max Height: " << currentState.max_height << "\n";
    //std::cout << "Previous State: Bumpiness: " << previousState.bumpiness << ", Num holes: " << previousState.num_holes << ", Landing Height: " << previousState.landing_height << ", Max Height: " << previousState.max_height << "\n";

    return reward;
}

int Controller::getNumOfEpisodes()
{
    return runsOfEpisodes;
}

void Controller::setNumOfEpisodes(const int& value)
{
    runsOfEpisodes = value;
}

int Controller::getTotalLinesCleared()
{
    return totalLinesCleared;
}

void Controller::setTotalLinesCleread(const int& value)
{
    totalLinesCleared = value;
}

bool Controller::getDropped()
{
    return dropped;
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

void Controller::logPerformance(const TetrisState& state, double reward)
{
    std::ofstream logFile("agent_performance.log", std::ios_base::app);
    logFile << "Lines Cleared: " << state.lines_cleared
        //<< ", Total Holes: " << state.total_holes
        //<< ", Max-Min Height Diff: " << state.max_min_height_diff
        << ", Reward: " << reward << std::endl;
    logFile.close();

}
