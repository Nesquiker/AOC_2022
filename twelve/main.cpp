#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>


std::vector<std::vector<char>> readMap(const std::string& path) {
    std::vector<std::vector<char>> map;
    std::ifstream data(path);
    for (std::string line; std::getline(data, line);) {
        std::vector<char> row;
        for (char c : line) {
            row.emplace_back(c);
        }
        map.emplace_back(row);
    }
    return map;
}

std::vector<std::vector<int>> distancesToE (const std::vector<std::vector<char>>& map) {
    std::pair<int, int> e_location;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (map[i][j] == 'E') {
                e_location = {i, j};
            }
        }
    }
    std::vector<int> row (map[0].size(), 0);
    std::vector<std::vector<int>> output (map.size(), row);
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            const int r_diff = abs(e_location.first - i);
            const int c_diff = abs(e_location.second - j);
            output[i][j] = r_diff + c_diff;
        }
    }
    return output;
}

std::vector<std::vector<int>> makeVisited (const std::vector<std::vector<char>>& map) {
    std::vector<int> row (map[0].size(), INT_MAX);
    std::vector<std::vector<int>> output (map.size(), row);
    return output;
}

std::pair<int, int> sLocation (const std::vector<std::vector<char>>& map) {
    std::pair<int, int> s_location;
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map[0].size(); j++) {
            if (map[i][j] == 'S') {
                s_location = {i, j};
                return s_location;
            }
        }
    }
    return s_location;
}

int part_2 (const std::vector<std::vector<char>>& map) {
    const int rows = map.size();
    const int columns = map[0].size();
    const std::vector<std::pair<int, int>> dirs {{1,0}, {0, 1}, {-1, 0}, {0, -1}};
    auto squares_to_exit = distancesToE(map);
    auto visited = makeVisited(map);
    auto start = sLocation(map);
    std::priority_queue <std::pair<int, std::vector<int>>, std::vector<std::pair<int, std::vector<int>>>, std::greater<>> possibilities;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (map[i][j] == 'a') {
                visited[i][j] = 0;
                possibilities.push({0, {i, j, 0, 0}});
            }
        }
    }
    possibilities.push({0, {start.first, start.second, 0, 0}});
    int shortest = INT_MAX;
    while (possibilities.top().second.back() == 0) {
        auto current = possibilities.top().second;
        const int next_steps = current[2] + 1;
        const int r = current[0];
        const int c = current[1];
        const char height = (map[r][c] != 'S') ? map[r][c] : 'a';
        possibilities.pop();
        for (auto dir : dirs) {
            const int nr = r + dir.first;
            const int nc = c + dir.second;
            if (nr < 0 || nr >= rows || nc < 0 || nc >= columns || visited[nr][nc] <= next_steps) {
                continue;
            }
            bool next_e = false;
            char next_height = (map[nr][nc] != 'E') ? map[nr][nc] : 'z';
            if ((next_height - height) > 1) {
                continue;
            }
            if (map[nr][nc] == 'E') {
                shortest = std::min(shortest, next_steps);
                next_e = true;
            }
            visited[nr][nc] = next_steps;
            std::vector<int> pos_data {nr, nc, next_steps, next_e};
            std::pair<int, std::vector<int>> pos {squares_to_exit[nr][nc] + next_steps, pos_data};
            possibilities.emplace(pos);
        }
    }
    return shortest;
}

int part_1 (const std::vector<std::vector<char>>& map) {
    const int rows = map.size();
    const int columns = map[0].size();
    const std::vector<std::pair<int, int>> dirs {{1,0}, {0, 1}, {-1, 0}, {0, -1}};
    auto squares_to_exit = distancesToE(map);
    auto visited = makeVisited(map);
    auto start = sLocation(map);
    std::priority_queue <std::pair<int, std::vector<int>>, std::vector<std::pair<int, std::vector<int>>>, std::greater<>> possibilities;
    possibilities.push({0, {start.first, start.second, 0, 0}});
    int shortest = INT_MAX;
    while (possibilities.top().second.back() == 0) {
        auto current = possibilities.top().second;
        const int next_steps = current[2] + 1;
        const int r = current[0];
        const int c = current[1];
        const char height = (map[r][c] != 'S') ? map[r][c] : 'a';
        possibilities.pop();
        for (auto dir : dirs) {
            const int nr = r + dir.first;
            const int nc = c + dir.second;
            if (nr < 0 || nr >= rows || nc < 0 || nc >= columns || visited[nr][nc] <= next_steps) {
                continue;
            }
            bool next_e = false;
            char next_height = (map[nr][nc] != 'E') ? map[nr][nc] : 'z';
            if ((next_height - height) > 1) {
                continue;
            }
            if (map[nr][nc] == 'E') {
                shortest = std::min(shortest, next_steps);
                next_e = true;
            }
            visited[nr][nc] = next_steps;
            std::vector<int> pos_data {nr, nc, next_steps, next_e};
            std::pair<int, std::vector<int>> pos {squares_to_exit[nr][nc] + next_steps, pos_data};
            possibilities.emplace(pos);
        }

    }

    return shortest;
}


int main() {
    const std::string f_path = "../input.txt";
    auto map = readMap(f_path);
    std::cout << part_2(map);
}