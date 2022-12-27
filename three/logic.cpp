#include <iostream>
#include <fstream>
#include <bitset>

int rucksackOrganization (const std::string& file) {
    std::ifstream data(file);
    int total = 0;
    int group_count = 0;
    std::bitset<60> null_set;
    std::bitset<60> visited_letters;
    for (std::string line; std::getline(data, line);) {
        const int line_size = (int)line.size();
        group_count %= 3;
        std::bitset<60> current_visited;
        for (int i = 0; i < line_size; i++) {
            const char current_letter = line[i];
            current_visited[current_letter - 'A'] = true;
        }
        if (group_count == 0) {
            visited_letters = current_visited;
        } else {
            visited_letters = visited_letters & current_visited;
        }
        if (group_count == 2) {
            char badge = 'A';
            for (int i = 0; i < 60; i++) {
                if (visited_letters[i]) {
                    badge = (char)(i + 'A');
                    break;
                }
            }
            if (islower(badge)) {
                total += (badge - 'a') + 1;
            } else {
                total += (badge - 'A') + 27;
            }
            visited_letters = null_set;
        }
        group_count++;
    }
    return total;
}

int main() {
    const std::string file = "../input.txt";
    std::cout << rucksackOrganization(file);
    return 0;
}