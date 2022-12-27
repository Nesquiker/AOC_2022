#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <array>
#include <complex>


struct cube_side {
    int cube_number = 0;
    int left_bound = 0;
    int upper_bound = 0;
    int right_bound = 0;
    int lower_bound = 0;
    std::vector<int> cube_edges;
};

struct board {
    std::vector<std::bitset<150>> wall_locations;
    std::vector<std::pair<int, int>> path_ranges;
    std::vector<std::pair<int, int>> vertical_ranges;
    std::vector<std::string> directions;
    std::vector<cube_side> cube;
    int side_size = 0;
};

board makeBoard (const std::string& path) {
    std::ifstream data(path);
    std::string line;
    std::getline(data,line);
    board output_board;
    int count_valid = 0;
    while (!line.empty()) {
        std::bitset<150> wall_locs;
        int path_start = -1;
        int path_end = (int)line.size() - 1;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != ' ') {
                count_valid++;
                if (path_start == -1) {
                    path_start = i;
                }
                if (line[i] == '#') {
                    wall_locs[i] = true;
                }
            }
        }
        output_board.wall_locations.emplace_back(wall_locs);
        output_board.path_ranges.emplace_back(path_start, path_end);
        std::getline(data,line);
    }
    const int side_size = (int)std::pow(count_valid / 6, .5);
    output_board.side_size = side_size;
    std::getline(data, line);
    std::string buffer;
    bool last_was_num = false;
    for (char c : line) {
        if (isalpha(c)) {
            if (last_was_num) {
                output_board.directions.emplace_back(buffer);
                buffer = "";
            }
            buffer.push_back(c);
            output_board.directions.emplace_back(buffer);
            buffer = "";
        } else {
            buffer.push_back(c);
            last_was_num = true;
        }
    }
    if (!buffer.empty()) {
        output_board.directions.emplace_back(buffer);
    }
    std::vector<cube_side> cube;
    for (int i = 0; i < 6; i++) {
        cube.emplace_back();
        cube.back().cube_number = i;
    }
    cube[0].cube_edges = {1, 2, 3, 4};
    cube[1].cube_edges = {5, 2, 0, 4};
    cube[2].cube_edges = {1, 5, 3, 0};
    cube[3].cube_edges = {0, 2, 5, 4};
    cube[4].cube_edges = {1, 0, 3, 5};
    cube[5].cube_edges = {1, 4, 3, 2};
    std::vector<std::pair<int, int>> directions {{0, -side_size}, {-side_size, 0},
                                                 {0, side_size}, {side_size, 0}};
    std::vector<std::vector<int>> cube_traveller {{0, 0, 0, 0, 1}};
    int cube_count = 0;
    const int max_row = (int)output_board.path_ranges.size();
    while (cube_count < 6) {
        std::vector<int> current = cube_traveller.back();
        cube_traveller.pop_back();

        if (cube[current[2]].right_bound != 0 || current[0] < 0 || current[0] >= max_row) {
            continue;
        }
        const int left_bound = output_board.path_ranges[current[0]].first;
        const int right_bound = output_board.path_ranges[current[0]].second;
        if (cube_count == 0 && current[1] < left_bound) {
            current[1] += side_size;
            cube_traveller.emplace_back(current);
            continue;
        }

        if (current[1] >= left_bound && current[1] <= right_bound) {
            cube[current[2]].left_bound = current[1];
            cube[current[2]].right_bound = current[1] + side_size - 1;
            cube[current[2]].lower_bound = current[0];
            cube[current[2]].upper_bound = current[0] + side_size - 1;
            cube_count++;
        } else {
            continue;
        }
        const int ref = current[3];
        while (cube[current[2]].cube_edges[ref] != current[4]) {
            const int last_val = cube[current[2]].cube_edges.back();
            cube[current[2]].cube_edges.pop_back();
            cube[current[2]].cube_edges.insert(cube[current[2]].cube_edges.begin(), last_val);
        }
        for (int i = 0; i < 4; i++) {
            std::vector<int> next = current;
            next[0] = current[0] + directions[i].first;
            next[1] = current[1] + directions[i].second;
            next[2] = cube[current[2]].cube_edges[i];
            next[3] = (i + 2) % 4;
            next[4] = current[2];
            cube_traveller.emplace_back(next);
        }
    }
    std::vector<std::pair<int, int>> vertical_range (150, {-1, -1});
    for (int r = 0; r < output_board.path_ranges.size(); r++) {
        const int left_bound = output_board.path_ranges[r].first;
        const int right_bound = output_board.path_ranges[r].second;
        for (int c = left_bound; c <= right_bound; c++) {
            if (vertical_range[c].first == -1) {
                vertical_range[c].first = r;
            }
            vertical_range[c].second = r;
        }
    }
    output_board.cube = cube;
    output_board.vertical_ranges = vertical_range;
    return output_board;
}


int calcAnswer (const std::pair<int, int>& location, const int facing) {
    const int multiplier = 1000;
    const int row = location.first + 1;
    const int column = location.second + 1;
    return multiplier * row + column * 4 + facing;

}

