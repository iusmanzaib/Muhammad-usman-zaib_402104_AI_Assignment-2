#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace chrono;

#define MATRIX_SIZE 3
int total_steps = 0;
int rows[] = {1, 0, -1, 0};
int cols[] = {0, -1, 0, 1};

int goal_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
};

struct PuzzleNode {
    PuzzleNode *parent;
    int matrix[MATRIX_SIZE][MATRIX_SIZE];
    int position_x, position_y;
    int cost;
    int level;
    int heuristic;
};

int displayMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    return 0;
}

PuzzleNode *createNode(int matrix[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int new_x, int new_y, int level, PuzzleNode *parent) {
    PuzzleNode *node = new PuzzleNode;
    node->parent = parent;

    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            node->matrix[i][j] = matrix[i][j];
        }
    }

    int temp = node->matrix[x][y];
    node->matrix[x][y] = node->matrix[new_x][new_y];
    node->matrix[new_x][new_y] = temp;
    node->cost = INT_MAX;
    node->level = level;
    node->position_x = new_x;
    node->position_y = new_y;
    return node;
}

int calculateCost(int matrix[MATRIX_SIZE][MATRIX_SIZE], int goal[MATRIX_SIZE][MATRIX_SIZE]) {
    int count = 0;
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            if (matrix[i][j] != goal[i][j])
                count++;
    return count;
}

int calculateHeuristic(int matrix[MATRIX_SIZE][MATRIX_SIZE], int goal[MATRIX_SIZE][MATRIX_SIZE]) {
    int heuristic = 0;
    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            if (matrix[i][j] != goal[i][j])
                heuristic++;
    return heuristic;
}

int isWithinLimits(int x, int y) {
    return (x >= 0 && x < MATRIX_SIZE && y >= 0 && y < MATRIX_SIZE);
}

void displayPuzzle(PuzzleNode *root) {
    if (root == NULL)
        return;
    displayPuzzle(root->parent);
    displayMatrix(root->matrix);
    printf("\n");
}

struct CompareNodes {
    bool operator()(const PuzzleNode *lhs, const PuzzleNode *rhs) const {
        return lhs->heuristic > rhs->heuristic;
    }
};

// Function to convert the puzzle matrix into a string
std::string matrixToString(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    std::ostringstream oss;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            oss << matrix[i][j] << " ";
        }
    }
    return oss.str();
}

void solvePuzzle(int initial[MATRIX_SIZE][MATRIX_SIZE], int x, int y, int goal[MATRIX_SIZE][MATRIX_SIZE]) {
    auto start_time = high_resolution_clock::now();

    priority_queue<PuzzleNode *, vector<PuzzleNode *>, CompareNodes> pq;

    unordered_set<string> visited;

    PuzzleNode *root = createNode(initial, x, y, x, y, 0, NULL);
    root->cost = calculateCost(initial, goal);
    root->heuristic = calculateHeuristic(initial, goal);
    pq.push(root);

    while (!pq.empty()) {
        PuzzleNode *min = pq.top();
        pq.pop();

        if (min->cost == 0) {
            auto end_time = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(end_time - start_time);

            cout << "Goal state reached in " << total_steps << " steps.\n";
            cout << "Time taken: " << duration.count() << " milliseconds\n";
            displayPuzzle(min);
            return;
        }

        for (int i = 0; i < 4; i++) {
            if (isWithinLimits(min->position_x + rows[i], min->position_y + cols[i])) {
                PuzzleNode *child = createNode(min->matrix, min->position_x, min->position_y, min->position_x + rows[i], min->position_y + cols[i], min->level + 1, min);
                child->cost = calculateCost(child->matrix, goal);
                child->heuristic = calculateHeuristic(child->matrix, goal);

                if (visited.find(matrixToString(child->matrix)) == visited.end()) {
                    pq.push(child);
                    visited.insert(matrixToString(child->matrix));
                    total_steps++;
                }
            }
        }
    }
}

int main() {
    int initial[MATRIX_SIZE][MATRIX_SIZE];

    cout << "\n\t\t----------------------------------------------------------------------------\n";
    cout << " Enter Initial state of puzzle in this form \n";
    cout << "*** 2 3 1 5 6 0 8 4 7 ***\n>> ";

    for (int i = 0; i < MATRIX_SIZE; i++)
        for (int j = 0; j < MATRIX_SIZE; j++)
            cin >> initial[i][j];

    cout << "initial puzzle entered is :  \n>> ";
    displayMatrix(initial);
    cout << "\n\t\t----------------------------------------------------------------------------\n";
    cout << "Puzzle is being solved  \n>> ";

    int start_x = 1, start_y = 2;

    solvePuzzle(initial, start_x, start_y, goal_matrix);

    return 0;
}
