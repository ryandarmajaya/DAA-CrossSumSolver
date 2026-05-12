#include <iostream>
#include <vector>
#include <iomanip>

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
    
    // Dipisah untuk handle angka positif dan negatif
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

    // Fungsi cek apakah status saat ini masih punya harapan untuk mencapai target
    bool isValidPath(int r, int c) {
        if (current_row_sum[r] + rem_pos_row[r] < row_targets[r]) return false;
        if (current_row_sum[r] + rem_neg_row[r] > row_targets[r]) return false;
        if (current_col_sum[c] + rem_pos_col[c] < col_targets[c]) return false;
        if (current_col_sum[c] + rem_neg_col[c] > col_targets[c]) return false;
        return true;
    }

    bool solve(int r = 0, int c = 0) {
        // Base Case: Cek apakah hasil akhir benar-benar sesuai target
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

        // Kurangi potensi sisa karena kotak ini akan kita putuskan
        if (val > 0) {
            rem_pos_row[r] -= val; rem_pos_col[c] -= val;
        } else {
            rem_neg_row[r] -= val; rem_neg_col[c] -= val;
        }

        // ---------------------------------------------------------
        // CABANG 1: "PERTAHANKAN" ANGKA
        // ---------------------------------------------------------
        current_row_sum[r] += val;
        current_col_sum[c] += val;
        kept[r][c] = true;

        if (isValidPath(r, c)) {
            if (solve(next_r, next_c)) return true;
        }

        // Backtracking (Batalkan Pertahankan)
        current_row_sum[r] -= val;
        current_col_sum[c] -= val;
        kept[r][c] = false;

        // ---------------------------------------------------------
        // CABANG 2: "HAPUS" ANGKA
        // ---------------------------------------------------------
        if (isValidPath(r, c)) {
            if (solve(next_r, next_c)) return true;
        }

        // Kembalikan potensi sisa jika kita mundur (Backtrack ke node sebelumnya)
        if (val > 0) {
            rem_pos_row[r] += val; rem_pos_col[c] += val;
        } else {
            rem_neg_row[r] += val; rem_neg_col[c] += val;
        }

        return false; 
    }

    void printSolution() {
        if (solve()) {
            // cout << "Solusi Ditemukan!\n" << string(50, '-') << endl
            cout << "Solusi Ditemukan!" << endl;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    if (kept[r][c]) {
                        cout << setw(4) << matrix[r][c] << " ";
                    } else {
                        cout << setw(4) << "X" << " ";
                    }
                }
                // cout << " | " << row_targets[r] << endl;
                cout << endl;
            }
        } else {
            cout << "Puzzle ini tidak memiliki solusi." << endl;
        }
    }
};

int main() {
    vector<vector<int>> grid = {
        {-10, -17, -14,  19,  15, -11,   1,  12, -15},
        { -9,  -8,   8, -15,  -8,  19,  10,  12, -13},
        {  1,  15, -11,  -8,   2,   8,  -3,  17,   2},
        {-18,   7,  11,  -3, -13,  -5,  10,  -7,  12},
        { -8,  -5, -10,  -3, -11, -13,  -5,   2,  -1},
        { 19, -14, -14,   3,   5,  -9,  17, -19,  15},
        { 11,  -9,  15,  -2,   9,  -8,  16,   3,  10},
        {  9,  -8,   9,   7,  -4,   1, -15,   3,  13},
        { -9, -14,   4,   2,  -7,   8, -16,   1, -17}
    };
    
    vector<int> row_t = {6, -8, 0, -16, -22, -24, 15, 30, -45}; 
    vector<int> col_t = {-44, -45, -7, 6, 2, -4, -5, 6, 27}; 

    PuzzleSolver solver(grid, row_t, col_t);
    solver.printSolution();

    return 0;
}