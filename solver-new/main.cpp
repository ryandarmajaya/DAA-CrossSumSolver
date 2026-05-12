#include "raylib.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "solver.h"

using namespace std;

enum SelectionType { NONE, GRID_CELL, ROW_TARGET, COL_TARGET };

int safeStoi(const string& str) {
    if (str.empty() || str == "-") throw invalid_argument("Empty or invalid number");
    return stoi(str);
}

int main() {
    const int screenWidth = 950;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Sumplete Solver (Dynamic Layout)");
    SetTargetFPS(60);

    int gridSize = 3;
    const int MIN_GRID = 3;
    const int MAX_GRID = 8;
    
    float cellSize = 60.0f;
    float cellGap = 6.0f; 
    float stepSize = cellSize + cellGap;

    vector<vector<string>> grid(MAX_GRID, vector<string>(MAX_GRID, ""));
    vector<string> rowTargets(MAX_GRID, "");
    vector<string> colTargets(MAX_GRID, "");

    vector<vector<Cell>> solvedBoard(MAX_GRID, vector<Cell>(MAX_GRID));
    bool isSolved = false; 

    SelectionType currentSelection = NONE;
    int selRow = -1;
    int selCol = -1;
    string statusMessage = "Enter numbers and press Solve when ready.";

    Color bgLight = { 248, 250, 252, 255 };
    Color cellBgNormal = WHITE;
    Color cellBgHover = { 241, 245, 249, 255 };
    Color cellActive = { 254, 240, 138, 255 };
    Color targetBgNormal = { 219, 234, 254, 255 };
    Color targetBgHover = { 191, 219, 254, 255 };
    Color targetText = { 30, 58, 138, 255 };
    Color textNormal = { 15, 23, 42, 255 };
    Color textMuted = { 100, 116, 139, 255 };
    Color btnSolveNormal = { 34, 197, 94, 255 };
    Color btnSolveHover = { 22, 163, 74, 255 };
    Color btnClearNormal = { 239, 68, 68, 255 };
    Color btnClearHover = { 220, 38, 38, 255 };
    Color btnActionNormal = { 148, 163, 184, 255 };
    Color btnActionHover = { 100, 116, 139, 255 };

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        float totalGridWidth = (gridSize * stepSize) - cellGap + 20.0f + cellSize;
        float totalGridHeight = (gridSize * stepSize) - cellGap + 20.0f + cellSize;
        
        float totalUIHeight = totalGridHeight + 35.0f + 45.0f + 15.0f + 20.0f;

        float startX = (screenWidth - totalGridWidth) / 2.0f;
        float startY = (screenHeight - totalUIHeight) / 2.0f + 40.0f; 

        string sizeText = "Grid: " + to_string(gridSize) + " x " + to_string(gridSize);
        int sizeTextW = MeasureText(sizeText.c_str(), 20);
        float blockW = 30 + 15 + sizeTextW + 15 + 30;
        float blockStartX = (screenWidth - blockW) / 2.0f;

        Rectangle btnMinus = { blockStartX, 35, 30, 30 };
        Rectangle btnPlus = { blockStartX + 30 + 15 + sizeTextW + 15, 35, 30, 30 };
        
        float buttonsY = startY + totalGridHeight + 35.0f;
        Rectangle btnClear = { screenWidth / 2.0f - 110, buttonsY, 100, 45 };
        Rectangle btnSolve = { screenWidth / 2.0f + 10, buttonsY, 100, 45 };

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(mousePoint, btnMinus) && gridSize > MIN_GRID) {
                gridSize--; currentSelection = NONE; isSolved = false;
            }
            if (CheckCollisionPointRec(mousePoint, btnPlus) && gridSize < MAX_GRID) {
                gridSize++; currentSelection = NONE; isSolved = false;
            }
            if (CheckCollisionPointRec(mousePoint, btnClear)) {
                for(int i=0; i<MAX_GRID; i++) { rowTargets[i] = ""; colTargets[i] = ""; for(int j=0; j<MAX_GRID; j++) grid[i][j] = ""; }
                statusMessage = "Grid cleared."; currentSelection = NONE; isSolved = false;
            }
            
            if (CheckCollisionPointRec(mousePoint, btnSolve)) {
                currentSelection = NONE;
                vector<vector<int>> intGrid(gridSize, vector<int>(gridSize, 0));
                vector<int> intRowTargets(gridSize, 0);
                vector<int> intColTargets(gridSize, 0);

                try {
                    for(int i = 0; i < gridSize; i++) {
                        intRowTargets[i] = safeStoi(rowTargets[i]);
                        intColTargets[i] = safeStoi(colTargets[i]);
                        for(int j = 0; j < gridSize; j++) {
                            intGrid[i][j] = safeStoi(grid[i][j]);
                        }
                    }
                    
                    statusMessage = "Solving...";
                    bool success = SolveSumplete(intGrid, intRowTargets, intColTargets, solvedBoard);
                    
                    if (success) {
                        statusMessage = "Puzzle Solved successfully!";
                        isSolved = true;
                    } else {
                        statusMessage = "Puzzle cannot be solved (invalid configuration).";
                        isSolved = false;
                    }
                } catch (...) {
                    statusMessage = "Error: Please fill all cells properly. '-' alone is invalid.";
                    isSolved = false;
                }
            }

            bool clickedCell = false;
            for (int i = 0; i < gridSize; i++) {
                for (int j = 0; j < gridSize; j++) {
                    Rectangle cellRec = { startX + j * stepSize, startY + i * stepSize, cellSize, cellSize };
                    if (CheckCollisionPointRec(mousePoint, cellRec)) {
                        currentSelection = GRID_CELL; selRow = i; selCol = j; clickedCell = true; isSolved = false;
                    }
                }
            }
            for (int i = 0; i < gridSize; i++) {
                Rectangle targetRec = { startX + gridSize * stepSize + 20.0f, startY + i * stepSize, cellSize, cellSize };
                if (CheckCollisionPointRec(mousePoint, targetRec)) {
                    currentSelection = ROW_TARGET; selRow = i; selCol = -1; clickedCell = true; isSolved = false;
                }
            }
            for (int j = 0; j < gridSize; j++) {
                Rectangle targetRec = { startX + j * stepSize, startY + gridSize * stepSize + 20.0f, cellSize, cellSize };
                if (CheckCollisionPointRec(mousePoint, targetRec)) {
                    currentSelection = COL_TARGET; selRow = -1; selCol = j; clickedCell = true; isSolved = false;
                }
            }
            if (!clickedCell && mousePoint.y > 100 && mousePoint.y < buttonsY - 10) currentSelection = NONE;
        }

        if (currentSelection != NONE) {
            string* activeStr = nullptr;
            if (currentSelection == GRID_CELL) activeStr = &grid[selRow][selCol];
            else if (currentSelection == ROW_TARGET) activeStr = &rowTargets[selRow];
            else if (currentSelection == COL_TARGET) activeStr = &colTargets[selCol];

            if (activeStr != nullptr) {
                int key = GetCharPressed();
                while (key > 0) {
                    bool isMinus = (key == '-');
                    bool isNumber = (key >= '0' && key <= '9');

                    int maxLen = (activeStr->length() > 0 && (*activeStr)[0] == '-') ? 
                                 ((currentSelection == GRID_CELL) ? 3 : 4) : 
                                 ((currentSelection == GRID_CELL) ? 2 : 3);

                    if (isMinus && activeStr->empty()) {
                        *activeStr += (char)key;
                    } 
                    else if (isNumber) {
                        if (*activeStr == "0") {
                            if (key != '0') *activeStr = string(1, (char)key);
                        } 
                        else if (*activeStr == "-" && key == '0') {
                        } 
                        else if (activeStr->length() < maxLen) {
                            *activeStr += (char)key;
                        }
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && activeStr->length() > 0) activeStr->pop_back();
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                if (currentSelection == GRID_CELL) {
                    if (selCol < gridSize - 1) selCol++;
                    else { currentSelection = ROW_TARGET; selCol = -1; } 
                } else if (currentSelection == COL_TARGET && selCol < gridSize - 1) selCol++;
            }
            if (IsKeyPressed(KEY_LEFT)) {
                if (currentSelection == GRID_CELL && selCol > 0) selCol--;
                else if (currentSelection == ROW_TARGET) { currentSelection = GRID_CELL; selCol = gridSize - 1; }
                else if (currentSelection == COL_TARGET && selCol > 0) selCol--;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                if (currentSelection == GRID_CELL) {
                    if (selRow < gridSize - 1) selRow++;
                    else { currentSelection = COL_TARGET; selRow = -1; } 
                } else if (currentSelection == ROW_TARGET && selRow < gridSize - 1) selRow++;
            }
            if (IsKeyPressed(KEY_UP)) {
                if (currentSelection == GRID_CELL && selRow > 0) selRow--;
                else if (currentSelection == COL_TARGET) { currentSelection = GRID_CELL; selRow = gridSize - 1; }
                else if (currentSelection == ROW_TARGET && selRow > 0) selRow--;
            }
        }

        BeginDrawing();
        ClearBackground(bgLight);

        DrawText("SUMPLETE SOLVER", 30, 30, 28, textNormal);
        DrawText("Dynamic Matric Solver", 30, 65, 16, textMuted);

        bool hoverMinus = CheckCollisionPointRec(mousePoint, btnMinus);
        bool hoverPlus = CheckCollisionPointRec(mousePoint, btnPlus);
        DrawRectangleRounded(btnMinus, 0.4f, 0, hoverMinus ? btnActionHover : btnActionNormal);
        DrawRectangleRounded(btnPlus, 0.4f, 0, hoverPlus ? btnActionHover : btnActionNormal);
        DrawText("-", btnMinus.x + 10, btnMinus.y + 2, 24, WHITE);
        DrawText("+", btnPlus.x + 8, btnPlus.y + 5, 24, WHITE);
        DrawText(sizeText.c_str(), blockStartX + 30 + 15, 40, 20, textNormal);

        bool showCursor = ((int)(GetTime() * 2.0)) % 2 == 0;

        for (int i = 0; i < gridSize; i++) {
            for (int j = 0; j < gridSize; j++) {
                Rectangle cellRec = { startX + j * stepSize, startY + i * stepSize, cellSize, cellSize };
                bool isHovered = CheckCollisionPointRec(mousePoint, cellRec);
                bool isActive = (currentSelection == GRID_CELL && selRow == i && selCol == j);

                Color bgColor = isActive ? cellActive : (isHovered ? cellBgHover : cellBgNormal);
                DrawRectangleRounded(cellRec, 0.2f, 0, bgColor);
                DrawRectangleRoundedLines(cellRec, 0.2f, 0, isActive ? ORANGE : textMuted);

                int textW = MeasureText(grid[i][j].c_str(), 20);
                
                if (isSolved && solvedBoard[i][j].status == CELL_DELETE) {
                    DrawText(grid[i][j].c_str(), cellRec.x + (cellSize - textW) / 2, cellRec.y + 20, 20, LIGHTGRAY);
                    DrawLineEx({cellRec.x + 10, cellRec.y + 10}, {cellRec.x + cellSize - 10, cellRec.y + cellSize - 10}, 3.0f, RED);
                } else if (isSolved && solvedBoard[i][j].status == KEEP) {
                    DrawCircleLines(cellRec.x + cellSize/2, cellRec.y + cellSize/2, 20.0f, GREEN);
                    DrawText(grid[i][j].c_str(), cellRec.x + (cellSize - textW) / 2, cellRec.y + 20, 20, textNormal);
                } else {
                    DrawText(grid[i][j].c_str(), cellRec.x + (cellSize - textW) / 2, cellRec.y + 20, 20, textNormal);
                }

                if (isActive && showCursor) DrawLine(cellRec.x + (cellSize + textW)/2 + 4, cellRec.y + 15, cellRec.x + (cellSize + textW)/2 + 4, cellRec.y + 45, textNormal);
            }
        }

        for (int i = 0; i < gridSize; i++) {
            Rectangle targetRec = { startX + gridSize * stepSize + 20.0f, startY + i * stepSize, cellSize, cellSize };
            bool isHovered = CheckCollisionPointRec(mousePoint, targetRec);
            bool isActive = (currentSelection == ROW_TARGET && selRow == i);
            Color bgColor = isActive ? cellActive : (isHovered ? targetBgHover : targetBgNormal);
            DrawRectangleRounded(targetRec, 0.3f, 0, bgColor);
            int textW = MeasureText(rowTargets[i].c_str(), 20);
            DrawText(rowTargets[i].c_str(), targetRec.x + (cellSize - textW) / 2, targetRec.y + 20, 20, targetText);
            if (isActive && showCursor) DrawLine(targetRec.x + (cellSize + textW)/2 + 4, targetRec.y + 15, targetRec.x + (cellSize + textW)/2 + 4, targetRec.y + 45, targetText);
        }

        for (int j = 0; j < gridSize; j++) {
            Rectangle targetRec = { startX + j * stepSize, startY + gridSize * stepSize + 20.0f, cellSize, cellSize };
            bool isHovered = CheckCollisionPointRec(mousePoint, targetRec);
            bool isActive = (currentSelection == COL_TARGET && selCol == j);
            Color bgColor = isActive ? cellActive : (isHovered ? targetBgHover : targetBgNormal);
            DrawRectangleRounded(targetRec, 0.3f, 0, bgColor);
            int textW = MeasureText(colTargets[j].c_str(), 20);
            DrawText(colTargets[j].c_str(), targetRec.x + (cellSize - textW) / 2, targetRec.y + 20, 20, targetText);
            if (isActive && showCursor) DrawLine(targetRec.x + (cellSize + textW)/2 + 4, targetRec.y + 15, targetRec.x + (cellSize + textW)/2 + 4, targetRec.y + 45, targetText);
        }

        DrawRectangleRounded(btnClear, 0.3f, 0, CheckCollisionPointRec(mousePoint, btnClear) ? btnClearHover : btnClearNormal);
        DrawText("CLEAR", btnClear.x + 25, btnClear.y + 13, 18, WHITE);

        DrawRectangleRounded(btnSolve, 0.3f, 0, CheckCollisionPointRec(mousePoint, btnSolve) ? btnSolveHover : btnSolveNormal);
        DrawText("SOLVE", btnSolve.x + 23, btnSolve.y + 13, 18, WHITE);

        int msgWidth = MeasureText(statusMessage.c_str(), 16);
        DrawText(statusMessage.c_str(), (screenWidth - msgWidth) / 2, buttonsY + 65.0f, 16, isSolved ? DARKGREEN : RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}