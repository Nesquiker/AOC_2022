#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>


bool tryMoveKnot (std::vector<std::pair<int,int>>& knots, const int knot_num) {
    const int previous = knot_num - 1;
    const int y_diff = knots[previous].first - knots[knot_num].first;
    const int x_diff = knots[previous].second - knots[knot_num].second;
    if (abs(y_diff) > 1 || abs(x_diff) > 1) {
        if (y_diff < 0) {
            knots[knot_num].first--;
        } else if (y_diff > 0) {
            knots[knot_num].first++;
        }
        if (x_diff < 0) {
            knots[knot_num].second--;
        } else if (x_diff >0){
            knots[knot_num].second++;
        }
        return true;
    }
    return false;
}


std::string ropeMover (const char direction, std::vector<std::pair<int, int>>& knots) {
    if (direction == 'R') {
        knots.front().second++;
    } else if (direction == 'L') {
        knots.front().second--;
    } else if (direction == 'D') {
        knots.front().first++;
    } else if (direction == 'U') {
        knots.front().first--;
    }
    int knot_num = 1;
    while (knot_num < knots.size()) {
        if (!tryMoveKnot(knots, knot_num)) {
            break;
        }
        knot_num++;
    }
    return std::to_string(knots.back().first) + "," + std::to_string(knots.back().second);
}


int part_2 (const std::string& url) {
    std::ifstream data(url);
    std::unordered_set<std::string> visited;
    std::vector<std::pair<int, int>> knots (10);
    for (std::string line; std::getline(data, line);) {
        const char direction = line[0];
        int steps = std::stoi(line.substr(2));
        // This is where we move the rope. The end location of the rope is recorded every time and put into the set as
        // a string.
        while (steps > 0) {
            visited.insert(ropeMover(direction, knots));
            steps--;
        }
    }
    return (int)visited.size();
}


int main() {
    const std::string url = "../input.txt";
    std::cout << part_2(url);
}