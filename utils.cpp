#include "utils.h"
#include <string>
#include <vector>

using namespace std;

bool IsPointInside(const Vector2& point, const Rectangle& rect) {
    return CheckCollisionPointRec(point, rect);
}

static void ResetGrid(AppState& s) {
    for (int i = 0; i < AppState::MAX_GRID; i++) {
        s.rowTargets[i] = "";
        s.colTargets[i] = "";
        for (int j = 0; j < AppState::MAX_GRID; j++) {
            s.grid[i][j] = "";
        }
    }
}

static void TrySolvePuzzle(AppState& s) {
    bool isInputComplete = true;

    vector<vector<int>> intGrid(s.gridSize, vector<int>(s.gridSize, 0));
    vector<int> intRowTargets(s.gridSize, 0);
    vector<int> intColTargets(s.gridSize, 0);

    try {
        for (int i = 0; i < s.gridSize; i++) {
            if (s.rowTargets[i] == "") isInputComplete = false;
            else intRowTargets[i] = stoi(s.rowTargets[i]);

            if (s.colTargets[i] == "") isInputComplete = false;
            else intColTargets[i] = stoi(s.colTargets[i]);

            for (int j = 0; j < s.gridSize; j++) {
                if (s.grid[i][j] == "") isInputComplete = false;
                else intGrid[i][j] = stoi(s.grid[i][j]);
            }
        }

        if (!isInputComplete) {
            s.statusMessage = "Error: Please fill all cells and targets before solving!";
            s.isSolved = false;
            return;
        }

        s.statusMessage = "Solving...";
        bool success = SolveSumplete(intGrid, intRowTargets, intColTargets, s.solvedBoard);

        if (success) {
            s.statusMessage = "Puzzle Solved successfully!";
            s.isSolved = true;
        } else {
            s.statusMessage = "Puzzle cannot be solved or has multiple solutions.";
            s.isSolved = false;
        }
    } catch (...) {
        s.statusMessage = "Data error. Please enter valid numbers.";
        s.isSolved = false;
    }
}

void HandleAppInput(AppState& s,
                    Vector2 mousePoint,
                    Rectangle btnMinus,
                    Rectangle btnPlus,
                    Rectangle btnClear,
                    Rectangle btnSolve,
                    float startX,
                    float startY,
                    float stepSize,
                    float cellSize,
                    int screenHeight) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        bool actionClicked = false;

        if (IsPointInside(mousePoint, btnMinus) && s.gridSize > AppState::MIN_GRID) {
            s.gridSize--;
            s.currentSelection = NONE;
            s.selRow = -1;
            s.selCol = -1;
            s.isSolved = false;
            actionClicked = true;
        }

        if (IsPointInside(mousePoint, btnPlus) && s.gridSize < AppState::MAX_GRID) {
            s.gridSize++;
            s.currentSelection = NONE;
            s.selRow = -1;
            s.selCol = -1;
            s.isSolved = false;
            actionClicked = true;
        }

        if (IsPointInside(mousePoint, btnClear)) {
            ResetGrid(s);
            s.statusMessage = "Grid cleared.";
            s.currentSelection = NONE;
            s.selRow = -1;
            s.selCol = -1;
            s.isSolved = false;
            actionClicked = true;
        }

        if (IsPointInside(mousePoint, btnSolve)) {
            s.currentSelection = NONE;
            s.selRow = -1;
            s.selCol = -1;
            TrySolvePuzzle(s);
            actionClicked = true;
        }

        if (!actionClicked) {
            bool clickedCell = false;

            for (int i = 0; i < s.gridSize; i++) {
                for (int j = 0; j < s.gridSize; j++) {
                    Rectangle cellRec = { startX + j * stepSize, startY + i * stepSize, cellSize, cellSize };
                    if (IsPointInside(mousePoint, cellRec)) {
                        s.currentSelection = GRID_CELL;
                        s.selRow = i;
                        s.selCol = j;
                        clickedCell = true;
                        s.isSolved = false;
                    }
                }
            }

            for (int i = 0; i < s.gridSize; i++) {
                Rectangle targetRec = { startX + s.gridSize * stepSize + 20.0f, startY + i * stepSize, cellSize, cellSize };
                if (IsPointInside(mousePoint, targetRec)) {
                    s.currentSelection = ROW_TARGET;
                    s.selRow = i;
                    s.selCol = -1;
                    clickedCell = true;
                    s.isSolved = false;
                }
            }

            for (int j = 0; j < s.gridSize; j++) {
                Rectangle targetRec = { startX + j * stepSize, startY + s.gridSize * stepSize + 20.0f, cellSize, cellSize };
                if (IsPointInside(mousePoint, targetRec)) {
                    s.currentSelection = COL_TARGET;
                    s.selRow = -1;
                    s.selCol = j;
                    clickedCell = true;
                    s.isSolved = false;
                }
            }

            if (!clickedCell && mousePoint.y > 100 && mousePoint.y < screenHeight - 150) {
                s.currentSelection = NONE;
            }
        }
    }

    if (s.currentSelection != NONE) {
        string* activeStr = nullptr;
        if (s.currentSelection == GRID_CELL) activeStr = &s.grid[s.selRow][s.selCol];
        else if (s.currentSelection == ROW_TARGET) activeStr = &s.rowTargets[s.selRow];
        else if (s.currentSelection == COL_TARGET) activeStr = &s.colTargets[s.selCol];

        if (activeStr != nullptr) {
            int key = GetCharPressed();
            while (key > 0) {
                if (s.currentSelection == GRID_CELL) {
                    if (key >= '1' && key <= '9') {
                        *activeStr = string(1, (char)key);
                    }
                } else {
                    if (key >= '0' && key <= '9' && activeStr->length() < 3) {
                        *activeStr += (char)key;
                    }
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && !activeStr->empty()) {
                activeStr->pop_back();
            }
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            if (s.currentSelection == GRID_CELL) {
                if (s.selCol < s.gridSize - 1) s.selCol++;
                else {
                    s.currentSelection = ROW_TARGET;
                    s.selCol = -1;
                }
            } else if (s.currentSelection == COL_TARGET && s.selCol < s.gridSize - 1) {
                s.selCol++;
            }
        }

        if (IsKeyPressed(KEY_LEFT)) {
            if (s.currentSelection == GRID_CELL && s.selCol > 0) {
                s.selCol--;
            } else if (s.currentSelection == ROW_TARGET) {
                s.currentSelection = GRID_CELL;
                s.selCol = s.gridSize - 1;
            } else if (s.currentSelection == COL_TARGET && s.selCol > 0) {
                s.selCol--;
            }
        }

        if (IsKeyPressed(KEY_DOWN)) {
            if (s.currentSelection == GRID_CELL) {
                if (s.selRow < s.gridSize - 1) s.selRow++;
                else {
                    s.currentSelection = COL_TARGET;
                    s.selRow = -1;
                }
            } else if (s.currentSelection == ROW_TARGET && s.selRow < s.gridSize - 1) {
                s.selRow++;
            }
        }

        if (IsKeyPressed(KEY_UP)) {
            if (s.currentSelection == GRID_CELL && s.selRow > 0) {
                s.selRow--;
            } else if (s.currentSelection == COL_TARGET) {
                s.currentSelection = GRID_CELL;
                s.selRow = s.gridSize - 1;
            } else if (s.currentSelection == ROW_TARGET && s.selRow > 0) {
                s.selRow--;
            }
        }
    }
}