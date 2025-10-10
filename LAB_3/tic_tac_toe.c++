#include <iostream>
#include <climits>
using namespace std;

const int n = 3;

struct State {
    State* parent;
    char board[n][n];
    int score;
    bool isMax;          // true if X's turn, false if O's turn
    State* children[9];  // at most 9 possible moves (first level)
    int childCount;
};

// Utility Functions 
bool moves_left(char board[n][n]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (board[i][j] == '_')
                return true;
    return false;
}

int get_score(char board[n][n]) {
    // Rows and Columns
    for (int i = 0; i < n; i++) {
        if (board[i][0] != '_' && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0] == 'X' ? 1 : -1;
        if (board[0][i] != '_' && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i] == 'X' ? 1 : -1;
    }
    // Diagonals
    if (board[0][0] != '_' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0] == 'X' ? 1 : -1;
    if (board[0][2] != '_' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2] == 'X' ? 1 : -1;

    return 0;
}

void print_board(char board[n][n]) {
    cout << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << board[i][j] << " ";
        cout << "\n";
    }
    cout << "\n";
}

void copy_board(char dest[n][n], char src[n][n]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dest[i][j] = src[i][j];
}

// Tree Generation (Minimax) 
int generate_tree(State* s) {
    int score = get_score(s->board);
    if (score != 0) {
        s->score = score;
        return score;
    }

    if (!moves_left(s->board)) {
        s->score = 0;
        return 0;
    }

    s->childCount = 0;

    if (s->isMax) { // X's move
        int best = INT_MIN;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (s->board[i][j] == '_') {
                    State* child = new State;
                    copy_board(child->board, s->board);
                    child->board[i][j] = 'X';
                    child->parent = s;
                    child->isMax = false;
                    child->childCount = 0;
                    best = max(best, generate_tree(child));
                    s->children[s->childCount++] = child;
                }
            }
        }
        s->score = best;
        return best;
    } else { // O's move
        int best = INT_MAX;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (s->board[i][j] == '_') {
                    State* child = new State;
                    copy_board(child->board, s->board);
                    child->board[i][j] = 'O';
                    child->parent = s;
                    child->isMax = true;
                    child->childCount = 0;
                    best = min(best, generate_tree(child));
                    s->children[s->childCount++] = child;
                }
            }
        }
        s->score = best;
        return best;
    }
}

State* best_computer_move(State* s) {
    int bestVal = INT_MIN;
    State* bestChild = nullptr;
    for (int i = 0; i < s->childCount; i++) {
        State* child = s->children[i];
        if (child->score > bestVal) {
            bestVal = child->score;
            bestChild = child;
        }
    }
    return bestChild;
}

void play_game(State* root) {
    State* current = root;

    while (true) {
        print_board(current->board);
        int score = get_score(current->board);

        if (score == 1) { cout << "Computer (X) wins!\n"; break; }
        if (score == -1) { cout << "You (O) win!\n"; break; }
        if (!moves_left(current->board)) { cout << "It's a draw!\n"; break; }

        // Player move
        int x, y;
        cout << "Enter your move (row col): ";
        cin >> x >> y;

        if (x < 0 || x >= n || y < 0 || y >= n || current->board[x][y] != '_') {
            cout << "Invalid move. Try again.\n";
            continue;
        }

        // Find or create the matching state
        State* nextState = nullptr;
        for (int i = 0; i < current->childCount; i++) {
            State* child = current->children[i];
            if (child->board[x][y] == 'O') {
                nextState = child;
                break;
            }
        }
        if (nextState == nullptr) {
            State* newChild = new State;
            copy_board(newChild->board, current->board);
            newChild->board[x][y] = 'O';
            newChild->isMax = true;
            newChild->parent = current;
            newChild->childCount = 0;
            generate_tree(newChild);
            nextState = newChild;
        }

        current = nextState;
        print_board(current->board);
        score = get_score(current->board);
        if (score == -1) { cout << "You (O) win!\n"; break; }
        if (!moves_left(current->board)) { cout << "It's a draw!\n"; break; }

        // Computer's move
        current = best_computer_move(current);
        cout << "Computer plays X:\n";
    }
}

int main() {
    State* root = new State;
    root->parent = nullptr;
    root->isMax = true;  // Computer (X) starts
    root->childCount = 0;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            root->board[i][j] = '_';

    generate_tree(root);
    play_game(root);
    return 0;
}
