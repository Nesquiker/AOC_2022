#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <bitset>
#include <climits>

const int REAL = 120;
const int TEST = 6;

struct board {
    std::vector<std::bitset<REAL>> left;
    std::vector<std::bitset<REAL>> right;
    std::vector<std::bitset<REAL>> up;
    std::vector<std::bitset<REAL>> down;
};

board readBoard (const std::string& path) {
    std::ifstream data(path);
    std::string line;
    std::getline(data,line);
    board b;
    int row = 0;
    for (line; std::getline(data, line);) {
        std::bitset<REAL> left;
        std::bitset<REAL> right;
        std::bitset<REAL> up;
        std::bitset<REAL> down;
        for (int i = 0; i < line.size(); i++){
            const char c = line[i];
            if (c != '#') {
                if (c == '<') {
                    left[i - 1] = true;
                } else if (c == '>') {
                    right[i - 1] = true;
                } else if (c == '^') {
                    up[i - 1] = true;
                } else if (c == 'v') {
                    down[i - 1] = true;
                }
            }
        }
        b.left.emplace_back(left);
        b.right.emplace_back(right);
        b.up.emplace_back(up);
        b.down.emplace_back(down);
        row++;
    }
    b.left.pop_back();
    b.right.pop_back();
    b.up.pop_back();
    b.down.pop_back();
    return b;
}

std::vector<std::vector<int>> makeDistanceGrid (const int rows, const int cols) {
    std::vector<std::vector<int>> output;
    for (int r = 0; r < rows; r++) {
        std::vector<int> distance_to_end;
        for (int c = 0; c < cols; c++) {
            distance_to_end.emplace_back(rows + cols - c - r - 1);
        }
        output.emplace_back(distance_to_end);
    }
    return output;
}

std::vector<std::vector<std::bitset<600>>> makeRowBestValues (const int rows, const int cols) {
    std::vector<std::vector<std::bitset<600>>> output;
    std::bitset<600> d;
    for (int r = 0; r < rows; r++) {
        std::vector<std::bitset<600>> temp;
        for (int c = 0; c < cols; c++) {
            temp.emplace_back(d);
        }
        output.emplace_back(temp);
    }
    return output;
}


board incrementBoard(board b) {
    const int rows = (int)b.left.size();
    const int cols = (int)b.left[0].size();
    // left arrows
    int move_arrow = 0;
    for (int r = 0; r < rows; r++) {
        move_arrow = 0;
        for (int c = cols - 1; c >= 0; c--) {
            if (b.left[r][c]) {
                move_arrow++;
                if (move_arrow == 2) {
                    move_arrow--;
                } else {
                    b.left[r][c] = false;
                }
            } else if (move_arrow == 1) {
                b.left[r][c] = true;
                move_arrow = 0;
            }
        }
        if (move_arrow == 1) {
            b.left[r][cols - 1] = true;
        }
    }
    // right arrows
    for (int r = 0; r < rows; r++) {
        move_arrow = 0;
        for (int c = 0; c < cols; c++) {
            if (b.right[r][c]) {
                move_arrow++;
                if (move_arrow == 2) {
                    move_arrow--;
                } else {
                    b.right[r][c] = false;
                }
            } else if (move_arrow == 1) {
                b.right[r][c] = true;
                move_arrow = 0;
            }
        }
        if (move_arrow == 1) {
            b.right[r][0] = true;
        }
    }
    // up arrows
    for (int c = 0; c < cols; c++) {
        move_arrow = 0;
        for (int r = rows - 1; r >= 0; r--) {
            if (b.up[r][c]) {
                move_arrow++;
                if (move_arrow == 2) {
                    move_arrow--;
                } else {
                    b.up[r][c] = false;
                }
            } else if (move_arrow == 1) {
                b.up[r][c] = true;
                move_arrow = 0;
            }
        }
        if (move_arrow == 1) {
            b.up[rows - 1][c] = true;
        }
    }
    // down arrows
    for (int c = 0; c < cols; c++) {
        move_arrow = 0;
        for (int r = 0; r < rows; r++) {
            if (b.down[r][c]) {
                move_arrow++;
                if (move_arrow == 2) {
                    move_arrow--;
                } else {
                    b.down[r][c] = false;
                }
            } else if (move_arrow == 1) {
                b.down[r][c] = true;
                move_arrow = 0;
            }
        }
        if (move_arrow == 1) {
            b.down[0][c] = true;
        }
    }
    return b;
}

