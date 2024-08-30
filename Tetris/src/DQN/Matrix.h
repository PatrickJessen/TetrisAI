//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <cstdlib>
//#include <ctime>
//#include <algorithm>
//#include <numeric>
//
//// Matrix class with gradient computation
//class Matrix {
//public:
//    std::vector<std::vector<double>> data;
//
//    Matrix(int rows, int cols, bool randomize = true) {
//        data.resize(rows, std::vector<double>(cols, 0.0));
//        if (randomize) {
//            for (auto& row : data)
//                for (auto& val : row)
//                    val = (rand() / (double)RAND_MAX) * 2 - 1;
//        }
//    }
//
//    Matrix operator*(const Matrix& other) const {
//        if (data[0].size() != other.data.size()) {
//            throw std::invalid_argument("Matrix dimensions do not match for multiplication");
//        }
//
//        int rows = data.size();
//        int cols = other.data[0].size();
//        int common_dim = data[0].size();
//        Matrix result(rows, cols, false);
//
//        for (int i = 0; i < rows; ++i) {
//            for (int j = 0; j < cols; ++j) {
//                for (int k = 0; k < common_dim; ++k) {
//                    result.data[i][j] += data[i][k] * other.data[k][j];
//                }
//            }
//        }
//
//        return result;
//    }
//
//    Matrix operator+(const Matrix& other) const {
//        if (data.size() != other.data.size() || data[0].size() != other.data[0].size()) {
//            throw std::invalid_argument("Matrix dimensions do not match for addition");
//        }
//
//        int rows = data.size();
//        int cols = data[0].size();
//        Matrix result(rows, cols, false);
//
//        for (int i = 0; i < rows; ++i) {
//            for (int j = 0; j < cols; ++j) {
//                result.data[i][j] = data[i][j] + other.data[i][j];
//            }
//        }
//
//        return result;
//    }
//
//    Matrix operator-(const Matrix& other) const {
//        if (data.size() != other.data.size() || data[0].size() != other.data[0].size()) {
//            throw std::invalid_argument("Matrix dimensions do not match for subtraction");
//        }
//
//        int rows = data.size();
//        int cols = data[0].size();
//        Matrix result(rows, cols, false);
//
//        for (int i = 0; i < rows; ++i) {
//            for (int j = 0; j < cols; ++j) {
//                result.data[i][j] = data[i][j] - other.data[i][j];
//            }
//        }
//
//        return result;
//    }
//
//    Matrix transpose() const {
//        int rows = data[0].size();
//        int cols = data.size();
//        Matrix result(rows, cols, false);
//
//        for (int i = 0; i < rows; ++i) {
//            for (int j = 0; j < cols; ++j) {
//                result.data[i][j] = data[j][i];
//            }
//        }
//
//        return result;
//    }
//
//    void applyFunction(double (*func)(double)) {
//        for (auto& row : data) {
//            for (auto& val : row) {
//                val = func(val);
//            }
//        }
//    }
//
//    static double relu(double x) {
//        return std::max(0.0, x);
//    }
//
//    static double relu_derivative(double x) {
//        return x > 0 ? 1.0 : 0.0;
//    }
//
//    static double sigmoid(double x) {
//        return 1.0 / (1.0 + exp(-x));
//    }
//
//    static double sigmoid_derivative(double x) {
//        double sig = sigmoid(x);
//        return sig * (1 - sig);
//    }
//};
//
//// Simple random sampling without replacement
//template <typename T>
//std::vector<T> randomSample(const std::vector<T>& data, int sample_size) {
//    std::vector<T> copy = data;
//    std::vector<T> result;
//    std::random_device rd;
//    std::mt19937 gen(rd());
//
//    std::shuffle(copy.begin(), copy.end(), gen);
//
//    if (sample_size > copy.size()) {
//        sample_size = copy.size();
//    }
//
//    result.assign(copy.begin(), copy.begin() + sample_size);
//
//    return result;
//}
//
//class NeuralNetwork {
//public:
//    Matrix w1, w2;
//    Matrix b1, b2;
//
//    NeuralNetwork(int input_size, int hidden_size, int output_size)
//        : w1(input_size, hidden_size), w2(hidden_size, output_size),
//        b1(1, hidden_size, false), b2(1, output_size, false) {}
//
//    Matrix forward(const Matrix& input) const {
//        Matrix hidden = (input * w1) + b1;
//        hidden.applyFunction(Matrix::relu);
//        Matrix output = (hidden * w2) + b2;
//        return output;
//    }
//
//    void backpropagate(const Matrix& input, const Matrix& hidden, const Matrix& output,
//        const Matrix& target, double learning_rate) {
//        // Calculate the error at the output layer
//        Matrix output_error = output - target;
//        Matrix output_delta = output_error;  // Assuming linear activation at the output
//
//        // Calculate the error at the hidden layer
//        Matrix hidden_error = output_delta * w2.transpose();
//        hidden_error.applyFunction(Matrix::relu_derivative);
//        Matrix hidden_delta = hidden_error;
//
//        // Assuming hidden is [batch_size x hidden_size] and output_delta is [batch_size x output_size]
//// w2 = w2 - (hidden.transpose() * output_delta) * learning_rate;
//        Matrix hidden_T = hidden.transpose(); // [hidden_size x batch_size]
//        Matrix grad_w2 = hidden_T * output_delta; // [hidden_size x output_size]
//        grad_w2 = grad_w2 * learning_rate; // scale by learning rate
//        w2 = w2 - grad_w2; // update weights
//
//        // b2 = b2 - output_delta * learning_rate;
//        Matrix output_delta_mean = output_delta.mean(); // [1 x output_size]
//        b2 = b2 - (output_delta_mean * learning_rate);
//
//        // Assuming input is [batch_size x input_size] and hidden_delta is [batch_size x hidden_size]
//        // w1 = w1 - (input.transpose() * hidden_delta) * learning_rate;
//        Matrix input_T = input.transpose(); // [input_size x batch_size]
//        Matrix grad_w1 = input_T * hidden_delta; // [input_size x hidden_size]
//        grad_w1 = grad_w1 * learning_rate; // scale by learning rate
//        w1 = w1 - grad_w1; // update weights
//
//        // b1 = b1 - hidden_delta * learning_rate;
//        Matrix hidden_delta_mean = hidden_delta.mean(); // [1 x hidden_size]
//        b1 = b1 - (hidden_delta_mean * learning_rate);
//    }
//};
//
//struct Experience {
//    std::vector<double> state;
//    int action;
//    double reward;
//    std::vector<double> next_state;
//    bool done;
//};
//
//class ReplayBuffer {
//public:
//    std::vector<Experience> buffer;
//    int capacity;
//
//    ReplayBuffer(int capacity) : capacity(capacity) {}
//
//    void add(Experience experience) {
//        if (buffer.size() >= capacity)
//            buffer.erase(buffer.begin());
//        buffer.push_back(experience);
//    }
//
//    std::vector<Experience> sample(int batch_size) {
//        return randomSample(buffer, batch_size);
//    }
//
//    bool isReady(int batch_size) {
//        return buffer.size() >= batch_size;
//    }
//};
//
//class DQNAgent {
//public:
//    NeuralNetwork q_network;
//    NeuralNetwork target_network;
//    ReplayBuffer replay_buffer;
//    double learning_rate;
//    double gamma;
//    double epsilon;
//    int input_size;
//    int output_size;
//    int target_update_frequency;
//
//    DQNAgent(int input_size, int hidden_size, int output_size, int buffer_size,
//        double learning_rate, double gamma, double epsilon, int target_update_frequency)
//        : q_network(input_size, hidden_size, output_size),
//        target_network(input_size, hidden_size, output_size),
//        replay_buffer(buffer_size), learning_rate(learning_rate), gamma(gamma),
//        epsilon(epsilon), input_size(input_size), output_size(output_size),
//        target_update_frequency(target_update_frequency) {}
//
//    int getAction(const std::vector<double>& state) {
//        if ((rand() / (double)RAND_MAX) < epsilon)
//            return rand() % output_size;
//
//        Matrix input(1, input_size, false);
//        input.data[0] = state;
//
//        Matrix q_values = q_network.forward(input);
//        return std::distance(q_values.data[0].begin(),
//            std::max_element(q_values.data[0].begin(),
//                q_values.data[0].end()));
//    }
//
//    void train(int batch_size) {
//        if (!replay_buffer.isReady(batch_size))
//            return;
//
//        auto batch = replay_buffer.sample(batch_size);
//
//        for (const auto& experience : batch) {
//            Matrix input(1, input_size, false);
//            input.data[0] = experience.state;
//
//            Matrix target = q_network.forward(input);
//            double q_update = experience.reward;
//
//            if (!experience.done) {
//                Matrix next_input(1, input_size, false);
//                next_input.data[0] = experience.next_state;
//
//                Matrix next_q_values = target_network.forward(next_input);
//                q_update += gamma * *std::max_element(next_q_values.data[0].begin(),
//                    next_q_values.data[0].end());
//            }
//
//            target.data[0][experience.action] = q_update;
//
//            Matrix hidden = (input * q_network.w1) + q_network.b1;
//            hidden.applyFunction(Matrix::relu);
//            Matrix output = (hidden * q_network.w2) + q_network.b2;
//
//            q_network.backpropagate(input, hidden, output, target, learning_rate);
//        }
//    }
//
//    void updateTargetNetwork() {
//        target_network.w1 = q_network.w1;
//        target_network.w2 = q_network.w2;
//        target_network.b1 = q_network.b1;
//        target_network.b2 = q_network.b2;
//    }
//
//    void trainAndUpdateTarget(int episode, int batch_size) {
//        train(batch_size);
//        if (episode % target_update_frequency == 0) {
//            updateTargetNetwork();
//        }
//    }
//};