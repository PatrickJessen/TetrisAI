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
        std::hash<int> int_hash;
        std::hash<float> float_hash;

        // Combine hashes of the heights
        for (int h : state.heights) {
            hash ^= int_hash(h) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }

        // Combine hashes of the height_diffs
        for (int diff : state.height_diffs) {
            hash ^= int_hash(diff) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }

        // Combine hashes of the other attributes
        hash ^= int_hash(state.num_holes) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= int_hash(state.max_height) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= int_hash(state.bumpiness) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= int_hash(state.lines_cleared) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= int_hash(state.landing_height) + 0x9e3779b9 + (hash << 6) + (hash >> 2);

        return hash;
    }
};

class QLearning {
private:
    double alpha; // Learning rate
    double gamma; // Discount factor
    double epsilon; // Exploration rate
    std::unordered_map<TetrisState, std::vector<double>, StateHash> q_table; // Q-table
    std::mt19937 rng; // Random number generator for epsilon-greedy

public:
    QLearning(double alpha, double gamma, double epsilon)
        : alpha(alpha), gamma(gamma), epsilon(epsilon), rng(std::random_device{}()) {}

    // Get the Q-values for a given state
    std::vector<double>& get_q_values(const TetrisState& state) {
        if (q_table.find(state) == q_table.end()) {
            // Initialize Q-values for all possible actions to 0.0
            q_table[state] = std::vector<double>(get_number_of_actions(), 0.0);
        }
        return q_table[state];
    }

    // Select an action using epsilon-greedy strategy
    int select_action(const TetrisState& state) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        if (dist(rng) < epsilon) {
            // Exploration: choose a random action
            std::uniform_int_distribution<int> action_dist(0, get_number_of_actions() - 1);
            return action_dist(rng);
        }
        else {
            // Exploitation: choose the action with the highest Q-value
            std::vector<double>& q_values = get_q_values(state);
            return std::distance(q_values.begin(), std::max_element(q_values.begin(), q_values.end()));
        }
    }

    // Update Q-value using the Q-learning update rule
    void update_q_value(const TetrisState& state, int action, double reward, const TetrisState& next_state) {
        std::vector<double>& q_values = get_q_values(state);
        double max_next_q_value = *std::max_element(get_q_values(next_state).begin(), get_q_values(next_state).end());
        q_values[action] += alpha * (reward + gamma * max_next_q_value - q_values[action]);
    }

    // To be implemented: a function that returns the number of possible actions
    int get_number_of_actions() const {
        // Return the number of possible actions (rotations and translations) for a Tetris piece
        return 4; // Placeholder, depends on Tetris implementation
    }

    // Save the Q-table to a file (optional)
    void save_q_table(const std::string& filename) {
        // Implement saving logic
    }

    // Load the Q-table from a file (optional)
    void load_q_table(const std::string& filename) {
        // Implement loading logic
    }
};