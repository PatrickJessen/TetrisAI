#include <unordered_map>
#include <vector>
#include <random>
#include <algorithm>
#include <deque>
#include <fstream>
#include <array>
#include <iostream>
#include "TetrisV2/TetrisState.h"
#include "TetrisV2/Shape.h"

class QLearningAgent {
public:
    QLearningAgent(double alpha, double gamma, double epsilon, double minEpsilon, double epsilonDecay, const std::string& path, int replayBufferSize = 10000)
        : alpha(alpha), gamma(gamma), epsilon(epsilon), minEpsilon(minEpsilon), epsilonDecay(epsilonDecay), initialEpsilon(epsilon), generator(rd()), replayBufferSize(replayBufferSize) {
        loadQTable(path);
    }

    // Get the best action for the current state
    Action getBestAction(const TetrisState& state) {
        auto it = qTable.find(state);
        if (it != qTable.end()) {
            return static_cast<Action>(std::distance(it->second.begin(), std::max_element(it->second.begin(), it->second.end())));
        }
        else {
            initializeState(state);
            return static_cast<Action>(std::uniform_int_distribution<int>(0, 4)(generator));
        }
    }

    // Choose an action based on epsilon-greedy strategy
    Action chooseAction(const TetrisState& state) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        if (dist(generator) < epsilon) {
            std::uniform_int_distribution<int> action_dist(0, 4);
            return static_cast<Action>(action_dist(generator));
        }
        else {
            return getBestAction(state);
        }
    }

    // Update Q-values based on the state, action, reward, and next state
    void update(const TetrisState& state, Action action, double reward, const TetrisState& nextState) {
        double maxQ = 0.0;
        auto it = qTable.find(nextState);
        if (it != qTable.end()) {
            maxQ = *std::max_element(it->second.begin(), it->second.end());
        }
        else {
            initializeState(nextState);
        }

        double& q = qTable[state][static_cast<int>(action)];
        q = q + alpha * (reward + gamma * maxQ - q);

        // Store the experience in the replay buffer
        if (replayBuffer.size() >= replayBufferSize) {
            replayBuffer.pop_front();
        }
        replayBuffer.push_back({ state, action, reward, nextState });

        // Update using a sample from the replay buffer
        if (replayBuffer.size() >= replaySampleSize) {
            for (int i = 0; i < replaySampleSize; ++i) {
                replayExperience();
            }
        }

        // Decay epsilon
        epsilon = std::max(minEpsilon, epsilon * 0.999999);  // Slow down decay
    }

    // Reset Q-values (optional)
    void reset() {
        qTable.clear();
        replayBuffer.clear();
        epsilon = initialEpsilon;
    }

    // Function to save the Q-table to a file
    void saveQTable(const std::string& filename) {
        std::ofstream outFile(filename, std::ios::binary);
        if (outFile.is_open()) {
            for (const auto& entry : qTable) {
                const TetrisState& state = entry.first;
                const std::array<double, 5>& qValues = entry.second;

                // Save the state
                saveState(outFile, state);

                // Save the Q-values
                outFile.write(reinterpret_cast<const char*>(qValues.data()), qValues.size() * sizeof(double));
            }
            outFile.close();
        }
        else {
            std::cout << "Error opening file for saving Q-table." << std::endl;
        }
    }

    // Function to load the Q-table from a file
    void loadQTable(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (inFile.is_open()) {
            qTable.clear(); // Clear the current Q-table
            while (inFile.peek() != EOF) {
                TetrisState state;
                std::array<double, 5> qValues;

                loadState(inFile, state);

                if (!inFile.read(reinterpret_cast<char*>(qValues.data()), qValues.size() * sizeof(double))) {
                    std::cerr << "Error reading Q-values from file." << std::endl;
                    break;
                }

                qTable[state] = qValues;
            }
            inFile.close();
            std::cout << "Sucessfully loaded Q-table." << std::endl;
        }
        else {
            std::cout << "Error opening file for loading Q-table." << std::endl;
        }
    }

    void setEpsilon(const double& value) {
        epsilon = value;
    }

