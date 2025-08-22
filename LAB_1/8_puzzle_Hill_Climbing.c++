#include <iostream>
#include <climits>
#include <cstdlib>
#include <cstring>
using namespace std;

struct State {
    int board[3][3];
    State* parent;
    int level;
};

// Movement directions (left, right, up, down)
int row_moves[] = {0, 0, -1, 1};
int col_moves[] = {-1, 1, 0, 0};

// Copy 3x3 board
void copyBoard(int dest[3][3], int src[3][3]) {
    memcpy(dest, src, sizeof(int) * 9);
}

// Print board
void printBoard(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            cout << board[i][j] << " ";
        cout << "\n";
    }
}

// Compare two boards
bool isGoal(int a[3][3], int b[3][3]) {
    return memcmp(a, b, sizeof(int) * 9) == 0;
}

bool isValid(int x, int y) {
    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

// Misplaced tiles heuristic
int misplacedCount(int board[3][3], int goal[3][3]) {
    int count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != 0 && board[i][j] != goal[i][j])
                count++;
        }
    }
    return count;
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

// Hill Climbing Search
void hillClimb(int start[3][3], int goal[3][3]) {
    State* current = new State;
    copyBoard(current->board, start);
    current->parent = nullptr;
    current->level = 0;

    int currentHeuristic = misplacedCount(current->board, goal);

    while (true) {
        int blankX, blankY;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (current->board[i][j] == 0)
                    blankX = i, blankY = j;

        State* nextState = nullptr;
        int bestHeuristic = INT_MAX;

        // Try all 4 possible moves
        for (int k = 0; k < 4; k++) {
            int newX = blankX + row_moves[k];
            int newY = blankY + col_moves[k];

            if (isValid(newX, newY)) {
                State* child = new State;
                copyBoard(child->board, current->board);
                swap(child->board[blankX][blankY], child->board[newX][newY]);
                child->parent = current;
                child->level = current->level + 1;

                int h = misplacedCount(child->board, goal);
                if (h < bestHeuristic) {
                    bestHeuristic = h;
                    nextState = child;
                }
            }
        }

        if (nextState == nullptr) {
            cout << "No further moves possible.\n";
            printPath(current);
            return;
        }

        // If no improvement, stop (local minima)
        if (bestHeuristic >= currentHeuristic) {
            cout << "\nLocal Maxima Reached (No Better Neighbor Found)\n";
            printPath(current);
            return;
        }

        // Move to next better state
        current = nextState;
        currentHeuristic = bestHeuristic;

        if (isGoal(current->board, goal)) {
            cout << "\nGoal Reached!\n";
            printPath(current);
            return;
        }
    }
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

    cout << "\nSolving puzzle using Hill Climbing (Misplaced Tiles Heuristic)...\n";
    hillClimb(start, goal);

    return 0;
}
