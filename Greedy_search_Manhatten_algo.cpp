#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Size of the puzzle
const int PUZZLE_SIZE = 3;

// Structure to represent a state of the puzzle
struct PuzzleState {
    vector<vector<int>> puzzleBoard;
    int heuristic;  // Manhattan distance heuristic value

    // Constructor
    PuzzleState(const vector<vector<int>>& board) : puzzleBoard(board) {
        heuristic = calculateHeuristic();
    }

    // Calculate the Manhattan distance heuristic
    int calculateHeuristic() const {
        int distance = 0;
        for (int i = 0; i < PUZZLE_SIZE; ++i) {
            for (int j = 0; j < PUZZLE_SIZE; ++j) {
                if (puzzleBoard[i][j] != 0) {
                    int targetRow = (puzzleBoard[i][j] - 1) / PUZZLE_SIZE;
                    int targetCol = (puzzleBoard[i][j] - 1) % PUZZLE_SIZE;
                    distance += abs(i - targetRow) + abs(j - targetCol);
                }
            }
        }
        return distance;
    }

    // Check if the current state is the goal state
    bool isGoal() const {
        int targetValue = 0;
        for (int i = 0; i < PUZZLE_SIZE; ++i) {
            for (int j = 0; j < PUZZLE_SIZE; ++j) {
                if (puzzleBoard[i][j] != targetValue) {
                    return false;
                }
                ++targetValue;
            }
        }
        return true;
    }

    // Check if two puzzle states are equal
    bool operator==(const PuzzleState& other) const {
        return puzzleBoard == other.puzzleBoard;
    }
};

// Hash function for PuzzleState (used for unordered_set)
struct PuzzleStateHash {
    size_t operator()(const PuzzleState& state) const {
        size_t hashValue = 0;
        for (const auto& row : state.puzzleBoard) {
            for (int val : row) {
                hashValue ^= hash<int>()(val) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            }
        }
        return hashValue;
    }
};

// Compare function for priority_queue
struct ComparePuzzleState {
    bool operator()(const PuzzleState& lhs, const PuzzleState& rhs) const {
        return lhs.heuristic > rhs.heuristic;
    }
};

// Function to print the puzzle state
void printPuzzleState(const PuzzleState& state) {
    for (const auto& row : state.puzzleBoard) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
    cout << "Heuristic: " << state.heuristic << endl;
    cout << "-----------------" << endl;
}

// Function to perform the greedy search
void greedySearch(const PuzzleState& initial) {
    priority_queue<PuzzleState, vector<PuzzleState>, ComparePuzzleState> priorityQueue;
    unordered_set<PuzzleState, PuzzleStateHash> visited;

    priorityQueue.push(initial);

    while (!priorityQueue.empty()) {
        PuzzleState current = priorityQueue.top();
        priorityQueue.pop();

        if (current.isGoal()) {
            cout << "Goal state reached!" << endl;
            printPuzzleState(current);
            return;
        }

        if (visited.find(current) == visited.end()) {
            visited.insert(current);
            printPuzzleState(current);

            // Generate possible next states
            // In this implementation, only left, right, up, and down moves are considered
            vector<int> moves = {-1, 0, 1};

            for (int dx : moves) {
                for (int dy : moves) {
                    if (abs(dx) + abs(dy) == 1) {
                        int newX = 0, newY = 0;

                        // Find the position of the zero (empty space)
                        for (int i = 0; i < PUZZLE_SIZE; ++i) {
                            for (int j = 0; j < PUZZLE_SIZE; ++j) {
                                if (current.puzzleBoard[i][j] == 0) {
                                    newX = i + dx;
                                    newY = j + dy;
                                    break;
                                }
                            }
                        }

                        // Check if the new position is within the bounds
                        if (newX >= 0 && newX < PUZZLE_SIZE && newY >= 0 && newY < PUZZLE_SIZE) {
                            // Create a new state by swapping the zero and the adjacent tile
                            vector<vector<int>> newBoard = current.puzzleBoard;
                            swap(newBoard[newX][newY], newBoard[newX - dx][newY - dy]);

                            PuzzleState nextState(newBoard);

                            // Add the new state to the priority queue
                            priorityQueue.push(nextState);
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

    PuzzleState initialPuzzleState(initialState);

    cout << "Initial state:" << endl;
    printPuzzleState(initialPuzzleState);

    cout << "Starting greedy search with Manhattan distance heuristic..." << endl;
    greedySearch(initialPuzzleState);

    return 0;
}
