#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

enum Status { UNKNOWN, KEEP, DELETE };

const int SIZE = 9;

struct Cell {
    int value;
    Status status = UNKNOWN;
};

int gridValues[SIZE][SIZE] = {
    {18,  16,   6, -15,  -1,  -7,   3, -13,  15},
    {18,  13,   9,  12, -10, -16,  19,   4,   8},
    {-13, -8,   4,  18, -10,  19,  -4,   9, -10},
    { 1,  16, -15,   7,   8,  18,  -6,  12,   9},
    { 9, -10, -12, -18,   3,  16,  14,  14, -14},
    {15, -19,   5, -15,  11,  -3,   7,  -8, -12},
    {-4,  -8,  19,  -9,   4,  -3,  -9,   3, -11},
    {18,  13,   3,  19,   5, -15, -14, -14, -12},
    {13, -18,   8,  -5,  -8,  19, -11,  14, -16}
};

int rowTargets[SIZE] = {28, 3, 15, 41, 9, -2, -22, 17, -15};
int colTargets[SIZE] = {48, -2, 22, 7, 0, 15, -5, 25, -36};

Cell board[SIZE][SIZE];

bool solveLine(vector<Cell*> line, int target) {
    int n = line.size();
    vector<int> validMasks;

    for (int mask = 0; mask < (1 << n); ++mask) {
        int currentSum = 0;
        bool possible = true;

        for (int i = 0; i < n; ++i) {
            bool isSelected = (mask >> i) & 1;
            
            if (line[i]->status == KEEP && !isSelected) { possible = false; break; }
            if (line[i]->status == DELETE && isSelected) { possible = false; break; }
            
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
            line[i]->status = DELETE;
            changed = true;
        }
    }
    return changed;
}

bool isFull() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j].status == UNKNOWN) return false;
    return true;
}

// Cek apakah kondisi board saat ini masih valid (tidak melanggar target)
bool isValid() {
    // Cek baris & kolom: apakah ada yang mustahil mencapai target?
    // (Dalam implementasi ini, kita cukup cek apakah solveLine mengembalikan false)
    return true; 
}

bool solveDFS() {
    // 1. Jalankan logika deduksi sampai mentok
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < SIZE; i++) {
            vector<Cell*> row;
            for (int j = 0; j < SIZE; j++) row.push_back(&board[i][j]);
            if (solveLine(row, rowTargets[i])) changed = true;
        }
        for (int j = 0; j < SIZE; j++) {
            vector<Cell*> col;
            for (int i = 0; i < SIZE; i++) col.push_back(&board[i][j]);
            if (solveLine(col, colTargets[j])) changed = true;
        }
    }

    if (isFull()) return true; // Selesai!

    // 2. Cari sel pertama yang masih UNKNOWN untuk ditebak
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j].status == UNKNOWN) {
                // Simpan state board saat ini untuk restore jika tebakan salah
                Cell backup[SIZE][SIZE];
                for(int r=0; r<SIZE; r++) for(int c=0; c<SIZE; c++) backup[r][c] = board[r][c];

                // Tebakan 1: Set sebagai KEEP
                board[i][j].status = KEEP;
                if (solveDFS()) return true;

                // Jika salah, Restore dan Tebakan 2: Set sebagai DELETE
                for(int r=0; r<SIZE; r++) for(int c=0; c<SIZE; c++) board[r][c] = backup[r][c];
                board[i][j].status = DELETE;
                if (solveDFS()) return true;

                return false; // Jalur ini buntu
            }
        }
    }
    return false;
}

void displayBoard() {
    cout << "\nKondisi Grid Saat Ini (K = Keep, X = Delete, . = Unknown):\n";
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cout << board[i][j].value;
            if (board[i][j].status == KEEP) cout << "K ";
            else if (board[i][j].status == DELETE) cout << "X ";
            else cout << ". ";
        }
        cout << "| " << rowTargets[i] << endl;
    }
}

// int main() {
//     // for (int i = 0; i < SIZE; ++i) {
//     //     for (int j = 0; j < SIZE; ++j) {
//     //         board[i][j].value = gridValues[i][j];
//     //         if (board[i][j].value > rowTargets[i] || board[i][j].value > colTargets[j]) {
//     //             board[i][j].status = DELETE;
//     //         }
//     //     }
//     // }

//     bool changed = true;
//     while (changed) {
//         changed = false;

//         for (int i = 0; i < SIZE; ++i) {
//             vector<Cell*> row;
//             for (int j = 0; j < SIZE; ++j) row.push_back(&board[i][j]);
//             if (solveLine(row, rowTargets[i])) changed = true;
//         }

//         for (int j = 0; j < SIZE; ++j) {
//             vector<Cell*> col;
//             for (int i = 0; i < SIZE; ++i) col.push_back(&board[i][j]);
//             if (solveLine(col, colTargets[j])) changed = true;
//         }
//     }

//     displayBoard();

//     return 0;
// }

// int main() {
//     // 1. INISIALISASI DATA (Wajib ada!)
//     for (int i = 0; i < SIZE; ++i) {
//         for (int j = 0; j < SIZE; ++j) {
//             board[i][j].value = gridValues[i][j]; // Pindahkan angka ke board
//             board[i][j].status = UNKNOWN;        // Pastikan status awal unknown
            
//             /* CATATAN: Baris "if value > target" dihapus/dimatikan 
//                karena di level Master ada angka negatif. 
//                Logika bitmasking di solveLine sudah cukup kuat menangani ini.
//             */
//         }
//     }

//     bool changed = true;
//     int iteration = 0;
    
//     while (changed) {
//         changed = false;
//         iteration++;

//         // Cek Baris
//         for (int i = 0; i < SIZE; ++i) {
//             vector<Cell*> row;
//             for (int j = 0; j < SIZE; ++j) row.push_back(&board[i][j]);
//             if (solveLine(row, rowTargets[i])) changed = true;
//         }

//         // Cek Kolom
//         for (int j = 0; j < SIZE; ++j) {
//             vector<Cell*> col;
//             for (int i = 0; i < SIZE; ++i) col.push_back(&board[i][j]);
//             if (solveLine(col, colTargets[j])) changed = true;
//         }
        
//         // Opsional: lihat perkembangan tiap iterasi
//         cout << "Iterasi ke-" << iteration << " selesai..." << endl;
//     }

//     displayBoard();

//     return 0;
// }

int main() {
    // 1. INISIALISASI DATA (Wajib ada!)
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j].value = gridValues[i][j]; // Pindahkan angka ke board
            board[i][j].status = UNKNOWN;        // Pastikan status awal unknown
            
            /* CATATAN: Baris "if value > target" dihapus/dimatikan 
               karena di level Master ada angka negatif. 
               Logika bitmasking di solveLine sudah cukup kuat menangani ini.
            */
        }
    }    

    if (solveDFS()) {
        cout << "PUZZLE BERHASIL DIPECAHKAN!" << endl;
    } else {
        cout << "Tidak ada solusi yang mungkin." << endl;
    }

    displayBoard();
    return 0;
}