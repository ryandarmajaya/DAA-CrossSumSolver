#include "solver.h"
#include <vector>

using namespace std;

class PuzzleSolver {
private:
    int rows, cols;
    vector<vector<int>> matrix;
    vector<int> row_targets;
    vector<int> col_targets;
    
    vector<vector<bool>> kept; 
    vector<int> current_row_sum;
    vector<int> current_col_sum;
    
    vector<int> rem_pos_row, rem_pos_col;
    vector<int> rem_neg_row, rem_neg_col;

public:
    PuzzleSolver(vector<vector<int>> mat, vector<int> r_targets, vector<int> c_targets) {
        matrix = mat;
        row_targets = r_targets;
        col_targets = c_targets;
        rows = matrix.size();
        cols = matrix[0].size();

        kept.assign(rows, vector<bool>(cols, false));
        current_row_sum.assign(rows, 0);
        current_col_sum.assign(cols, 0);
        
        rem_pos_row.assign(rows, 0); rem_pos_col.assign(cols, 0);
        rem_neg_row.assign(rows, 0); rem_neg_col.assign(cols, 0);

        // Hitung total sisa positif dan negatif dari awal
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (matrix[r][c] > 0) {
                    rem_pos_row[r] += matrix[r][c];
                    rem_pos_col[c] += matrix[r][c];
                } else {
                    rem_neg_row[r] += matrix[r][c];
                    rem_neg_col[c] += matrix[r][c];
                }
            }
        }
    }

    bool isValidPath(int r, int c) {
        if (current_row_sum[r] + rem_pos_row[r] < row_targets[r]) return false;
        if (current_row_sum[r] + rem_neg_row[r] > row_targets[r]) return false;
        if (current_col_sum[c] + rem_pos_col[c] < col_targets[c]) return false;
        if (current_col_sum[c] + rem_neg_col[c] > col_targets[c]) return false;
        return true;
    }

    bool solve(int r = 0, int c = 0) {
        if (r == rows) {
            for(int i=0; i<rows; i++) if(current_row_sum[i] != row_targets[i]) return false;
            for(int j=0; j<cols; j++) if(current_col_sum[j] != col_targets[j]) return false;
            return true; 
        }

        int next_r = r;
        int next_c = c + 1;
        if (next_c == cols) {
            next_r = r + 1;
            next_c = 0;
        }

        int val = matrix[r][c];

        if (val > 0) {
            rem_pos_row[r] -= val; rem_pos_col[c] -= val;
        } else {
            rem_neg_row[r] -= val; rem_neg_col[c] -= val;
        }

        current_row_sum[r] += val;
        current_col_sum[c] += val;
        kept[r][c] = true;

        if (isValidPath(r, c)) {
            if (solve(next_r, next_c)) return true;
        }

        current_row_sum[r] -= val;
        current_col_sum[c] -= val;
        kept[r][c] = false;

        if (isValidPath(r, c)) {
            if (solve(next_r, next_c)) return true;
        }

        if (val > 0) {
            rem_pos_row[r] += val; rem_pos_col[c] += val;
        } else {
            rem_neg_row[r] += val; rem_neg_col[c] += val;
        }

        return false; 
    }

    const vector<vector<bool>>& getKeptState() const {
        return kept;
    }
};

bool SolveSumplete(const std::vector<std::vector<int>>& gridValues, 
                   const std::vector<int>& rowTargets, 
                   const std::vector<int>& colTargets,
                   std::vector<std::vector<Cell>>& resultBoard) {
    
    if (gridValues.empty() || gridValues[0].empty()) return false;

    PuzzleSolver solver(gridValues, rowTargets, colTargets);
    
    if (solver.solve()) {
        const vector<vector<bool>>& solvedState = solver.getKeptState();
        int rows = gridValues.size();
        int cols = gridValues[0].size();
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                resultBoard[i][j].value = gridValues[i][j];
                resultBoard[i][j].status = solvedState[i][j] ? KEEP : CELL_DELETE;
            }
        }
        return true; 
    }
    
    return false; 
}