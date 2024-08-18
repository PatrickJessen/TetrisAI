#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include "Tetris/Board.h"

// Hash function for TetrisState
struct StateHash {
    std::size_t operator()(const TetrisState& state) const {
        std::size_t hash = 0;
        for (const auto& val : state.grid) {
            hash_combine(hash, val);
        }
        hash_combine(hash, std::hash<int>{}(state.currentPiece));
        hash_combine(hash, std::hash<int>{}(state.rotationState));
        for (const auto& height : state.columnHeights) {
            hash_combine(hash, height);
        }
        hash_combine(hash, std::hash<int>{}(state.maxHeight));
        hash_combine(hash, std::hash<int>{}(state.holes));
        hash_combine(hash, std::hash<int>{}(state.linesCleared));
        return hash;
    }

    inline void hash_combine(std::size_t& seed, const int& v) const {
        seed ^= std::hash<int>{}(v)+0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

template<typename T>
class QLearningAgent {
public:
    QLearningAgent(int size, double learningRate, double discountFactor, double epsilon)
        : size(size), learningRate(learningRate), discountFactor(discountFactor),
        epsilon(epsilon), rng(std::random_device{}()), dist(0.0, 1.0), dist_action(0, 3) {}

    T chooseAction(const TetrisState& state) {
        if (dist(rng) < epsilon) {
            return static_cast<T>(dist_action(rng));
        }
        else {
            const auto& qValues = qTable[stateToIndex(state)];
            return static_cast<T>(std::distance(qValues.begin(),
                std::max_element(qValues.begin(), qValues.end())));
        }
    }

    void updateQValue(const TetrisState& state, T action, double reward, const TetrisState& newState) {
        epsilon = std::max(minEpsilon, epsilon - epsilonDecay);

        int stateIndex = stateToIndex(state);
        int newStateIndex = stateToIndex(newState);

        double oldQValue = qTable[stateIndex][static_cast<int>(action)];
        double maxNextQValue = (newStateIndex != -1) ?
            *std::max_element(qTable[newStateIndex].begin(), qTable[newStateIndex].end()) : 0.0;
        qTable[stateIndex][static_cast<int>(action)] = oldQValue +
            learningRate * (reward + discountFactor * maxNextQValue - oldQValue);
    }

    double getQValue(const TetrisState& state, int action) const {
        return qTable[stateToIndex(state)][action];
    }

    void linearDecay(double initialEpsilon, double minEpsilon, double decayRate, int episode) {
        epsilon = std::max(initialEpsilon - decayRate * episode, minEpsilon);
    }

    void setEpsilon(double val) { epsilon = val; }
    void setLearningRate(double val) { learningRate = val; }

private:
    int stateToIndex(const TetrisState& state) {
        auto it = stateIndexMap.find(state);
        if (it != stateIndexMap.end()) {
            return it->second;
        }
        else {
            int newIndex = stateIndexMap.size();
            stateIndexMap[state] = newIndex;
            if (newIndex >= qTable.size()) {
                qTable.emplace_back(5, 0.0); // Dynamically grow Q-table
            }
            return newIndex;
        }
    }

private:
    std::vector<std::vector<double>> qTable;
    double learningRate;
    double discountFactor;
    double epsilon;
    std::mt19937 rng;
    int size;
    std::uniform_real_distribution<double> dist;
    std::uniform_int_distribution<int> dist_action;
    std::unordered_map<TetrisState, int, StateHash> stateIndexMap;
    double minEpsilon = 0.1;
    double epsilonDecay = 0.000001;
};