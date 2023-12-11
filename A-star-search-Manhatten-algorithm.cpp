#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Size of the puzzle
const int PuzzleSize = 3;

// Structure to represent a state of the puzzle
struct PuzzleConfiguration {
    vector<vector<int>> gameBoard;
    int heuristicEstimate;  // Manhattan distance heuristic value
    int movementCost;       // Cost to reach this state

    // Constructor
    PuzzleConfiguration(const vector<vector<int>>& board, int cost) : gameBoard(board), movementCost(cost) {
        heuristicEstimate = calculateHeuristic();
    }

    // Calculate the Manhattan distance heuristic
    int calculateHeuristic() const {
        int totalDistance = 0;

        for (int i = 0; i < PuzzleSize; ++i) {
            for (int j = 0; j < PuzzleSize; ++j) {
                int value = gameBoard[i][j];
                if (value != 0) {
                    int targetRow = (value - 1) / PuzzleSize;
                    int targetCol = (value - 1) % PuzzleSize;
                    totalDistance += abs(i - targetRow) + abs(j - targetCol);
                }
            }
        }

        return totalDistance;
    }

    // Check if the current state is the goal state
    bool isGoalState() const {
        int targetValue = 0;
        for (int i = 0; i < PuzzleSize; ++i) {
            for (int j = 0; j < PuzzleSize; ++j) {
                if (gameBoard[i][j] != targetValue) {
                    return false;
                }
                ++targetValue;
            }
        }
        return true;
    }

    // Check if two puzzle states are equal
    bool operator==(const PuzzleConfiguration& other) const {
        return gameBoard == other.gameBoard;
    }
};

// Hash function for PuzzleConfiguration (used for unordered_set)
struct PuzzleConfigurationHash {
    size_t operator()(const PuzzleConfiguration& configuration) const {
        size_t hashValue = 0;
        for (const auto& row : configuration.gameBoard) {
            for (int val : row) {
                hashValue ^= hash<int>()(val) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            }
        }
        return hashValue;
    }
};

// Compare function for priority_queue
struct ComparePuzzleConfiguration {
    bool operator()(const PuzzleConfiguration& lhs, const PuzzleConfiguration& rhs) const {
        return (lhs.movementCost + lhs.heuristicEstimate) > (rhs.movementCost + rhs.heuristicEstimate);
    }
};

// Function to print the puzzle state
void printPuzzleConfiguration(const PuzzleConfiguration& configuration) {
    for (const auto& row : configuration.gameBoard) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << "Heuristic: " << configuration.heuristicEstimate << " Cost: " << configuration.movementCost << endl;
    cout << "-----------------" << endl;
}

// Function to perform the A* search with Manhattan distance heuristic
void aStarSearch(const PuzzleConfiguration& initial) {
    priority_queue<PuzzleConfiguration, vector<PuzzleConfiguration>, ComparePuzzleConfiguration> priorityQueue;
    unordered_set<PuzzleConfiguration, PuzzleConfigurationHash> visitedConfigurations;

    priorityQueue.push(initial);

    while (!priorityQueue.empty()) {
        PuzzleConfiguration currentConfiguration = priorityQueue.top();
        priorityQueue.pop();

        if (currentConfiguration.isGoalState()) {
            cout << "Goal state reached!" << endl;
            printPuzzleConfiguration(currentConfiguration);
            return;
        }

        if (visitedConfigurations.find(currentConfiguration) == visitedConfigurations.end()) {
            visitedConfigurations.insert(currentConfiguration);
            printPuzzleConfiguration(currentConfiguration);

            // Generate possible next configurations
            // In this implementation, only left, right, up, and down moves are considered
            vector<int> moves = {-1, 0, 1};

            for (int dx : moves) {
                for (int dy : moves) {
                    if (abs(dx) + abs(dy) == 1) {
                        int newX = 0, newY = 0;

                        // Find the position of the zero (empty space)
                        for (int i = 0; i < PuzzleSize; ++i) {
                            for (int j = 0; j < PuzzleSize; ++j) {
                                if (currentConfiguration.gameBoard[i][j] == 0) {
                                    newX = i + dx;
                                    newY = j + dy;
                                    break;
                                }
                            }
                        }

                        // Check if the new position is within the bounds
                        if (newX >= 0 && newX < PuzzleSize && newY >= 0 && newY < PuzzleSize) {
                            // Create a new configuration by swapping the zero and the adjacent tile
                            vector<vector<int>> newBoard = currentConfiguration.gameBoard;
                            swap(newBoard[newX][newY], newBoard[newX - dx][newY - dy]);

                            PuzzleConfiguration nextConfiguration(newBoard, currentConfiguration.movementCost + 1);

                            // Add the new configuration to the priority queue
                            priorityQueue.push(nextConfiguration);
                        }
                    }
                }
            }
        }
    }

    cout << "Goal state not reachable!" << endl;
}

int main() {
    // Initial state
    vector<vector<int>> initialState = {
        {8, 0, 6},
        {5, 4, 7},
        {2, 3, 1}
    };

    PuzzleConfiguration initialPuzzleConfiguration(initialState, 0);

    cout << "Initial state:" << endl;
    printPuzzleConfiguration(initialPuzzleConfiguration);

    cout << "Starting A* search with Manhattan distance heuristic..." << endl;
    aStarSearch(initialPuzzleConfiguration);

    return 0;
}
