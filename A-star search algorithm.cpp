#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace chrono;

#define MATRIX_SIZE 3
int moveSteps = 0;
int rowMoves[] = {1, 0, -1, 0};
int colMoves[] = {0, -1, 0, 1};

int goalMatrix[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8}
};

struct PuzzleNode {
    PuzzleNode *parentNode;
    int puzzle[MATRIX_SIZE][MATRIX_SIZE];
    int x, y;
    int cost;
    int depth;
};

int printPuzzle(int puzzle[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++)
            cout << puzzle[i][j] << " ";
        cout << endl;
    }
    return 0;
}

PuzzleNode *createNewNode(int puzzle[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int newX, int newY, int depth, PuzzleNode *parent) {
    PuzzleNode *node = new PuzzleNode;
    node->parentNode = parent;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            node->puzzle[i][j] = puzzle[i][j];
        }
    }

    int temp = node->puzzle[x][y];
    node->puzzle[x][y] = node->puzzle[newX][newY];
    node->puzzle[newX][newY] = temp;
    node->cost = INT_MAX;
    node->depth = depth;
    node->x = newX;
    node->y = newY;
    return node;
}

int calculateCost(int puzzle[MATRIX_SIZE][MATRIX_SIZE], int goal[MATRIX_SIZE][MATRIX_SIZE]) {
    int count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            if (puzzle[i][j] != goal[i][j]) {
                count++;
            }
    return count;
}

int isWithinLimits(int x, int y) {
    return (x >= 0 && x < MATRIX_SIZE && y >= 0 && y < MATRIX_SIZE);
}

void displayPuzzle(PuzzleNode *root) {
    if (root == NULL)
        return;
    displayPuzzle(root->parentNode);
    printPuzzle(root->puzzle);
    cout << endl;
}

struct CompareNodes {
    bool operator()(const PuzzleNode *lhs, const PuzzleNode *rhs) const {
        return (lhs->cost + lhs->depth) > (rhs->cost + rhs->depth);
    }
};

std::string stringifyPuzzle(int puzzle[MATRIX_SIZE][MATRIX_SIZE]) {
    ostringstream oss;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            oss << puzzle[i][j] << " ";
        }
    }
    return oss.str();
}

void solvePuzzle(int initialPuzzle[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int goal[MATRIX_SIZE][MATRIX_SIZE]) {
    auto startTime = high_resolution_clock::now();

    priority_queue<PuzzleNode *, vector<PuzzleNode *>, CompareNodes> priorityQueue;

    unordered_set<string> visitedStates;

    PuzzleNode *rootNode = createNewNode(initialPuzzle, x, y, x, y, 0, NULL);
    rootNode->cost = calculateCost(initialPuzzle, goal);
    priorityQueue.push(rootNode);

    while (!priorityQueue.empty()) {
        PuzzleNode *minCostNode = priorityQueue.top();
        priorityQueue.pop();

        if (minCostNode->cost == 0) {
            auto endTime = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(endTime - startTime);

            cout << "Achieved the goal state in " << moveSteps << " steps." << endl;
            cout << "Time taken: " << duration.count() << " milliseconds" << endl;
            displayPuzzle(minCostNode);
            return;
        }

        for (int i = 0; i < 4; i++) {
            if (isWithinLimits(minCostNode->x + rowMoves[i], minCostNode->y + colMoves[i])) {
                PuzzleNode *childNode = createNewNode(minCostNode->puzzle, minCostNode->x, minCostNode->y, minCostNode->x + rowMoves[i], minCostNode->y + colMoves[i], minCostNode->depth + 1, minCostNode);
                childNode->cost = calculateCost(childNode->puzzle, goal);

                if (visitedStates.find(stringifyPuzzle(childNode->puzzle)) == visitedStates.end()) {
                    priorityQueue.push(childNode);
                    visitedStates.insert(stringifyPuzzle(childNode->puzzle));
                    moveSteps++;
                }
            }
        }
    }
}

int main() {
    int initialState[MATRIX_SIZE][MATRIX_SIZE];

    cout << "\n\t\t----------------------------------------------------------------------------\n";
    cout << " Provide the current configuration of the puzzle.  \n";
    cout << "*** 2 3 1 5 6 0 8 4 7 ***\n>> ";

    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            cin >> initialState[i][j];

    cout << "The puzzle in its starting state is entered:  \n>> ";
    printPuzzle(initialState);
    cout << "\n\t\t----------------------------------------------------------------------------\n";
    cout << "Addressing the puzzle challenge  \n>> ";

    int initialX = 1, initialY = 2;

    solvePuzzle(initialState, initialX, initialY, goalMatrix);

    return 0;
}
