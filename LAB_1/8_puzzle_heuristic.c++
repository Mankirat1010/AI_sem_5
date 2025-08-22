#include <iostream>
#include <queue>
#include <cmath>
#include <cstring>
using namespace std;

struct State {
    int board[3][3];
    State* parent;
    int g;  // cost so far
    int h;  // heuristic (Manhattan distance)
    int f;  // total cost (g + h)
};

// Movement directions (left, right, up, down)
int row_moves[] = {0, 0, -1, 1};
int col_moves[] = {-1, 1, 0, 0};

// Function to copy 3x3 board
void copyBoard(int dest[3][3], int src[3][3]) {
    memcpy(dest, src, sizeof(int) * 9);
}

// Function to print a board
void printBoard(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            cout << board[i][j] << " ";
        cout << "\n";
    }
}

// Manhattan Distance heuristic
int manhattan(int board[3][3], int goal[3][3]) {
    int dist = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] != 0) {
                for (int x = 0; x < 3; x++)
                    for (int y = 0; y < 3; y++)
                        if (board[i][j] == goal[x][y])
                            dist += abs(i - x) + abs(j - y);
            }
    return dist;
}

// Compare function for priority queue (min-heap)
struct Compare {
    bool operator()(State* a, State* b) {
        return a->f > b->f;
    }
};

bool isGoal(int a[3][3], int b[3][3]) {
    return memcmp(a, b, sizeof(int) * 9) == 0;
}

bool isValid(int x, int y) {
    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

bool alreadyVisited(State* visited[], int visitedCount, int board[3][3]) {
    for (int i = 0; i < visitedCount; i++) {
        if (memcmp(visited[i]->board, board, sizeof(int) * 9) == 0)
            return true;
    }
    return false;
}

// Print path from start to goal
void printPath(State* s) {
    State* path[100];
    int count = 0;

    while (s != nullptr) {
        path[count++] = s;
        s = s->parent;
    }

    cout << "\n--- Solution Steps ---\n";
    for (int i = count - 1; i >= 0; i--) {
        cout << "Step " << (count - 1 - i) << ":\n";
        printBoard(path[i]->board);
        cout << "------------------\n";
    }
    cout << "Total steps to reach goal: " << count - 1 << "\n";
}

// A* Search Algorithm
void solveAStar(int start[3][3], int goal[3][3]) {
    priority_queue<State*, vector<State*>, Compare> pq;
    State* visited[10000];
    int visitedCount = 0;

    State* startState = new State;
    copyBoard(startState->board, start);
    startState->parent = nullptr;
    startState->g = 0;
    startState->h = manhattan(start, goal);
    startState->f = startState->g + startState->h;

    pq.push(startState);
    visited[visitedCount++] = startState;

    while (!pq.empty()) {
        State* current = pq.top();
        pq.pop();

        if (isGoal(current->board, goal)) {
            printPath(current);
            return;
        }

        int blankX, blankY;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (current->board[i][j] == 0)
                    blankX = i, blankY = j;

        for (int k = 0; k < 4; k++) {
            int newX = blankX + row_moves[k];
            int newY = blankY + col_moves[k];

            if (isValid(newX, newY)) {
                State* child = new State;
                copyBoard(child->board, current->board);
                swap(child->board[blankX][blankY], child->board[newX][newY]);
                child->parent = current;
                child->g = current->g + 1;
                child->h = manhattan(child->board, goal);
                child->f = child->g + child->h;

                if (!alreadyVisited(visited, visitedCount, child->board)) {
                    visited[visitedCount++] = child;
                    pq.push(child);
                }
            }
        }
    }

    cout << "No solution found.\n";
}

int main() {
    int start[3][3], goal[3][3];

    cout << "Enter Start puzzle values (use 0 for blank):\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << "Start[" << i << "][" << j << "]: ";
            cin >> start[i][j];
        }
    }

    cout << "\nEnter Goal puzzle values (use 0 for blank):\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << "Goal[" << i << "][" << j << "]: ";
            cin >> goal[i][j];
        }
    }

    cout << "\nStart State:\n";
    printBoard(start);
    cout << "\nGoal State:\n";
    printBoard(goal);

    cout << "\nSolving puzzle using A* Search (Manhattan Distance)...\n";
    solveAStar(start, goal);

    return 0;
}
