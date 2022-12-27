#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

int findLargest(const std::string& file) {
    std::priority_queue<int, std::vector<int>, std::greater<>> best_three;
    int running_max = 0;
    int running_line = 0;
    std::ifstream data(file);
    std::string line;
    while (std::getline(data, line)) {
        if (line.empty()) {
            if (best_three.size() == 3) {
                running_max = best_three.top();
                best_three.pop();
            } else {
                running_max = 0;
            }
            running_max = std::max(running_max, running_line);
            best_three.emplace(running_max);
            running_line = 0;
        } else {
            running_line += std::stoi(line);
        }
    }
    if (best_three.size() == 3) {
        running_max = best_three.top();
        best_three.pop();
    } else {
        running_max = 0;
    }
    running_max = std::max(running_max, running_line);
    best_three.emplace(running_max);
    int output = 0;
    while (!best_three.empty()) {
        output += best_three.top();
        best_three.pop();
    }
    return output;
}

int main() {
    const std::string file = "../data.txt";
    const int most_calories = findLargest(file);
    std::cout << most_calories;
}