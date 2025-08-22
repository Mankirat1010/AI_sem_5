#include <iostream>
#include <stack>
#include <cstring> // for memcpy, memcmp
using namespace std;

struct State {
    State* parent;
    int board[3][3];
    int state_no;
};

int row_moves[] = {0, 0, -1, 1};
int col_moves[] = {-1, 1, 0, 0};

bool isValid(int x, int y) {
    return x >= 0 && x < 3 && y >= 0 && y < 3;
}

bool isGoal(int current[3][3], int goal[3][3]) {
    return memcmp(current, goal, sizeof(int) * 9) == 0;
}

void printBoard(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            cout << board[i][j] << " ";
        cout << "\n";
    }
}

void printPath(State* s) {
    if (s == nullptr) return;
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

bool alreadyVisited(State* visited[], int visitedCount, int board[3][3]) {
    for (int i = 0; i < visitedCount; i++) {
        if (memcmp(visited[i]->board, board, sizeof(int) * 9) == 0)
            return true;
    }
    return false;
}

void copyBoard(int dest[3][3], int src[3][3]) {
    memcpy(dest, src, sizeof(int) * 9);
}

void solveDFS(State* start, int goal[3][3]) {
    stack<State*> st;
    State* visited[10000];
    int visitedCount = 0;

    st.push(start);
    visited[visitedCount++] = start;

    while (!st.empty()) {
        State* current = st.top();
        st.pop();

        if (isGoal(current->board, goal)) {
            printPath(current);
            return;
        }

        int blankX, blankY;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (current->board[i][j] == 0)
                    blankX = i, blankY = j;

        // Explore all possible moves (DFS)
        for (int k = 0; k < 4; k++) {
            int newX = blankX + row_moves[k];
            int newY = blankY + col_moves[k];

            if (isValid(newX, newY)) {
                State* child = new State;
                copyBoard(child->board, current->board);
                swap(child->board[blankX][blankY], child->board[newX][newY]);
                child->parent = current;
                child->state_no = current->state_no + 1;

                if (!alreadyVisited(visited, visitedCount, child->board)) {
                    visited[visitedCount++] = child;
                    st.push(child);
                }
            }
        }
    }

    cout << "No solution found.\n";
}

int main() {
    int start[3][3], goal[3][3];
    int sx, sy, gx, gy;

    cout << "Enter Start puzzle values (use 0 for blank):\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << "Start[" << i << "][" << j << "]: ";
            cin >> start[i][j];
            if (start[i][j] == 0) sx = i, sy = j;
        }
    }

    cout << "\nEnter Goal puzzle values (use 0 for blank):\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << "Goal[" << i << "][" << j << "]: ";
            cin >> goal[i][j];
            if (goal[i][j] == 0) gx = i, gy = j;
        }
    }

    cout << "\nStart State:\n";
    printBoard(start);
    cout << "\nGoal State:\n";
    printBoard(goal);

    State* startState = new State;
    copyBoard(startState->board, start);
    startState->parent = nullptr;
    startState->state_no = 0;

    cout << "\nSolving puzzle using DFS...\n";
    solveDFS(startState, goal);

    return 0;
}
