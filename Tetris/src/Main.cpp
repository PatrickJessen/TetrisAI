#include <iostream>
#include "Tetris/Board.h"
#include "SDL/Window/WindowsWindow.h"
#include "Draw.h"
#include "SDLController.h"
#include "QLearningAgent.h"


int main() {
    Window* window = WindowsWindow::Create({ "Tetris" });
    Board* board = new Board(10, 20, 24);
    Draw* draw = new Draw(board);
    SDLController controller(new Controller(board));

    const int FPS = 80;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    double discountFactor = 1.0;
    double epsilon = 0.3;
    double learningFactor = 0.001;

    QLearningAgent<Direction>* agent = new QLearningAgent<Direction>(board->getWidth() * board->getHeight(), learningFactor, discountFactor, epsilon);
    bool useAi = true;
    while (true)
    {
        TetrisState state{};
        /*frameStart = SDL_GetTicks();
        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }*/
        TetrisState currentState{};
        while (!board->getIsGameOver()) {
            Direction action = agent->chooseAction(currentState);
            controller.Update(action, currentState);
            if (controller.getController()->canUpdateRewards()) {
                currentState = board->getCurrentState();
                double reward = controller.getController()->getReward(currentState);
                if (reward > 0) {
                    std::cout << "Rewards: " << reward << "\n";
                }
                TetrisState newState = board->getCurrentState();
                agent->updateQValue(state, action, reward, newState);
                state = newState;
            }
            draw->drawBoard();
            window->OnUpdate();
            window->Clear();
        }
    }
    window->Clean();

    return 0;
}
