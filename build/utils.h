#pragma once

#include "raylib.h"
#include "solver.h"
#include <string>
#include <vector>

enum SelectionType { NONE, GRID_CELL, ROW_TARGET, COL_TARGET };

struct AppState {
    static constexpr int MIN_GRID = 3;
    static constexpr int MAX_GRID = 9;

    int gridSize = 3;
    std::vector<std::vector<std::string>> grid;
    std::vector<std::string> rowTargets;
    std::vector<std::string> colTargets;
    std::vector<std::vector<Cell>> solvedBoard;

    bool isSolved = false;
    SelectionType currentSelection = NONE;
    int selRow = -1;
    int selCol = -1;
    std::string statusMessage = "Enter numbers and press Solve when ready.";

    AppState();
};

int safeStoi(const std::string& str);

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
                    int screenHeight);