#include "Game.h"
#include "../SDL/Window/WindowsWindow.h"

void Game::update() {
    //Window* window = WindowsWindow::Create({ "Tetris" });
    Board* board = new Board();
    SDLController controller(new Controller(board));
    //Draw draw = Draw(board);

    // Hyperparameters
    double alpha = 0.001;
    double gamma = 0.95;
    double epsilon = 1.0;
    double minEpsilon = 0.01;
    double epsilonDecay = 0.99999;
    std::string bestQTablePath = "best_qtable.dat"; // Save the best Q-table here

    // Create the agent
    QLearningAgent* agent = new QLearningAgent(alpha, gamma, epsilon, minEpsilon, epsilonDecay, bestQTablePath);

    // Performance tracking
    int currentPerformance = 0;
    int bestPerformance = 0;

    const int FPS = 10;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;
    bool slowDown = true;
    bool useUI = true;
    int resets = 0;
    std::cout <<"Running...\n";
    int currentGen = 1;
    int bestGen = 1;

    while (true) {
        board->reset();
        TetrisState state = board->extractState();

        while (!board->isGameOver()) {
            if (controller.getController()->getNumOfEpisodes() >= 50000) {
                std::cout << "Current gen: " << currentGen << "\n";
                resets++;

                // Evaluate performance at the end of an episode
                if (currentPerformance > bestPerformance) {
                    bestGen = currentGen;
                    bestPerformance = currentPerformance;
                    std::string newBest = "best_qtable_"; // Save the best Q-table here
                    newBest.append(std::to_string(currentGen));
                    newBest.append(".dat");
                    agent->saveQTable(newBest);
                    std::cout << "New best performance: " << bestPerformance << ". Q-table saved.\n";
                }
                // Reset for the next generation
                currentPerformance = 0.0;
                agent->setEpsilon(epsilon);
                controller.getController()->setNumOfEpisodes(0);
                controller.getController()->setTotalLinesCleread(0);
                currentGen++;
            }

            //if (Input::KeyPressed(Key::S)) {
            //    agent->saveQTable(bestQTablePath);
            //    std::cout << "Saved learning\n";
            //}
            //if (Input::KeyPressed(Key::L)) {
            //    slowDown = !slowDown;
            //}
            //if (Input::KeyPressed(Key::K)) {
            //    useUI = !useUI;
            //}
            //if (slowDown) {
            //    frameStart = SDL_GetTicks();
            //    frameTime = SDL_GetTicks() - frameStart;

            //    if (frameDelay > frameTime) {
            //        SDL_Delay(frameDelay - frameTime);
            //    }
            //}

            // Agent action and update
            Action action = agent->chooseAction(state);
            controller.Update(action);
            if (controller.getController()->getDropped()) {
                double reward = controller.getController()->calculateReward(state);
                TetrisState nextState = board->extractState();
                agent->update(state, action, reward, nextState);
                currentPerformance += nextState.lines_cleared; // Track the cumulative performance
                state = nextState;
            }

            /*if (useUI) {
                draw.drawBoard();
            }

            window->OnUpdate();
            window->Clear()*/;
        }
    }

    //window->Clean();
}
