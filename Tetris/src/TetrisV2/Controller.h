#pragma once
#include "Board.h"
#include "../QLearningAgent.h"

class Controller
{
public:
    Controller(Board* board);
public:
    void moveShape(Action dir);
    void runEpisode(Action action);
    void rotateShape();
    void dropShape();
    void handleInput();
    double calculateReward(const TetrisState& currentState);
    int getNumOfEpisodes();
    void setNumOfEpisodes(const int& value);
    int getTotalLinesCleared();
    void setTotalLinesCleread(const int& value);
    bool getDropped();
private:
    bool tryWallKick(int dx, int dy);
    void logPerformance(const TetrisState& state, double reward);
private:
    Board* board = nullptr;
    Shape* currentShape = nullptr;

    int currentIterations = 0;
    int maxIterations = 1;
    TetrisState previousState;
    TetrisState cuState;
    int runsOfEpisodes = 0;
    int totalLinesCleared = 0;
    bool dropped = false;

    std::vector<std::pair<int, int>> wallKickOffsets = {
    {0, 0},   // No kick
    {-1, 0},  // Kick left
    {1, 0},   // Kick right
    {0, -1},  // Kick up (to prevent ground collision)
    {-1, -1}, // Kick left and up
    {1, -1},  // Kick right and up
    {-2, 0},  // Kick further left
    {2, 0},   // Kick further right
    };
};