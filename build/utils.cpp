#include "utils.h"

#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

AppState::AppState()
    : grid(MAX_GRID, vector<string>(MAX_GRID, "")),
      rowTargets(MAX_GRID, ""),
      colTargets(MAX_GRID, ""),
      solvedBoard(MAX_GRID, vector<Cell>(MAX_GRID)) {}

int safeStoi(const string& str) {
    if (str.empty() || str == "-") throw invalid_argument("Empty or invalid number");
    return stoi(str);
}

static void ClearGrid(AppState& state) {
    for (int i = 0; i < AppState::MAX_GRID; i++) {
        state.rowTargets[i] = "";
        state.colTargets[i] = "";
        for (int j = 0; j < AppState::MAX_GRID; j++) {
            state.grid[i][j] = "";
        }
    }
}

static void SolvePuzzle(AppState& state) {
    vector<vector<int>> intGrid(state.gridSize, vector<int>(state.gridSize, 0));
    vector<int> intRowTargets(state.gridSize, 0);
    vector<int> intColTargets(state.gridSize, 0);

    try {
        for (int i = 0; i < state.gridSize; i++) {
            intRowTargets[i] = safeStoi(state.rowTargets[i]);
            intColTargets[i] = safeStoi(state.colTargets[i]);
            for (int j = 0; j < state.gridSize; j++) {
                intGrid[i][j] = safeStoi(state.grid[i][j]);
            }
        }

        state.statusMessage = "Solving...";
        bool success = SolveSumplete(intGrid, intRowTargets, intColTargets, state.solvedBoard);

        if (success) {
            state.statusMessage = "Puzzle Solved successfully!";
            state.isSolved = true;
        } else {
            state.statusMessage = "Puzzle cannot be solved (invalid configuration).";
            state.isSolved = false;
        }
    } catch (...) {
        state.statusMessage = "Error: Please fill all cells properly. '-' alone is invalid.";
        state.isSolved = false;
    }
}

void HandleAppInput(AppState& state,
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
        if (CheckCollisionPointRec(mousePoint, btnMinus) && state.gridSize > AppState::MIN_GRID) {
            state.gridSize--;
            state.currentSelection = NONE;
            state.isSolved = false;
        }

        if (CheckCollisionPointRec(mousePoint, btnPlus) && state.gridSize < AppState::MAX_GRID) {
            state.gridSize++;
            state.currentSelection = NONE;
            state.isSolved = false;
        }

        if (CheckCollisionPointRec(mousePoint, btnClear)) {
            ClearGrid(state);
            state.statusMessage = "Grid cleared.";
            state.currentSelection = NONE;
            state.isSolved = false;
        }

        if (CheckCollisionPointRec(mousePoint, btnSolve)) {
            state.currentSelection = NONE;
            SolvePuzzle(state);
        }

        bool clickedCell = false;

        for (int i = 0; i < state.gridSize; i++) {
            for (int j = 0; j < state.gridSize; j++) {
                Rectangle cellRec = { startX + j * stepSize, startY + i * stepSize, cellSize, cellSize };
                if (CheckCollisionPointRec(mousePoint, cellRec)) {
                    state.currentSelection = GRID_CELL;
                    state.selRow = i;
                    state.selCol = j;
                    clickedCell = true;
                    state.isSolved = false;
                }
            }
        }

        for (int i = 0; i < state.gridSize; i++) {
            Rectangle targetRec = { startX + state.gridSize * stepSize + 20.0f, startY + i * stepSize, cellSize, cellSize };
            if (CheckCollisionPointRec(mousePoint, targetRec)) {
                state.currentSelection = ROW_TARGET;
                state.selRow = i;
                state.selCol = -1;
                clickedCell = true;
                state.isSolved = false;
            }
        }

        for (int j = 0; j < state.gridSize; j++) {
            Rectangle targetRec = { startX + j * stepSize, startY + state.gridSize * stepSize + 20.0f, cellSize, cellSize };
            if (CheckCollisionPointRec(mousePoint, targetRec)) {
                state.currentSelection = COL_TARGET;
                state.selRow = -1;
                state.selCol = j;
                clickedCell = true;
                state.isSolved = false;
            }
        }

        if (!clickedCell && mousePoint.y > 100 && mousePoint.y < screenHeight - 150) {
            state.currentSelection = NONE;
        }
    }

    if (state.currentSelection != NONE) {
        string* activeStr = nullptr;
        if (state.currentSelection == GRID_CELL) activeStr = &state.grid[state.selRow][state.selCol];
        else if (state.currentSelection == ROW_TARGET) activeStr = &state.rowTargets[state.selRow];
        else if (state.currentSelection == COL_TARGET) activeStr = &state.colTargets[state.selCol];

        if (activeStr != nullptr) {
            int key = GetCharPressed();
            while (key > 0) {
                bool isMinus = (key == '-');
                bool isNumber = (key >= '0' && key <= '9');

                int maxLen = (activeStr->length() > 0 && (*activeStr)[0] == '-')
                                 ? ((state.currentSelection == GRID_CELL) ? 3 : 4)
                                 : ((state.currentSelection == GRID_CELL) ? 2 : 3);

                if (isMinus && activeStr->empty()) {
                    *activeStr += (char)key;
                } else if (isNumber) {
                    if (*activeStr == "0") {
                        if (key != '0') *activeStr = string(1, (char)key);
                    } else if (*activeStr == "-" && key == '0') {
                    } else if ((int)activeStr->length() < maxLen) {
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
            if (state.currentSelection == GRID_CELL) {
                if (state.selCol < state.gridSize - 1) state.selCol++;
                else { state.currentSelection = ROW_TARGET; state.selCol = -1; }
            } else if (state.currentSelection == COL_TARGET && state.selCol < state.gridSize - 1) {
                state.selCol++;
            }
        }

        if (IsKeyPressed(KEY_LEFT)) {
            if (state.currentSelection == GRID_CELL && state.selCol > 0) state.selCol--;
            else if (state.currentSelection == ROW_TARGET) { state.currentSelection = GRID_CELL; state.selCol = state.gridSize - 1; }
            else if (state.currentSelection == COL_TARGET && state.selCol > 0) state.selCol--;
        }

        if (IsKeyPressed(KEY_DOWN)) {
            if (state.currentSelection == GRID_CELL) {
                if (state.selRow < state.gridSize - 1) state.selRow++;
                else { state.currentSelection = COL_TARGET; state.selRow = -1; }
            } else if (state.currentSelection == ROW_TARGET && state.selRow < state.gridSize - 1) {
                state.selRow++;
            }
        }

        if (IsKeyPressed(KEY_UP)) {
            if (state.currentSelection == GRID_CELL && state.selRow > 0) state.selRow--;
            else if (state.currentSelection == COL_TARGET) { state.currentSelection = GRID_CELL; state.selRow = state.gridSize - 1; }
            else if (state.currentSelection == ROW_TARGET && state.selRow > 0) state.selRow--;
        }
    }
}