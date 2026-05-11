#include "solver.h"

bool solveLine(std::vector<Cell*>& line, int target) {
    int n = line.size();
    std::vector<int> validMasks;

    for (int mask = 0; mask < (1 << n); ++mask) {
        int currentSum = 0;
        bool possible = true;

        for (int i = 0; i < n; ++i) {
            bool isSelected = (mask >> i) & 1;
            
            if (line[i]->status == KEEP && !isSelected) { possible = false; break; }
            if (line[i]->status == CELL_DELETE && isSelected) { possible = false; break; }
            
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
            line[i]->status = CELL_DELETE;
            changed = true;
        }
    }
    return changed;
}

bool SolveSumplete(const std::vector<std::vector<int>>& gridValues, 
                   const std::vector<int>& rowTargets, 
                   const std::vector<int>& colTargets,
                   std::vector<std::vector<Cell>>& resultBoard) {
    
    int size = gridValues.size();
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            resultBoard[i][j].value = gridValues[i][j];
            resultBoard[i][j].status = UNKNOWN;
            if (resultBoard[i][j].value > rowTargets[i] || resultBoard[i][j].value > colTargets[j]) {
                resultBoard[i][j].status = CELL_DELETE;
            }
        }
    }

    bool changed = true;
    while (changed) {
        changed = false;

        for (int i = 0; i < size; ++i) {
            std::vector<Cell*> row;
            for (int j = 0; j < size; ++j) row.push_back(&resultBoard[i][j]);
            if (solveLine(row, rowTargets[i])) changed = true;
        }

        for (int j = 0; j < size; ++j) {
            std::vector<Cell*> col;
            for (int i = 0; i < size; ++i) col.push_back(&resultBoard[i][j]);
            if (solveLine(col, colTargets[j])) changed = true;
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (resultBoard[i][j].status == UNKNOWN) return false; 
        }
    }
    return true;
}