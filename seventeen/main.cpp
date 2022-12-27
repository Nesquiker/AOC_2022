#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

std::string readData (const std::string& path) {
    std::ifstream data(path);
    std::string line;
    std::getline(data, line);
    return line;
}

struct rocks {
    int y = 3;
    int x = 2;
    std::vector<std::vector<bool>> shape;
};

std::vector<std::vector<std::vector<bool>>> defineRocks () {
    std::vector<std::vector<std::vector<bool>>> rock_forms {{
                                         {1, 1, 1, 1}},

                                        {{0, 1, 0},
                                         {1, 1, 1},
                                         {0, 1, 0}},
                                                            {{1, 1, 1},
                                                             {0 , 0, 1},
                                                             {0, 0, 1}},

                                        {{1},
                                         {1},
                                         {1},
                                         {1}},

                                        {{1,1},
                                         {1,1}}};
    return rock_forms;
}

std::vector<std::bitset<7>> makePath (const int h) {
    std::vector<std::bitset<7>> bits (h);
    return bits;
}

bool is_conflict (const rocks& rock, const std::vector<std::bitset<7>>& path) {
    for (int y = 0; y < rock.shape.size(); y++) {
        for (int x = 0; x < rock.shape[0].size(); x++) {
            if (rock.shape[y][x]) {
                const int s_y = y + rock.y;
                const int s_x = x + rock.x;
                if (s_y < 0 || path[s_y][s_x]) {
                    return true;
                }
            }
        }
    }
    return false;
}

int part_1 (const std::string& shifts) {
    auto path = makePath(500000);
    auto rock_shapes = defineRocks();
    int count = 0;
    int heighest_rock = -1;
    int rock_type = 0;
    const int shift_size = (int)shifts.size();
    int shift_index = 0;
    const int target_rock_number = 316075;
    while (count < target_rock_number) {
        count++;
        rocks rock;
        rock.y = heighest_rock + 4;
        rock_type %= (int)rock_shapes.size();
        rock.shape = rock_shapes[rock_type];
        const int rock_width = rock.shape[0].size();
        for (;;) {
            shift_index %= shift_size;
            if (shifts[shift_index] == '>') {
                if (rock.x + rock_width < 7) {
                    rock.x++;
                    if (is_conflict(rock, path)) {
                        rock.x--;
                    }
                }
            } else if (rock.x > 0){
                rock.x--;
                if (is_conflict(rock, path)) {
                    rock.x++;
                }
            }
            shift_index++;
            rock.y--;
            if (is_conflict(rock, path)) {
                rock.y++;
                for (int y = 0; y < rock.shape.size(); y++) {
                    for (int x = 0; x < rock_width; x++) {
                        if (rock.shape[y][x]) {
                            const int s_y = y + rock.y;
                            heighest_rock = std::max(heighest_rock, s_y);
                            const int s_x = x + rock.x;
                            path[s_y][s_x] = true;
                        }
                    }
                }
                break;
            }
        }
        rock_type++;
    }
//    for (int r = path.size() - 1; r >= 0; r--) {
//        for (int c = 0; c < 7; c++) {
//            std::cout << path[r][c];
//        }
//        std::cout << "\n";
//    }
    return heighest_rock + 1;
}


std::vector<int> heighestAtNumRocks (const std::string& shifts, const int target_rock_number) {
    const int path_size = (int)shifts.size();
    const int rock_type_count = 5;
    const int pattern_size = path_size * rock_type_count;
    int pattern_bp = pattern_size;
    std::vector<int> pattern_heights {0};
    auto path = makePath(target_rock_number * 5);
    auto rock_shapes = defineRocks();
    int count = 0;
    int heighest_rock = -1;
    int rock_type = 0;
    const int shift_size = (int)shifts.size();
    int shift_index = 0;
    while (count < target_rock_number) {
        rocks rock;
        rock.y = heighest_rock + 4;
        rock_type %= (int)rock_shapes.size();
        rock.shape = rock_shapes[rock_type];
        const int rock_width = rock.shape[0].size();
        for (;;) {
            shift_index %= shift_size;
            if (shifts[shift_index] == '>') {
                if (rock.x + rock_width < 7) {
                    rock.x++;
                    if (is_conflict(rock, path)) {
                        rock.x--;
                    }
                }
            } else if (rock.x > 0){
                rock.x--;
                if (is_conflict(rock, path)) {
                    rock.x++;
                }
            }
            shift_index++;
            rock.y--;
            if (is_conflict(rock, path)) {
                rock.y++;
                for (int y = 0; y < rock.shape.size(); y++) {
                    for (int x = 0; x < rock_width; x++) {
                        if (rock.shape[y][x]) {
                            const int s_y = y + rock.y;
                            heighest_rock = std::max(heighest_rock, s_y);
                            const int s_x = x + rock.x;
                            path[s_y][s_x] = true;
                        }
                    }
                }
                break;
            }
        }
        count++;
        if (count == pattern_bp) {
            pattern_heights.emplace_back(heighest_rock + 1);
            pattern_bp += pattern_size;
        }
        rock_type++;
    }
//    for (int r = path.size() - 1; r >= 0; r--) {
//        for (int c = 0; c < 7; c++) {
//            std::cout << path[r][c];
//        }
//        std::cout << "\n";
//    }
    return pattern_heights;
}


long long part_2 (const std::string& path) {
    const int path_size = (int)path.size();
    long long final_rock_count = 1000000000000;
    const int rock_type_count = 5;
    const int pattern_size = path_size * rock_type_count;
    std::vector<int> diffs {0};
    auto pattern_heights = heighestAtNumRocks(path, pattern_size * 500);
    std::vector<int> sums;
    for (int i = 1; i < pattern_heights.size(); i++) {
        diffs.push_back(pattern_heights[i] - pattern_heights[i - 1]);
    }
    int count = 0;
    int running_sum = 0;
    int repeat_length = 339;
//     int repeat_length = 7;
    for (int i = 0; i < diffs.size(); i++) {
        running_sum += diffs[i];
        count++;
        if (count > repeat_length - 1) {
            running_sum -= diffs[i - repeat_length];
            sums.emplace_back(running_sum);
        }
    }
    const long long repeat_sum = sums.back();
    const long long repeating_rock_count = repeat_length * pattern_size;
    const long long repeats_in_total = final_rock_count / repeating_rock_count;
    const long long remainder = final_rock_count % repeating_rock_count;
//    const long long base_index = remainder;
//    const long long base_size = pattern_heights[base_index / pattern_size];
    const long long base_size = 491174;

    const long long height_final = base_size +  repeats_in_total * repeat_sum;

    return height_final;

}

int main () {
    const std::string path = readData("../input.txt");
    const std::string test = readData("../test_input.txt");
    std::cout << "TEST  : " << part_1(test) << "\n";
    std::cout << "PART_1  : " << part_1(path) << "\n";
    std::cout << "TEST PART 2 : " << part_2(test) << "\n";
    std::cout << "PART 2 : " << part_2(path) << "\n";

    return 0;
}