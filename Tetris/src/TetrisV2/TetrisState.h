#pragma once
#include <vector>

struct TetrisState {
    int aggregate_height;       // Sum of the heights of all columns
    int holes;                  // Total number of holes
    int bumpiness;              // Sum of the absolute differences in height between adjacent columns
    int lines_cleared;          // Number of lines cleared by the last move
    int row_transitions;        // Number of row transitions (optional, could be removed)
    int column_transitions;     // Number of column transitions (optional, could be removed)

    bool operator==(const TetrisState& other) const {
        return aggregate_height == other.aggregate_height &&
            holes == other.holes &&
            bumpiness == other.bumpiness &&
            lines_cleared == other.lines_cleared &&
            row_transitions == other.row_transitions &&
            column_transitions == other.column_transitions;
    }
};

struct TetrisStateHash {
    std::size_t operator()(const TetrisState& state) const {
        std::size_t seed = 0;
        seed ^= std::hash<int>{}(state.aggregate_height) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>{}(state.holes) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>{}(state.bumpiness) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>{}(state.lines_cleared) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>{}(state.row_transitions) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<int>{}(state.column_transitions) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
