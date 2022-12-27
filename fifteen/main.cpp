#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <complex>

auto createSensors (const std::string& f_path) {
    std::vector<std::pair<int, std::pair<int, int>>> sensors;
    std::ifstream data(f_path);
    for (std::string line; std::getline(data, line);) {
        std::pair<int,std::pair<int,int>> sensor_data;
        std::string buffer;
        bool buffering = false;
        std::vector<int> nums;
        for (char c : line) {
            if (c == ',' || c == ':') {
                nums.push_back(std::stoi(buffer));
                buffer = "";
                buffering = false;
            }else if (buffering) {
                buffer.push_back(c);
            }else if (c == '=') {
                buffering = true;
            }
        }
        nums.push_back(std::stoi(buffer));
        sensor_data.second.first = nums[0];
        sensor_data.second.second = nums[1];
        sensor_data.first = std::abs(nums[0] - nums[2]) + std::abs(nums[1] - nums[3]);
        sensors.emplace_back(sensor_data);
    }
    return sensors;
}


int freeLocation (std::vector<std::pair<int, std::pair<int, int>>>& sensors, const int target_row) {
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> ranges_at_target;
    for (auto sensor : sensors) {
        const int x = sensor.second.first;
        const int y = sensor.second.second;
        const int magnitude = sensor.first;
        const int y_diff = std::abs(y - target_row);
        const int overlap = magnitude - y_diff;
        if (overlap < 0) {
            continue;
        }
        const std::pair<int, int> range {x - overlap, x + overlap};
        ranges_at_target.push(range);
    }
    std::pair<int, int> start_range {ranges_at_target.top()};
    if (start_range.first > 0) {
        return 0;
    }
    ranges_at_target.pop();
    while (!ranges_at_target.empty() && start_range.first <= 4000000) {
        std::pair<int, int> comp_range {ranges_at_target.top()};
        ranges_at_target.pop();
        if (comp_range.second <= start_range.second) {
            continue;
        } else if (comp_range.first > start_range.second) {
            if (start_range.second + 1 < comp_range.first && start_range.second < 4000000 && comp_range.first > 0) {
                return (start_range.second >= -1) ? start_range.second + 1 : comp_range.first - 1;
            }
            start_range = comp_range;
        } else {
            start_range = {start_range.first, comp_range.second};
        }
    }
    return -1;
}

long long part_2 (const std::string& f_path) {
    auto sensors = createSensors(f_path);
    int free_loc = -1;
    int out_row = -1;
    for (int row = 0; row < 4000001; row++) {
        free_loc = freeLocation(sensors, row);
        if (free_loc != -1) {
            out_row = row;
            break;
        }
    }
    long long output = (long long) free_loc * 4000000 + (long long)out_row;
    return output;
}

int part_1 (const std::string& f_path) {
    auto sensors = createSensors(f_path);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> ranges_at_target;
    const int target_row = 2000000;
    for (auto sensor : sensors) {
        const int x = sensor.second.first;
        const int y = sensor.second.second;
        const int magnitude = sensor.first;
        const int y_diff = std::abs(y - target_row);
        const int overlap = magnitude - y_diff;
        if (overlap < 0) {
            continue;
        }
        const std::pair<int, int> range {x - overlap, x + overlap};
        ranges_at_target.push(range);
    }
    std::pair<int, int> start_range {ranges_at_target.top()};
    ranges_at_target.pop();
    int total = 0;
    while (!ranges_at_target.empty()) {
        std::pair<int, int> comp_range {ranges_at_target.top()};
        ranges_at_target.pop();
        if (comp_range.second <= start_range.second) {
            continue;
        } else if (comp_range.first > start_range.second) {
            total += start_range.second - start_range.first + 1;
            start_range = comp_range;
        } else {
            start_range = {start_range.first, comp_range.second};
        }
    }
    total += start_range.second - start_range.first + 1;
    return total - 1;
}

int main() {
    const std::string f_path = "../input.txt";
    std::cout << part_2(f_path);
}