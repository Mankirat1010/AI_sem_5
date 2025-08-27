#include <iostream>
using namespace std;

int capacity1;  
int capacity2; 
int target;     
int targetJug;  

bool visited[101][101];   // Visited states
pair<int, int> path[1000]; // Store path
int pathIndex = 0;

bool dfs(int jug1, int jug2) {
    if (visited[jug1][jug2])
        return false;

    visited[jug1][jug2] = true;
    path[pathIndex++] = {jug1, jug2};

    // Check if goal is reached
    if ((targetJug == 1 && jug1 == target) ||
        (targetJug == 2 && jug2 == target)) {
        return true;
    }

    // All possible operations:

    // Fill jug1
    if (dfs(capacity1, jug2)) return true;

    // Fill jug2
    if (dfs(jug1, capacity2)) return true;

    // Empty jug1
    if (dfs(0, jug2)) return true;

    // Empty jug2
    if (dfs(jug1, 0)) return true;

    // Pour jug1 -> jug2
    int pour1to2 = min(jug1, capacity2 - jug2);
    if (dfs(jug1 - pour1to2, jug2 + pour1to2)) return true;

    // Pour jug2 -> jug1
    int pour2to1 = min(jug2, capacity1 - jug1);
    if (dfs(jug1 + pour2to1, jug2 - pour2to1)) return true;

    // Backtrack
    pathIndex--;
    return false;
}

int main() {
    cout << "Enter capacity of Jug X: ";
    cin >> capacity1;
    cout << "Enter capacity of Jug Y: ";
    cin >> capacity2;
    cout << "Enter target amount of water: ";
    cin >> target;
    cout << "In which jug do you want the target (1 for X, 2 for Y)? ";
    cin >> targetJug;

    // Initialize visited array
    for (int i = 0; i <= capacity1; i++)
        for (int j = 0; j <= capacity2; j++)
            visited[i][j] = false;

    if (dfs(0, 0)) {
        cout << "\nSolution steps:\n";
        for (int i = 0; i < pathIndex; i++) {
            cout << "(" << path[i].first << ", " << path[i].second << ")\n";
        }
    } else {
        cout << "No solution found.\n";
    }

    return 0;
}
