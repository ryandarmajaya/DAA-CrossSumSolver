#pragma once

#include "raylib.h"
#include "solver.h"
#include <string>
#include <vector>

enum SelectionType { NONE, GRID_CELL, ROW_TARGET, COL_TARGET };

struct AppState {
    static constexpr int MIN_GRID = 3;
    static constexpr int MAX_GRID = 8;

    int gridSize = MIN_GRID;
    std::vector<std::vector<std::string>> grid;
    std::vector<std::string> rowTargets;
    std::vector<std::string> colTargets;
    std::vector<std::vector<Cell>> solvedBoard;

    bool isSolved = false;
    SelectionType currentSelection = NONE;
    int selRow = -1;
    int selCol = -1;
    std::string statusMessage = "Enter the puzzle numbers, use Arrow Keys to move.";

    AppState()
        : grid(MAX_GRID, std::vector<std::string>(MAX_GRID, "")),
          rowTargets(MAX_GRID, ""),
          colTargets(MAX_GRID, ""),
          solvedBoard(MAX_GRID, std::vector<Cell>(MAX_GRID)) {}
};

bool IsPointInside(const Vector2& point, const Rectangle& rect);
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