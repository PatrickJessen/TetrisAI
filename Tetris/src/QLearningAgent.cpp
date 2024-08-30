//#include "QLearningAgent.h"
//#include <iostream>
//#include <numeric>
//
//QLearningAgent::QLearningAgent(double alpha, double gamma, double epsilon, double epsilon_decay)
//	: alpha(alpha), gamma(gamma), epsilon(epsilon), epsilon_decay(epsilon_decay) 
//{
//}
//
//Action QLearningAgent::chooseAction(const TetrisState& state)
//{
//    if (epsilon < 0) {
//        epsilon = 0.0;
//    }
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<> dis(0.0, 1.0);
//
//    initializeActions(state);
//    const auto& actions = qTable[state];
//
//    // Calculate the sum of exponentials of Q-values
//    double sumExpQ = std::accumulate(actions.begin(), actions.end(), 0.0,
//        [](double sum, const auto& actionPair) {
//            return sum + std::exp(actionPair.second);
//        });
//
//    // Select action based on softmax probabilities
//    double randVal = std::uniform_real_distribution<>(0, sumExpQ)(gen);
//    double cumulative = 0.0;
//    for (const auto& actionPair : actions) {
//        cumulative += std::exp(actionPair.second);
//        if (randVal <= cumulative) {
//            return actionPair.first;
//        }
//    }
//
//    // Fallback (shouldn't happen in theory)
//    return actions.begin()->first;
//}
//
//void QLearningAgent::updateQValue(const TetrisState& previousState, Action action, const TetrisState& newState, double reward)
//{
//    double currentQValue = getQValue(previousState, action);
//    double maxFutureQValue = getMaxQValue(newState);
//
//    qTable[previousState][action] = currentQValue + alpha * (reward + gamma * maxFutureQValue - currentQValue);
//}
//
//void QLearningAgent::decayEpsilon()
//{
//    if (epsilon > 0.01) {
//        epsilon *= epsilon_decay;
//    }
//}
//
//std::vector<Action> QLearningAgent::getPossibleActions() const
//{
//    return { Action::LEFT, Action::RIGHT, Action::UP, Action::DOWN, Action::SPACE };
//}
//
//double QLearningAgent::getMaxQValue(const TetrisState& state)
//{
//    initializeActions(state); // Ensure actions are initialized
//    const auto& actions = qTable[state];
//    return std::max_element(actions.begin(), actions.end(),
//        [](const auto& a, const auto& b) { return a.second < b.second; })->second;
//}
//
//void QLearningAgent::initializeActions(const TetrisState& state)
//{
//    if (qTable.find(state) == qTable.end()) {
//        std::unordered_map<Action, double> actions;
//        for (const auto& action : getPossibleActions()) {
//            actions[action] = 0.0; // Initialize Q-value to 0.0 for all actions
//        }
//        qTable[state] = actions;
//    }
//}
//
//double QLearningAgent::getQValue(const TetrisState& state, Action action)
//{
//    initializeActions(state); // Ensure actions are initialized
//    return qTable[state][action];
//}
