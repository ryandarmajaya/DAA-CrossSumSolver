#pragma once
#include <vector>

using namespace std;

enum CellStatus { UNKNOWN, KEEP, CELL_DELETE };

struct Cell {
    int value;
    CellStatus status = UNKNOWN;
};

bool SolveSumplete(const vector<vector<int>>& gridValues, 
                   const vector<int>& rowTargets, 
                   const vector<int>& colTargets,
                   vector<vector<Cell>>& resultBoard);