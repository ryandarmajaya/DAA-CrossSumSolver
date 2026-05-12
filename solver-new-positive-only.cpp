#include <iostream>
#include <vector>
#include "solver.h"

using namespace std;

class PuzzleSolver {
private:
    int rows, cols;
    vector<vector<int>> matrix;
    vector<int> row_targets;
    vector<int> col_targets;
    
    // State Tracking untuk DFS
    vector<vector<bool>> kept; // true jika angka dipertahankan, false jika dihapus
    vector<int> current_row_sum;
    vector<int> current_col_sum;
    vector<int> remaining_row_sum;
    vector<int> remaining_col_sum;

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
        remaining_row_sum.assign(rows, 0);
        remaining_col_sum.assign(cols, 0);

        // Pre-kalkulasi total semua angka per baris dan kolom untuk pruning
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                remaining_row_sum[r] += matrix[r][c];
                remaining_col_sum[c] += matrix[r][c];
            }
        }
    }

    // Fungsi utama DFS
    bool solve(int r = 0, int c = 0) {
        // Base Case: Jika sudah melewati baris terakhir, berarti solusi ditemukan!
        if (r == rows) {
            return true; 
        }

        // Kalkulasi koordinat kotak selanjutnya
        int next_r = r;
        int next_c = c + 1;
        if (next_c == cols) {
            next_r = r + 1;
            next_c = 0;
        }

        int val = matrix[r][c];

        // ---------------------------------------------------------
        // CABANG 1: KITA COBA "PERTAHANKAN" ANGKA INI
        // ---------------------------------------------------------
        // Pruning (Over-shoot check): Pastikan kalau ditambahkan, tidak melebihi target
        if (current_row_sum[r] + val <= row_targets[r] && 
            current_col_sum[c] + val <= col_targets[c]) {
            
            // Lakukan langkah (State Update)
            kept[r][c] = true;
            current_row_sum[r] += val;
            current_col_sum[c] += val;
            remaining_row_sum[r] -= val;
            remaining_col_sum[c] -= val;

            // Eksplorasi ke dalam (Depth-First)
            if (solve(next_r, next_c)) return true; // Jika ketemu solusi, return true sampai atas

            // BACKTRACKING: Mundur (Undo state) karena ternyata jalurnya salah
            kept[r][c] = false;
            current_row_sum[r] -= val;
            current_col_sum[c] -= val;
            remaining_row_sum[r] += val;
            remaining_col_sum[c] += val;
        }

        // ---------------------------------------------------------
        // CABANG 2: KITA COBA "HAPUS" ANGKA INI
        // ---------------------------------------------------------
        // Pruning (Under-shoot check): Pastikan kalau dihapus, sisa angka di depannya masih cukup untuk mencapai target
        if (current_row_sum[r] + (remaining_row_sum[r] - val) >= row_targets[r] &&
            current_col_sum[c] + (remaining_col_sum[c] - val) >= col_targets[c]) {
            
            // Lakukan langkah (State Update)
            kept[r][c] = false;
            remaining_row_sum[r] -= val;
            remaining_col_sum[c] -= val;

            // Eksplorasi ke dalam (Depth-First)
            if (solve(next_r, next_c)) return true;

            // BACKTRACKING: Mundur (Undo state)
            remaining_row_sum[r] += val;
            remaining_col_sum[c] += val;
        }

        // Jika kedua cabang (Pertahankan/Hapus) tidak ada yang menghasilkan True, 
        // berarti keputusan di kotak-kotak sebelumnya ada yang salah.
        return false; 
    }

    // Fungsi pembantu untuk mengeksekusi dan mencetak hasil
    void printSolution() {
        if (solve()) {
            cout << "Solusi Ditemukan!" << endl;
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    if (kept[r][c]) {
                        cout << matrix[r][c] << "\t";
                    } else {
                        cout << "X\t"; // X menandakan angka dihapus
                    }
                }
                cout << endl;
            }
        } else {
            cout << "Puzzle ini tidak memiliki solusi." << endl;
        }
    }
};

int main() {
    // Contoh grid kecil 3x3 untuk demonstrasi
    // vector<vector<int>> grid = {
    //     {5, 9, 2},
    //     {9, 8, 2},
    //     {1, 7, 5}
    // };
    
    // // Header/Target
    // vector<int> row_t = {14, 10, 6}; // Asal tebak target baris untuk contoh
    // vector<int> col_t = {6, 17, 7};  // Asal tebak target kolom untuk contoh

    vector<vector<int>> grid = {
        {5, 9, 2, 7, 2, 7},
        {9, 8, 2, 2, 2, 7},
        {1, 7, 5, 6, 7, 5},
        {2, 9, 3, 8, 2, 1},
        {9, 1, 2, 7, 3, 1},
        {5, 8, 7, 2, 5, 5}
    };
    
    // Header/Target
    vector<int> row_t = {12, 19, 18, 14, 14, 20}; 
    vector<int> col_t = {23, 32, 5, 17, 10, 10};

    // vector<vector<int>> grid = {
    //     {-10, -17, -14,  19,  15, -11,   1,  12, -15},
    //     { -9,  -8,   8, -15,  -8,  19,  10,  12, -13},
    //     {  1,  15, -11,  -8,   2,   8,  -3,  17,   2},
    //     {-18,   7,  11,  -3, -13,  -5,  10,  -7,  12},
    //     { -8,  -5, -10,  -3, -11, -13,  -5,   2,  -1},
    //     { 19, -14, -14,   3,   5,  -9,  17, -19,  15},
    //     { 11,  -9,  15,  -2,   9,  -8,  16,   3,  10},
    //     {  9,  -8,   9,   7,  -4,   1, -15,   3,  13},
    //     { -9, -14,   4,   2,  -7,   8, -16,   1, -17}
    // };
    
    // // Header/Target baris (sebelah kanan)
    // vector<int> row_t = {6, -8, 0, -16, -22, -24, 15, 30, -45}; 
    
    // // Header/Target kolom (sebelah bawah)
    // vector<int> col_t = {-44, -45, -7, 6, 2, -4, -5, 6, 27};

    PuzzleSolver solver(grid, row_t, col_t);
    solver.printSolution();

    return 0;
}