std::vector<board> makeBoardStates (board b, const int maximum_turns) {
    std::vector<board> output {b};
    for (int i = 0; i < maximum_turns; i++) {
        output.emplace_back(incrementBoard(output.back()));
    }
    return output;
}

bool checkBoard (const board& b, const int row, const int col) {
    return (!b.left[row][col] && !b.right[row][col] && !b.up[row][col] && !b.down[row][col]);
}

struct branch {
    int turn = 1;
    int row = 0;
    int col = 0;
};

int part_1 (const std::vector<board>& board_states) {
    const int rows = board_states.back().left.size();
    const int cols = board_states.back().left[0].size();
    std::vector<std::vector<int>> distance_grid = makeDistanceGrid(rows, cols);
    std::vector<std::vector<int>> dirs {{-1, 0}, {0, -1}, {0, 0}, {1, 0}, {0, 1}};
    auto best_vals = makeRowBestValues(rows, cols);
    int best_finish = INT_MAX;
    std::deque<branch> dfs;
    dfs.emplace_back();
    while (!dfs.empty()) {
        branch pos = dfs.back();
        dfs.pop_back();
        pos.turn++;
        const int turn_mod = pos.turn % 600;
        if (pos.turn + distance_grid[pos.row][pos.col] >= best_finish || best_vals[pos.row][pos.col][turn_mod]) {
            continue;
        }
        best_vals[pos.row][pos.col][turn_mod] = true;
        if (pos.row == rows - 1 && pos.col == cols - 1) {
            best_finish = std::min(best_finish, pos.turn);
            continue;
        }
        for (const auto& dir : dirs) {
            branch nb;
            nb.row = pos.row + dir[0];
            nb.col = pos.col + dir[1];
            nb.turn = pos.turn;
            if (nb.row < 0 || nb.row >= rows || nb.col < 0 || nb.col >= cols
            || !checkBoard(board_states[pos.turn], nb.row, nb.col)) {
                continue;
            }
            dfs.emplace_front(nb);
        }
    }
    return best_finish;
}

