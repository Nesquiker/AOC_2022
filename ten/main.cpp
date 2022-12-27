#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

void part_2 (const std::string f_path) {
    std::ifstream data(f_path);
    std::string row (40, '.');
    std::vector<std::string> image (6, row);
    int cycle = -1;
    bool last_was_add = false;
    int add_x = 0;
    int total = 1;
    int r = 0;
    for (;;) {
        cycle++;
        if (cycle == 40) {
            r++;
            cycle = 0;
        }
        const int diff = std::abs(total - cycle);
        if (diff < 2) {
            image[r][cycle] = '#';
        }
        if (last_was_add) {
            total += add_x;
            last_was_add = false;
            continue;
        }
        std::string line;
        if (std::getline(data, line)) {
            if (line[0] == 'n') {
                continue;
            }
            add_x = std::stoi(line.substr(5));
            last_was_add = true;
        } else {
            break;
        }
    }
    for (std::string p : image) {
        std::cout << p << "\n";
    }
}


int main() {
    const std::string f_path = "../input.txt";
    part_2(f_path);
    return 0;
}