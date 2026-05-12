#include "raylib.h"
#include "utils.h"
#include <string>

using namespace std;

int main() {
    const int screenWidth = 900;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "CrossSum Solver");
    SetTargetFPS(60);

    AppState app;

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

        float cellSize = 50.0f;
        float cellGap = 6.0f;
        float stepSize = cellSize + cellGap;

        float totalWidth = (app.gridSize * stepSize) - cellGap + 20.0f + cellSize;
        float totalHeight = (app.gridSize * stepSize) - cellGap + 20.0f + cellSize;
        float startX = (screenWidth - totalWidth) / 2.0f;
        float startY = (screenHeight - totalHeight) / 2.0f + 20.0f;

        string sizeText = "Grid: " + to_string(app.gridSize) + " x " + to_string(app.gridSize);
        int sizeTextW = MeasureText(sizeText.c_str(), 20);
        float blockW = 30 + 15 + sizeTextW + 15 + 30;
        float blockStartX = (screenWidth - blockW) / 2.0f;

        Rectangle btnMinus = { blockStartX, 35, 30, 30 };
        Rectangle btnPlus = { blockStartX + 30 + 15 + sizeTextW + 15, 35, 30, 30 };
        Rectangle btnClear = { screenWidth / 2.0f - 110, screenHeight - 100, 100, 45 };
        Rectangle btnSolve = { screenWidth / 2.0f + 10, screenHeight - 100, 100, 45 };

        HandleAppInput(app, mousePoint, btnMinus, btnPlus, btnClear, btnSolve, startX, startY, stepSize, cellSize, screenHeight);

        BeginDrawing();
        ClearBackground(bgLight);

        DrawText("CROSSSUM SOLVER", 30, 30, 28, textNormal);
        DrawText("Dynamic Matrix Setup", 30, 65, 16, textMuted);

        bool hoverMinus = CheckCollisionPointRec(mousePoint, btnMinus);
        bool hoverPlus = CheckCollisionPointRec(mousePoint, btnPlus);

        DrawRectangleRounded(btnMinus, 0.4f, 0, hoverMinus ? btnActionHover : btnActionNormal);
        DrawRectangleRounded(btnPlus, 0.4f, 0, hoverPlus ? btnActionHover : btnActionNormal);
        DrawText("-", btnMinus.x + 10, btnMinus.y + 2, 24, WHITE);
        DrawText("+", btnPlus.x + 8, btnPlus.y + 5, 24, WHITE);
        DrawText(sizeText.c_str(), blockStartX + 30 + 15, 40, 20, textNormal);

        bool showCursor = ((int)(GetTime() * 2.0)) % 2 == 0;

        for (int i = 0; i < app.gridSize; i++) {
            for (int j = 0; j < app.gridSize; j++) {
                Rectangle cellRec = { startX + j * stepSize, startY + i * stepSize, cellSize, cellSize };
                bool isHovered = CheckCollisionPointRec(mousePoint, cellRec);
                bool isActive = (app.currentSelection == GRID_CELL && app.selRow == i && app.selCol == j);

                Color bgColor = isActive ? cellActive : (isHovered ? cellBgHover : cellBgNormal);
                DrawRectangleRounded(cellRec, 0.2f, 0, bgColor);
                DrawRectangleRoundedLines(cellRec, 0.2f, 0, isActive ? ORANGE : textMuted);

                int textW = MeasureText(app.grid[i][j].c_str(), 20);

                if (app.isSolved && app.solvedBoard[i][j].status == CELL_DELETE) {
                    DrawText(app.grid[i][j].c_str(), cellRec.x + (cellSize - textW) / 2, cellRec.y + 15, 20, LIGHTGRAY);
                    DrawLineEx({ cellRec.x + 10, cellRec.y + 10 }, { cellRec.x + cellSize - 10, cellRec.y + cellSize - 10 }, 3.0f, RED);
                } else if (app.isSolved && app.solvedBoard[i][j].status == KEEP) {
                    DrawCircleLines(cellRec.x + cellSize / 2, cellRec.y + cellSize / 2, 16.0f, GREEN);
                    DrawText(app.grid[i][j].c_str(), cellRec.x + (cellSize - textW) / 2, cellRec.y + 15, 20, textNormal);
                } else {
                    DrawText(app.grid[i][j].c_str(), cellRec.x + (cellSize - textW) / 2, cellRec.y + 15, 20, textNormal);
                }

                if (isActive && showCursor) {
                    DrawLine(cellRec.x + (cellSize + textW) / 2 + 2, cellRec.y + 10, cellRec.x + (cellSize + textW) / 2 + 2, cellRec.y + 35, textNormal);
                }
            }
        }

        for (int i = 0; i < app.gridSize; i++) {
            Rectangle targetRec = { startX + app.gridSize * stepSize + 20.0f, startY + i * stepSize, cellSize, cellSize };
            bool isHovered = CheckCollisionPointRec(mousePoint, targetRec);
            bool isActive = (app.currentSelection == ROW_TARGET && app.selRow == i);
            Color bgColor = isActive ? cellActive : (isHovered ? targetBgHover : targetBgNormal);
            DrawRectangleRounded(targetRec, 0.3f, 0, bgColor);
            int textW = MeasureText(app.rowTargets[i].c_str(), 20);
            DrawText(app.rowTargets[i].c_str(), targetRec.x + (cellSize - textW) / 2, targetRec.y + 15, 20, targetText);
            if (isActive && showCursor) {
                DrawLine(targetRec.x + (cellSize + textW) / 2 + 2, targetRec.y + 10, targetRec.x + (cellSize + textW) / 2 + 2, targetRec.y + 35, targetText);
            }
        }

        for (int j = 0; j < app.gridSize; j++) {
            Rectangle targetRec = { startX + j * stepSize, startY + app.gridSize * stepSize + 20.0f, cellSize, cellSize };
            bool isHovered = CheckCollisionPointRec(mousePoint, targetRec);
            bool isActive = (app.currentSelection == COL_TARGET && app.selCol == j);
            Color bgColor = isActive ? cellActive : (isHovered ? targetBgHover : targetBgNormal);
            DrawRectangleRounded(targetRec, 0.3f, 0, bgColor);
            int textW = MeasureText(app.colTargets[j].c_str(), 20);
            DrawText(app.colTargets[j].c_str(), targetRec.x + (cellSize - textW) / 2, targetRec.y + 15, 20, targetText);
            if (isActive && showCursor) {
                DrawLine(targetRec.x + (cellSize + textW) / 2 + 2, targetRec.y + 10, targetRec.x + (cellSize + textW) / 2 + 2, targetRec.y + 35, targetText);
            }
        }

        DrawRectangleRounded(btnClear, 0.3f, 0, CheckCollisionPointRec(mousePoint, btnClear) ? btnClearHover : btnClearNormal);
        DrawText("CLEAR", btnClear.x + 25, btnClear.y + 13, 18, WHITE);

        DrawRectangleRounded(btnSolve, 0.3f, 0, CheckCollisionPointRec(mousePoint, btnSolve) ? btnSolveHover : btnSolveNormal);
        DrawText("SOLVE", btnSolve.x + 23, btnSolve.y + 13, 18, WHITE);

        int msgWidth = MeasureText(app.statusMessage.c_str(), 16);
        DrawText(app.statusMessage.c_str(), (screenWidth - msgWidth) / 2, screenHeight - 40, 16, app.isSolved ? DARKGREEN : RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}