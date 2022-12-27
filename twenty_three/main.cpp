#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <deque>
#include <climits>


struct elf {
    int row = 0;
    int col = 0;
    std::vector<char> directions = {'N', 'S', 'W', 'E'};
    int n_row = 0;
    int n_col = 0;
    bool did_move = false;
    int turn = 0;
};

struct elf_data {
    std::vector<std::bitset<10000>> elf_positions;
    std::vector<std::bitset<10000>> next_moves;
    std::vector<std::bitset<10000>> conflicting_moves;
    int number_moved = 0;
    std::deque<elf> elves;
};

elf_data readElfPositions (const std::string& path) {
    std::ifstream data(path);
    std::vector<std::bitset<10000>> elf_positions;
    const int size = 10000;
    elf_data elf_data;

    for (std::string line; std::getline(data, line);) {
        std::bitset<10000> elf_row;
        if (elf_positions.empty()) {
            for (int i = 0; i < size / 2; i++) {
                elf_positions.emplace_back(elf_row);
            }
        }
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == '#') {
                const int row = (int)elf_positions.size();
                elf_row[i + size / 2] = true;
                elf cur;
                cur.row = row;
                cur.col = i + size / 2;
                elf_data.elves.emplace_back(cur);
            }
        }
        elf_positions.emplace_back(elf_row);
    }
    for (int i = 0; i < size; i++) {
        std::bitset<10000> elf_row_2;
        if (elf_positions.size() - 1 <= i) {
            elf_positions.emplace_back(elf_row_2);

        }
        elf_data.next_moves.emplace_back(elf_row_2);
        elf_data.conflicting_moves.emplace_back(elf_row_2);
    }
    elf_data.elf_positions = elf_positions;
    return elf_data;
}

void moveElf (elf& current, elf_data& data) {
    bool conflict = false;
    for (char dir : current.directions) {
        std::vector<std::vector<int>> checks;
        if (dir == 'N') {
            checks = {{-1, -1}, {-1, 0}, {-1, 1}};
        } else if (dir == 'S') {
            checks = {{1, -1}, {1, 0}, {1, 1}};
        } else if (dir == 'W') {
            checks = {{1, -1}, {0, -1}, {-1, -1}};
        } else if (dir == 'E') {
            checks = {{1, 1}, {0, 1}, {-1, 1}};
        }
        for (const auto& check : checks) {
            if (data.elf_positions[current.row + check[0]][current.col + check[1]]) {
                conflict = true;
                break;
            }
        }
        if (conflict) {
            break;
        }
    }
    if (!conflict) {
        data.number_moved++;
        char rot = current.directions.front();
        current.directions.erase(current.directions.begin());
        current.directions.emplace_back(rot);
        return;
    }
    for (char dir : current.directions) {
        std::vector<std::vector<int>> checks;
        if (dir == 'N') {
            checks = {{-1, -1}, {-1, 0}, {-1, 1}};
        } else if (dir == 'S') {
            checks = {{1, -1}, {1, 0}, {1, 1}};
        } else if (dir == 'W') {
            checks = {{1, -1}, {0, -1}, {-1, -1}};
        } else if (dir == 'E') {
            checks = {{1, 1}, {0, 1}, {-1, 1}};
        }
        bool temp_conflict = false;
        for (const auto& check : checks) {
            if (data.elf_positions[current.row + check[0]][current.col + check[1]]) {
                temp_conflict = true;
                break;
            }
        }
        if (temp_conflict) {
            continue;
        }
        const int n_row = current.row + checks[1].front();
        const int n_col = current.col + checks[1].back();
        if (data.next_moves[n_row][n_col]) {
            data.conflicting_moves[n_row][n_col] = true;
        } else {
            data.next_moves[n_row][n_col] = true;
        }
        current.n_row = n_row;
        current.n_col = n_col;
        current.did_move = true;
        break;
    }
    char rot = current.directions.front();
    current.directions.erase(current.directions.begin());
    current.directions.emplace_back(rot);
}

void finalizeMoves (elf_data& data) {
    for (auto& elf : data.elves) {
        if (!elf.did_move) {
            continue;
        }
        if (!data.conflicting_moves[elf.n_row][elf.n_col]) {
            data.elf_positions[elf.row][elf.col] = false;
            elf.row = elf.n_row;
            elf.col = elf.n_col;
            data.elf_positions[elf.row][elf.col] = true;
        }
        elf.did_move = false;
    }
}

void resetMoveTrackers (elf_data& data, const int first_row, const int last_row) {
    for (int i = first_row - 3; i < last_row + 3; i++) {
        data.conflicting_moves[i].reset();
        data.next_moves[i].reset();
    }
}


struct boxBounds {
    int top = INT_MAX;
    int bottom = INT_MIN;
    int left = INT_MAX;
    int right = INT_MIN;
    int volume = 0;
    void calcVolume () {
        this -> volume = (this -> right - this -> left + 1) * (this -> bottom - this -> top + 1);
    }
};

boxBounds getBox (const elf_data& data) {
    boxBounds box;
    for (auto& elf : data.elves) {
        box.left = std::min(box.left, elf.col);
        box.right = std::max(box.right, elf.col);
        box.top = std::min(box.top, elf.row);
        box.bottom = std::max(box.bottom, elf.row);
    }
    box.calcVolume();
    return box;
}

int part_1 (elf_data data) {
    const int elf_population = data.elves.size();
    const int total_turn = 10;
    for (int i = 0; i < total_turn; i++) {
        for (int e = 0; e < elf_population; e++) {
            elf current = data.elves.back();
            data.elves.pop_back();
            moveElf(current, data);
            data.elves.emplace_front(current);
        }
        finalizeMoves(data);
        boxBounds box = getBox(data);
        for (int r = box.top; r <= box.bottom; r++) {
            for (int c = box.left; c <= box.right; c++) {
                if (data.elf_positions[r][c]) {
                    std::cout << "#";
                } else {
                    std::cout << ".";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        resetMoveTrackers(data, box.top, box.bottom);
    }
    boxBounds box = getBox(data);
    return box.volume - elf_population;
}

int part_2 (elf_data data) {
    const int elf_population = data.elves.size();
    const int total_turn = 10000;
    for (int i = 0; i < total_turn; i++) {
        for (int e = 0; e < elf_population; e++) {
            elf current = data.elves.back();
            data.elves.pop_back();
            moveElf(current, data);
            data.elves.emplace_front(current);
        }
        finalizeMoves(data);
        boxBounds box = getBox(data);
        resetMoveTrackers(data, box.top, box.bottom);
        if (data.number_moved == elf_population) {
            return i + 1;
        }
        data.number_moved = 0;
    }
    return -1;
}


int main () {
    const std::string test_path = "../test.txt";
    const std::string real_path = "../input.txt";
    auto test_elves = readElfPositions(test_path);
    auto real_elves = readElfPositions(real_path);
    std::cout << part_2 (test_elves) << "\n";
    std::cout << part_2 (real_elves) << "\n";


}