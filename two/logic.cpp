#include <iostream>
#include <fstream>
#include <vector>

int rps_calc (const std::string& file) {
    const int win = 6;
    const int draw = 3;
    const int lose = 0;
    const std::vector<int> rps_vals {1, 2, 3};
    std::ifstream data(file);
    int total_score = 0;
    for (std::string line; std::getline(data, line);) {
        char elf_play = '0';
        char my_play = '0';
        for (char c : line) {
            if (isalpha(c)) {
                if (elf_play == '0') {
                    elf_play = c;
                } else {
                    my_play = c;
                    break;
                }
            }
        }
        int game_score = 0;
        if (my_play == 'Y') {
            game_score += draw;
            game_score += rps_vals[elf_play - 'A'];
        } else if (my_play == 'Z') {
            game_score += win;
            game_score += rps_vals[(elf_play - 'A' + 1) % 3];
        } else {
            if (elf_play - 'A' == 0) {
                game_score += rps_vals[2];
            } else {
                game_score += rps_vals[elf_play - 'A' - 1];
            }
        }
        total_score += game_score;
    }
    return total_score;
}

int main() {
    const std::string file = "../input.txt";
    std::cout << rps_calc(file);

}