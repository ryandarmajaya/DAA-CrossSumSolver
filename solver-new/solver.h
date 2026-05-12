#pragma once
#include <vector>

enum CellStatus { UNKNOWN, KEEP, CELL_DELETE }; 

struct Cell {
    int value;
    CellStatus status = UNKNOWN;
};

bool SolveSumplete(const std::vector<std::vector<int>>& gridValues, 
                   const std::vector<int>& rowTargets, 
                   const std::vector<int>& colTargets,
                   std::vector<std::vector<Cell>>& resultBoard);