#include <iostream>
#include <queue>
#include <cmath>
#include <climits>
using namespace std;

struct State {
    State* parent;
    int g[10][10];     // cost so far
    int h[10][10];     // heuristic
    int x, y;          // current position
};

// Movement directions: left, right, up, down
int row_moves[] = {0, 0, -1, 1};
int col_moves[] = {-1, 1, 0, 0};

bool isValid(int x, int y, int n) {
    return (x >= 0 && x < n && y >= 0 && y < n);
}

bool isGoal(int x, int y, int destX, int destY) {
    return (x == destX && y == destY);
}

void printPath(State* s) {
    if (s == nullptr) return;
    printPath(s->parent);
    cout << "(" << s->x << "," << s->y << ")\n";
}

void copyBoard(int gDest[10][10], int hDest[10][10], int gSrc[10][10], int hSrc[10][10], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            gDest[i][j] = gSrc[i][j];
            hDest[i][j] = hSrc[i][j];
        }
}

int manhattanDist(int x, int y, int destX, int destY) {
    return abs(x - destX) + abs(y - destY);
}

struct Compare {
    bool operator()(State* a, State* b) {
        int fA = a->g[a->x][a->y] + a->h[a->x][a->y];
        int fB = b->g[b->x][b->y] + b->h[b->x][b->y];
        return fA > fB; // Min-heap based on f(n)
    }
};

void aStar(State* start, int n, int destX, int destY, bool water[10][10]) {
    priority_queue<State*, vector<State*>, Compare> pq;
    bool visited[10][10] = {false};

    pq.push(start);

    while (!pq.empty()) {
        State* current = pq.top();
        pq.pop();

        if (visited[current->x][current->y])
            continue;
        visited[current->x][current->y] = true;

        if (isGoal(current->x, current->y, destX, destY)) {
            cout << "\nPath found:\n";
            printPath(current);
            return;
        }

        for (int k = 0; k < 4; k++) {
            int newX = current->x + row_moves[k];
            int newY = current->y + col_moves[k];

            if (isValid(newX, newY, n) && !water[newX][newY]) {
                State* child = new State;
                copyBoard(child->g, child->h, current->g, current->h, n);
                child->x = newX;
                child->y = newY;
                child->parent = current;

                child->g[newX][newY] = current->g[current->x][current->y] + 1;
                child->h[newX][newY] = manhattanDist(newX, newY, destX, destY);

                pq.push(child);
            }
        }
    }

    cout << "No path found!\n";
}

int main() {
    int n;
    cout << "Enter grid size (n x n): ";
    cin >> n;

    int srcX, srcY, destX, destY;
    cout << "Enter source (x y): ";
    cin >> srcX >> srcY;
    cout << "Enter destination (x y): ";
    cin >> destX >> destY;

    bool water[10][10] = {false};
    char ch;
    do {
        int wx, wy;
        cout << "Enter obstacles (x y): ";
        cin >> wx >> wy;
        if (isValid(wx, wy, n))
            water[wx][wy] = true;
        cout << "Add more obstacles? (y/n): ";
        cin >> ch;
    } while (ch == 'y');

    State* start = new State;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            start->g[i][j] = INT_MAX;
            start->h[i][j] = 0;
        }

    start->x = srcX;
    start->y = srcY;
    start->g[srcX][srcY] = 0;
    start->h[srcX][srcY] = manhattanDist(srcX, srcY, destX, destY);
    start->parent = nullptr;

    cout << "\nSolving using A* (Manhattan Distance)...\n";
    aStar(start, n, destX, destY, water);

    return 0;
}