int part_1 (const board& b) {
    int facing = 0;
    std::vector<std::pair<int, int>> directions {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::pair<int, int> location {0, b.path_ranges.front().first};
    for (std::string move : b.directions) {
        if (isalpha(move[0])) {
            if (move[0] == 'R') {
                facing++;
                facing %= 4;
            } else {
                facing--;
                if (facing < 0) {
                    facing = 3;
                }
            }
            continue;
        }
        int number_moves = std::stoi(move);

        while (number_moves > 0) {
            number_moves--;
            const std::pair<int, int> previous = location;
            location.first += directions[facing].first;
            location.second += directions[facing].second;
            const int left_bound = b.path_ranges[location.first].first;
            const int right_bound = b.path_ranges[location.first].second;
            const int upper_bound = b.vertical_ranges[location.second].first;
            const int lower_bound = b.vertical_ranges[location.second].second;
            if (facing == 0 && right_bound < location.second) {
                location.second = left_bound;
            } else if (facing == 2 && left_bound > location.second) {
                location.second = right_bound;
            } else if (facing == 1 && lower_bound < location.first) {
                location.first = upper_bound;
            } else if (facing == 3 && upper_bound > location.first) {
                location.first = lower_bound;
            }
            if (b.wall_locations[location.first][location.second]) {
                location = previous;
                break;
            }
        }
    }
    return calcAnswer(location, facing);
}

int determineCubeSide (const board& b, const std::pair<int, int>& location) {
    const int row = location.first;
    const int col = location.second;
    for (auto& side : b.cube) {
        if (row >= side.lower_bound && row <= side.upper_bound && col >= side.left_bound && col <= side.right_bound) {
            return side.cube_number;
        }
    }
    return -1;
}

std::pair<int, int> convertLocation (const board& b, const std::pair<int, int>& location,
                                     int& facing, const int next_side, const int current_side) {
    int row = location.first;
    int col = location.second;
    const int left = b.cube[current_side].left_bound;
    const int right = b.cube[current_side].right_bound;
    const int lower = b.cube[current_side].lower_bound;
    const int upper = b.cube[current_side].upper_bound;
    const int nright = b.cube[next_side].right_bound;
    const int nleft = b.cube[next_side].left_bound;
    const int nupper = b.cube[next_side].upper_bound;
    const int nlower = b.cube[next_side].lower_bound;
    int x = 0;
    if (facing == 0) {
        x = b.side_size - row + lower - 1;
    } else if (facing == 1) {
        x = col - left;
    } else if (facing == 2) {
        x = row - lower;
    } else if (facing == 3) {
        x = b.side_size - col + left - 1;
    }
    const int s_minus_x = b.side_size - x - 1;
    for (;;) {
        if (facing == 0) {
            row = s_minus_x + nlower;
            col = nleft;
        } else if (facing == 1) {
            row = nlower;
            col = x + nleft;
        } else if (facing == 2) {
            row = x + nlower;
            col = nright;
        } else {
            row = nupper;
            col = s_minus_x + nleft;
        }

        if (b.cube[next_side].cube_edges[facing] == current_side) {
            break;
        }
        facing++;
        facing %= 4;
    }
    return {row, col};
}

int part_2 (const board& b) {
    int facing = 0;
    std::vector<std::pair<int, int>> directions {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::pair<int, int> location {0, b.path_ranges.front().first};
    for (std::string move : b.directions) {
        if (isalpha(move[0])) {
            if (move[0] == 'R') {
                facing++;
                facing %= 4;
            } else {
                facing--;
                if (facing < 0) {
                    facing = 3;
                }
            }
            continue;
        }
        int number_moves = std::stoi(move);
        while (number_moves > 0) {
            number_moves--;
            const std::pair<int, int> previous = location;
            const int previous_side = determineCubeSide(b, previous);
            location.first += directions[facing].first;
            location.second += directions[facing].second;
            const int left_bound = b.path_ranges[location.first].first;
            const int right_bound = b.path_ranges[location.first].second;
            const int upper_bound = b.vertical_ranges[location.second].first;
            const int lower_bound = b.vertical_ranges[location.second].second;
            const int prev_facing = facing;
            if (facing == 0 && right_bound < location.second) {
                const int next_side = b.cube[previous_side].cube_edges[2];
                location = convertLocation(b, previous, facing, next_side, previous_side);
            } else if (facing == 2 && left_bound > location.second) {
                const int next_side = b.cube[previous_side].cube_edges[0];
                location = convertLocation(b, previous, facing, next_side, previous_side);
            } else if (facing == 1 && lower_bound < location.first) {
                const int next_side = b.cube[previous_side].cube_edges[3];
                location = convertLocation(b, previous, facing, next_side, previous_side);
            } else if (facing == 3 && upper_bound > location.first) {
                const int next_side = b.cube[previous_side].cube_edges[1];
                location = convertLocation(b, previous, facing, next_side, previous_side);
            }
            if (b.wall_locations[location.first][location.second]) {
                location = previous;
                facing = prev_facing;
                break;
            }

        }
    }
    return calcAnswer(location, facing);
}


int main () {
    const std::string test_path = "../test.txt";
    const std::string real_path = "../input.txt";
    const board test_board = makeBoard(test_path);
    const board real_board = makeBoard(real_path);
    std::cout << part_1(test_board) << "\n";
    std::cout << part_1(real_board) << "\n";
    std::cout << part_2(test_board) << "\n";
    std::cout << part_2(real_board) << "\n";

    return 0;

}