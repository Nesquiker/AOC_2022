#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <climits>

auto constructCave (const std::string& f_path) {
    std::vector<std::bitset<300>> cave_structure (1200);
    std::ifstream data(f_path);
    std::vector<std::pair<int, int>> dirs {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int highest_point = 0;
    for (std::string line; std::getline(data, line);) {
        std::vector<std::pair<int, int>> rock_path;
        std::string buffer;
        std::pair<int, int> point;
        for (char c : line) {
            if (isdigit(c)) {
                buffer.push_back(c);
            } else if (c == ',') {
                point.first = std::stoi(buffer);
                buffer = "";
            } else if (c == ' ' and !buffer.empty()) {
                point.second = std::stoi(buffer);
                buffer = "";
                rock_path.emplace_back(point);
            }
        }
        point.second = std::stoi(buffer);
        rock_path.emplace_back(point);
        std::pair<int, int> prev {INT_MAX, INT_MAX};
        for (auto& start : rock_path) {
            if (prev.first == INT_MAX) {
                prev = start;
                continue;
            }
            highest_point = std::max(start.second, highest_point);
            std::pair<int, int> dir;
            if (start.first - prev.first > 0) {
                dir = dirs[0];
            } else if (start.first - prev.first < 0) {
                dir = dirs[2];
            } else if (start.second - prev.second > 0) {
                dir = dirs[1];
            } else {
                dir = dirs[3];
            }
            std::pair<int,int> current = prev;
            cave_structure[current.first][current.second] = true;
            while (current != start) {
                current.first += dir.first;
                current.second += dir.second;
                cave_structure[current.first][current.second] = true;
            }
            prev = start;
        }
    }

    highest_point += 2;
    for (int i = 0; i < 1200; i++) {
        cave_structure[i][highest_point] = true;
    }
    return cave_structure;
}

int part_1 (std::vector<std::bitset<300>> cave) {

    std::vector<std::pair<int, int>> sand_dirs = {{0, 1}, {-1, 1}, {1, 1}};
    const int abyss = 500;
    const std::pair<int, int> sand_source {500, 0};
    std::pair<int, int> c_sand = sand_source;
    int sand_units = 0;
    while (c_sand.second < abyss) {
        bool moved = false;
        for (auto dir : sand_dirs) {
            std::pair<int, int> next = c_sand;
            next.first += dir.first;
            next.second += dir.second;
            if (cave[next.first][next.second]) {
                continue;
            } else {
                moved = true;
                c_sand = next;
                break;
            }
        }
        if (!moved) {
            cave[c_sand.first][c_sand.second] = true;
            c_sand = sand_source;
            sand_units++;
        }
    }
    return sand_units;
}


int part_2 (std::vector<std::bitset<300>> cave) {
    std::vector<std::pair<int, int>> sand_dirs = {{0, 1}, {-1, 1}, {1, 1}};
    const std::pair<int, int> sand_source {500, 0};
    std::pair<int, int> c_sand = sand_source;
    int sand_units = 0;
    bool did_move = true;
    while (did_move) {
        bool moved = false;
        for (auto dir : sand_dirs) {
            std::pair<int, int> next = c_sand;
            next.first += dir.first;
            next.second += dir.second;
            if (next.second < 0 || next.second >= 300) {
                std::cout << next.first << "," << next.second << "\n";
                for (int i = 0; i < 300; i++) {
                    for (int j = 0; j < 600; j++) {
                        if (cave[j][i]) {
                            std::cout << '#';
                        } else {
                            std::cout << '.';
                        }
                    }
                    std::cout << "\n";
                }

            } else if (next.first < 0 || next.first >= 1200) {
                std::cout << "\nDouble Weird" << "\n";
            }
            if (cave[next.first][next.second]) {
                continue;
            } else {
                moved = true;
                c_sand = next;
                break;
            }
        }
        if (!moved) {
            cave[c_sand.first][c_sand.second] = true;
            if (c_sand == sand_source) {
                did_move = false;
            }

            c_sand = sand_source;
            sand_units++;
        }
    }
    return sand_units;

}

int main() {
    const std::string f_path = "../input.txt";
    auto cave = constructCave(f_path);
    std::cout << part_2(cave) << "\n";


}