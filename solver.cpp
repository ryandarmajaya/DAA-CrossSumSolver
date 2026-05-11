#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

enum Status { UNKNOWN, KEEP, DELETE };

const int SIZE = 6;

struct Cell {
    int value;
    Status status = UNKNOWN;
};

int gridValues[SIZE][SIZE] = {
    {5, 9, 2, 7, 2, 7},
    {9, 8, 2, 2, 2, 7},
    {1, 7, 5, 6, 7, 5},
    {2, 9, 3, 8, 2, 1},
    {9, 1, 2, 7, 3, 1},
    {5, 8, 7, 2, 5, 5}
};

int rowTargets[SIZE] = {12, 19, 18, 14, 14, 20};
int colTargets[SIZE] = {23, 32, 5, 17, 10, 10};

Cell board[SIZE][SIZE];

bool solveLine(vector<Cell*> line, int target) {
    int n = line.size();
    vector<int> validMasks;

    for (int mask = 0; mask < (1 << n); ++mask) {
        int currentSum = 0;
        bool possible = true;

        for (int i = 0; i < n; ++i) {
            bool isSelected = (mask >> i) & 1;
            
            if (line[i]->status == KEEP && !isSelected) { possible = false; break; }
            if (line[i]->status == DELETE && isSelected) { possible = false; break; }
            
            if (isSelected) currentSum += line[i]->value;
        }

        if (possible && currentSum == target) {
            validMasks.push_back(mask);
        }
    }

    if (validMasks.empty()) return false;

    bool changed = false;
    for (int i = 0; i < n; ++i) {
        if (line[i]->status != UNKNOWN) continue;

        bool inAll = true;
        bool inNone = true;

        for (int mask : validMasks) {
            bool isSelected = (mask >> i) & 1;
            if (isSelected) inNone = false;
            else inAll = false;
        }

        if (inAll) {
            line[i]->status = KEEP;
            changed = true;
        } else if (inNone) {
            line[i]->status = DELETE;
            changed = true;
        }
    }
    return changed;
}

void displayBoard() {
    cout << "\nKondisi Grid Saat Ini (K = Keep, X = Delete, . = Unknown):\n";
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cout << board[i][j].value;
            if (board[i][j].status == KEEP) cout << "K ";
            else if (board[i][j].status == DELETE) cout << "X ";
            else cout << ". ";
        }
        cout << "| " << rowTargets[i] << endl;
    }
}

int main() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j].value = gridValues[i][j];
            if (board[i][j].value > rowTargets[i] || board[i][j].value > colTargets[j]) {
                board[i][j].status = DELETE;
            }
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;

        for (int i = 0; i < SIZE; ++i) {
            vector<Cell*> row;
            for (int j = 0; j < SIZE; ++j) row.push_back(&board[i][j]);
            if (solveLine(row, rowTargets[i])) changed = true;
        }

        for (int j = 0; j < SIZE; ++j) {
            vector<Cell*> col;
            for (int i = 0; i < SIZE; ++i) col.push_back(&board[i][j]);
            if (solveLine(col, colTargets[j])) changed = true;
        }
    }

    displayBoard();

    return 0;
}