private:
    double alpha;           // Learning rate
    double gamma;           // Discount factor
    double epsilon;         // Exploration rate
    double minEpsilon;      // Minimum exploration rate
    double epsilonDecay;    // Decay rate for epsilon
    double initialEpsilon;  // Initial epsilon value for reset

    int replayBufferSize;   // Size of the experience replay buffer
    int replaySampleSize = 32; // Number of samples to replay per update

    std::unordered_map<TetrisState, std::array<double, 5>, TetrisStateHash> qTable;
    std::deque<std::tuple<TetrisState, Action, double, TetrisState>> replayBuffer;

    std::random_device rd;
    std::mt19937 generator;

    // Initialize Q-values for an unseen state
    void initializeState(const TetrisState& state) {
        if (qTable.find(state) == qTable.end()) {
            TetrisState closestState;
            double minDistance = std::numeric_limits<double>::max();

            for (const auto& entry : qTable) {
                double distance = calculateDistance(state, entry.first);
                if (distance < minDistance) {
                    closestState = entry.first;
                    minDistance = distance;
                }
            }

            // Define a threshold for "closeness"
            double threshold = 5.0; // Example threshold, adjust based on your application's needs

            if (minDistance < threshold) {
                qTable[state] = qTable[closestState];  // Use Q-values of the closest state
            }
            else {
                qTable[state] = { 0.3, 0.3, 0.3, 0.3, 0.3 };  // Neutral starting point
            }
        }
    }

    double calculateDistance(const TetrisState& state1, const TetrisState& state2) {
        double distance = 0.0;

        // Calculate Manhattan distance using the most impactful features
        distance += std::abs(state1.aggregate_height - state2.aggregate_height);
        distance += std::abs(state1.holes - state2.holes);
        distance += std::abs(state1.bumpiness - state2.bumpiness);
        distance += std::abs(state1.lines_cleared - state2.lines_cleared);

        // Optionally include row and column transitions if they are part of the state
        distance += std::abs(state1.row_transitions - state2.row_transitions);
        distance += std::abs(state1.column_transitions - state2.column_transitions);

        return distance;
    }

    void replayExperience() {
        if (replayBuffer.empty()) return;

        std::uniform_int_distribution<int> dist(0, replayBuffer.size() - 1);
        auto& experience = replayBuffer[dist(generator)];

        TetrisState state = std::get<0>(experience);
        Action action = std::get<1>(experience);
        double reward = std::get<2>(experience);
        TetrisState nextState = std::get<3>(experience);

        double maxQ = 0.0;
        auto it = qTable.find(nextState);
        if (it != qTable.end()) {
            maxQ = *std::max_element(it->second.begin(), it->second.end());
        }

        double& q = qTable[state][static_cast<int>(action)];
        q = q + alpha * (reward + gamma * maxQ - q);
    }

    // Function to save the TetrisState to a file
    void saveState(std::ofstream& outFile, const TetrisState& state) {
        outFile.write(reinterpret_cast<const char*>(&state.aggregate_height), sizeof(state.aggregate_height));
        outFile.write(reinterpret_cast<const char*>(&state.holes), sizeof(state.holes));
        outFile.write(reinterpret_cast<const char*>(&state.bumpiness), sizeof(state.bumpiness));
        outFile.write(reinterpret_cast<const char*>(&state.lines_cleared), sizeof(state.lines_cleared));
        outFile.write(reinterpret_cast<const char*>(&state.row_transitions), sizeof(state.row_transitions));
        outFile.write(reinterpret_cast<const char*>(&state.column_transitions), sizeof(state.column_transitions));
    }

    void loadState(std::ifstream& inFile, TetrisState& state) {
        inFile.read(reinterpret_cast<char*>(&state.aggregate_height), sizeof(state.aggregate_height));
        inFile.read(reinterpret_cast<char*>(&state.holes), sizeof(state.holes));
        inFile.read(reinterpret_cast<char*>(&state.bumpiness), sizeof(state.bumpiness));
        inFile.read(reinterpret_cast<char*>(&state.lines_cleared), sizeof(state.lines_cleared));
        inFile.read(reinterpret_cast<char*>(&state.row_transitions), sizeof(state.row_transitions));
        inFile.read(reinterpret_cast<char*>(&state.column_transitions), sizeof(state.column_transitions));
    }

};
