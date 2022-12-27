#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

std::vector<std::vector<std::bitset<30>>> createGrid(const std::string& file_path) {
    std::vector<std::bitset<30>> twod_grid (30);
    std::vector<std::vector<std::bitset<30>>> grid (30, twod_grid);
    std::ifstream data(file_path);
    for (std::string line; std::getline(data, line);) {
        std::vector<int> pos;
        std::string buffer;
        for (char c : line) {
            if (isdigit(c)) {
                buffer.push_back(c);
            } else {
                pos.emplace_back(std::stoi(buffer) + 1);
                buffer = "";
            }
        }
        pos.emplace_back(std::stoi(buffer) + 1);
        grid[pos[0]][pos[1]][pos[2]] = true;
    }
    return grid;
}


std::vector<std::vector<std::bitset<30>>> createFillGrid(const std::vector<std::vector<std::bitset<30>>>& point_grid) {
    std::vector<std::bitset<30>> twod_grid (30);
    std::vector<std::vector<std::bitset<30>>> grid (30, twod_grid);
    grid[0][0][0] = true;
    std::vector<std::vector<int>> directions {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    std::vector<std::vector<int>> pos {{0, 0, 0}};
    const int grid_bound = point_grid.size();
    while (!pos.empty()) {
        auto point = pos.back();
        pos.pop_back();
        for (auto dir : directions) {
            bool is_not_a_dir = true;
            auto this_point = point;
            for (int i = 0; i < dir.size(); i++) {
                this_point[i] += dir[i];
                if (this_point[i] < 0 || this_point[i] >= grid_bound) {
                    is_not_a_dir = false;
                    break;
                }
            }
            if (!is_not_a_dir) {
                continue;
            }
            if (grid[this_point[0]][this_point[1]][this_point[2]]) {
                continue;
            }
            if (!point_grid[this_point[0]][this_point[1]][this_point[2]]) {
                grid[this_point[0]][this_point[1]][this_point[2]] = true;
                pos.emplace_back(this_point);
            }
        }
    }
    return grid;
}

std::vector<std::vector<int>> point_list (const std::string& file_path) {
    std::ifstream data(file_path);
    std::vector<std::vector<int>> output;
    for (std::string line; std::getline(data, line);) {
        std::vector<int> pos;
        std::string buffer;
        for (char c : line) {
            if (isdigit(c)) {
                buffer.push_back(c);
            } else {
                pos.emplace_back(std::stoi(buffer) + 1);
                buffer = "";
            }
        }
        pos.emplace_back(std::stoi(buffer) + 1);
        output.emplace_back(pos);
    }
    return output;
}

int part_1 (const std::string& file_path) {
    auto grid = createGrid(file_path);
    auto points = point_list(file_path);
    const int grid_bound = (int)grid.size();
    std::vector<std::vector<int>> directions {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    int surface_area = 0;
    for (auto point : points) {
        for (auto dir : directions) {
            bool is_not_a_dir = true;
            auto this_point = point;
            for (int i = 0; i < dir.size(); i++) {
                this_point[i] += dir[i];
                if (this_point[i] < 0 || this_point[i] >= grid_bound) {
                    is_not_a_dir = false;
                    break;
                }
            }
            if (!is_not_a_dir) {
                continue;
            }
            if (!grid[this_point[0]][this_point[1]][this_point[2]]) {
                surface_area++;
            }
        }
    }
    return surface_area;
}

int part_2 (const std::string& file_path) {
    auto grid = createGrid(file_path);
    auto points = point_list(file_path);
    auto fill_grid = createFillGrid(grid);
    const int grid_bound = (int)grid.size();
    std::vector<std::vector<int>> directions {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    int surface_area = 0;
    for (auto point : points) {
        for (auto dir : directions) {
            bool is_not_a_dir = true;
            auto this_point = point;
            for (int i = 0; i < dir.size(); i++) {
                this_point[i] += dir[i];
                if (this_point[i] < 0 || this_point[i] >= grid_bound) {
                    is_not_a_dir = false;
                    break;
                }
            }
            if (!is_not_a_dir) {
                continue;
            }
            if (!fill_grid[this_point[0]][this_point[1]][this_point[2]]) {
                continue;
            }
            if (!grid[this_point[0]][this_point[1]][this_point[2]]) {
                surface_area++;
            }
        }
    }
    return surface_area;
}


int main() {
    const std::string real_path = "../input.txt";
    const std::string test_path = "../test.txt";
//    int test_output_part_1 = part_1 (test_path);
//    int real_output_part_1 = part_1 (real_path);
//    std::cout << test_output_part_1 << "\n";
//    std::cout << real_output_part_1 << "\n";
    int test_output_part_2 = part_2 (test_path);
    int real_output_part_2 = part_2 (real_path);
    std::cout << test_output_part_2 << "\n";
    std::cout << real_output_part_2 << "\n";

    return 0;

}