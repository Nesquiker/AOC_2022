#include <iostream>
#include <fstream>
#include <vector>

int campCleanup_p1 (const std::string file) {
    std::ifstream data(file);
    int pairs_contained = 0;
    for (std::string line; std::getline(data, line);) {
        int first_elf [2];
        int second_elf [2];
        bool comma_reached = false;
        std::string num_holder;
        for (char c : line) {
            if (std::isdigit(c)) {
                num_holder.push_back(c);
            } else if (c == '-') {
                if (!comma_reached) {
                    first_elf[0] = std::stoi(num_holder);
                    num_holder = "";
                } else {
                    second_elf[0] = std::stoi(num_holder);
                    num_holder = "";
                }
            } else if (c == ',') {
                first_elf[1] = std::stoi(num_holder);
                num_holder = "";
                comma_reached = true;
            }
        }
        second_elf[1] = std::stoi(num_holder);

        const int diff_1 = second_elf[0] - first_elf[0];
        const int diff_2 = second_elf[1] - first_elf[1];
        const int diff_mult = diff_1 * diff_2;

        if (diff_mult <= 0) {
            pairs_contained++;
        }
    }
    return pairs_contained;
}

int campCleanup_p2 (const std::string file) {
    std::ifstream data(file);
    int pairs_overlapped = 0;
    for (std::string line; std::getline(data, line);) {
        int first_elf [2];
        int second_elf [2];
        bool comma_reached = false;
        std::string num_holder;
        for (char c : line) {
            if (std::isdigit(c)) {
                num_holder.push_back(c);
            } else if (c == '-') {
                if (!comma_reached) {
                    first_elf[0] = std::stoi(num_holder);
                    num_holder = "";
                } else {
                    second_elf[0] = std::stoi(num_holder);
                    num_holder = "";
                }
            } else if (c == ',') {
                first_elf[1] = std::stoi(num_holder);
                num_holder = "";
                comma_reached = true;
            }
        }
        second_elf[1] = std::stoi(num_holder);

        const int diff_1 = second_elf[0] - first_elf[0];
        if (diff_1 > 0) {
            pairs_overlapped += (second_elf[0] <= first_elf[1]);
        } else {
            pairs_overlapped += (first_elf[0] <= second_elf[1]);
        }
    }
    return pairs_overlapped;
}

int main() {
    const std::string file = "../input.txt";
    std::cout << campCleanup_p2(file);

}