int part_2 (const std::vector<board>& board_states) {
    const int rows = board_states.back().left.size();
    const int cols = board_states.back().left[0].size();
    std::vector<std::vector<int>> dirs {{-1, 0}, {0, -1}, {0, 0}, {1, 0}, {0, 1}};
    auto best_vals = makeRowBestValues(rows, cols);
    int best_finish = INT_MAX;
    std::deque<branch> dfs;
    dfs.emplace_back();
    while (!dfs.empty()) {
        branch pos = dfs.back();
        dfs.pop_back();
        pos.turn++;
        const int turn_mod = pos.turn % 600;
        if (best_vals[pos.row][pos.col][turn_mod]) {
            continue;
        }
        best_vals[pos.row][pos.col][turn_mod] = true;
        if (pos.row == rows - 1 && pos.col == cols - 1) {
            best_finish = std::min(best_finish, pos.turn);
            break;
        }
        for (const auto& dir : dirs) {
            branch nb;
            nb.row = pos.row + dir[0];
            nb.col = pos.col + dir[1];
            nb.turn = pos.turn;
            if (nb.row < 0 || nb.row >= rows || nb.col < 0 || nb.col >= cols
                || !checkBoard(board_states[pos.turn], nb.row, nb.col)) {
                continue;
            }
            dfs.emplace_front(nb);
        }
    }
    const int there = best_finish;
    best_vals = makeRowBestValues(rows, cols);
    best_finish = INT_MAX;
    dfs.clear();
    std::deque<branch> starters;
    int starting_points = there + 50;
    for (int s = there + 1; s < starting_points; s++) {
        if (checkBoard(board_states[s], rows - 1, cols - 1)) {
            branch nb;
            nb.row = rows - 1;
            nb.col = cols - 1;
            nb.turn = s;
            starters.emplace_front(nb);
        }
    }
    dfs.emplace_back(starters.back());
    starters.pop_back();
    while (!dfs.empty() || !starters.empty()) {
        if (dfs.empty()) {
            dfs.emplace_front(starters.back());
            starters.pop_back();
        }
        branch pos = dfs.back();
        dfs.pop_back();
        pos.turn++;
        if (!starters.empty() && starters.back().turn == pos.turn) {
            dfs.emplace_front(starters.back());
            starters.pop_back();
        }
        const int turn_mod = pos.turn % 600;
        if (best_vals[pos.row][pos.col][turn_mod]) {
            continue;
        }
        best_vals[pos.row][pos.col][turn_mod] = true;
        if (pos.row == 0 && pos.col == 0) {
            best_finish = std::min(best_finish, pos.turn);
            break;
        }
        for (const auto& dir : dirs) {
            branch nb;
            nb.row = pos.row + dir[0];
            nb.col = pos.col + dir[1];
            nb.turn = pos.turn;
            if (nb.row < 0 || nb.row >= rows || nb.col < 0 || nb.col >= cols
                || !checkBoard(board_states[pos.turn], nb.row, nb.col)) {
                continue;
            }
            dfs.emplace_front(nb);
        }
    }
    const int back = best_finish;
    best_vals = makeRowBestValues(rows, cols);
    best_finish = INT_MAX;
    dfs.clear();
    starters.clear();
    starting_points = back + 50;
    for (int s = back + 1; s < starting_points; s++) {
        if (checkBoard(board_states[s],  0, 0)) {
            branch nb;
            nb.row = 0;
            nb.col = 0;
            nb.turn = s;
            starters.emplace_front(nb);
        }
    }
    dfs.emplace_back(starters.back());
    starters.pop_back();
    while (!dfs.empty() || !starters.empty()) {
        if (dfs.empty()) {
            dfs.emplace_front(starters.back());
            starters.pop_back();
        }
        branch pos = dfs.back();
        dfs.pop_back();
        pos.turn++;
        if (!starters.empty() && starters.back().turn == pos.turn) {
            dfs.emplace_front(starters.back());
            starters.pop_back();
        }
        const int turn_mod = pos.turn % 600;
        if (best_vals[pos.row][pos.col][turn_mod]) {
            continue;
        }
        best_vals[pos.row][pos.col][turn_mod] = true;
        if (pos.row == rows - 1 && pos.col == cols - 1) {
            best_finish = std::min(best_finish, pos.turn);
            break;
        }
        for (const auto& dir : dirs) {
            branch nb;
            nb.row = pos.row + dir[0];
            nb.col = pos.col + dir[1];
            nb.turn = pos.turn;
            if (nb.row < 0 || nb.row >= rows || nb.col < 0 || nb.col >= cols
                || !checkBoard(board_states[pos.turn], nb.row, nb.col)) {
                continue;
            }
            dfs.emplace_front(nb);
        }
    }
    return best_finish;
}

int main() {
    const int maximum_turns = 3000;
//    const std::string test_path = "../test.txt";
//    board test_board = readBoard(test_path);
//    auto test_board_states = makeBoardStates(test_board, maximum_turns);
//    std::cout << part_1 (test_board_states) << "\n";
//    std::cout << part_2 (test_board_states) << "\n";

    const std::string real_path = "../input.txt";
    board real_board = readBoard(real_path);
    auto real_board_states = makeBoardStates(real_board, maximum_turns);
    std::cout << part_1 (real_board_states) << "\n";
    std::cout << part_2 (real_board_states) << "\n";

    return 0;

}