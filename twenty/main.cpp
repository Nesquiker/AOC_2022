#include <iostream>
#include <fstream>
#include <vector>


std::vector<std::pair<long long, int>> readInput (const std::string& path) {
    std::ifstream data(path);
    std::vector<std::pair<long long,int>> inputs;
    int count = 0;
    for (std::string line; std::getline(data, line);) {
        inputs.emplace_back(std::stoi(line), count);
        count++;
    }
    return inputs;
}


void mixInput (std::vector<std::pair<long long, int>>& input, std::vector<int>& reference)  {
    const long long s = input.size();

    for (int i = 0; i < s; i++) {
        int input_index = reference[i];
        long long final_index = (input_index + input[input_index].first) % (s - 1);
        if (final_index < 0) {
            final_index = s + final_index - 1;
        }
        if (input[input_index].first < 0 && final_index == 0) {
            final_index = s - 1;
        }
        const int shift_value = final_index - input_index;

        const int number_of_moves = abs(shift_value);
        if (shift_value == 0) {
            continue;
        }
        int inc = 1;
        if (shift_value < 0) {
            inc = -1;
        }
        int count = 0;
        while (count < number_of_moves) {
            int temp = input_index + inc;
            std::swap(input[input_index], input[temp]);
            reference[input[input_index].second] = input_index;
            input_index = temp;
            count++;
        }
        reference[input[input_index].second] = input_index;
    }
}


int part_1 (std::vector<std::pair<long long, int>> data) {
    const int s = data.size();
    std::vector<int> reference (s);
    for (int i = 0; i < s; i++) {
        reference[i] = i;
    }
    mixInput(data, reference);

    int zero_location = 0;
    for (int i = 0; i < s; i++) {
        if (data[i].first == 0) {
            zero_location = i;
            break;
        }
    }
    std::vector<int> groves {(1000 + zero_location) % s, (2000 + zero_location) % s, (3000 + zero_location) % s};
    int output = 0;
    for (int grove : groves) {
        output += data[grove].first;
    }
    return output;
}

long long part_2 (std::vector<std::pair<long long, int>> data) {
    const int s = data.size();
    std::vector<int> reference (s);
    for (int i = 0; i < s; i++) {
        reference[i] = i;
    }
    const long long multiplier = 811589153;
    for (int i = 0; i < s; i++) {
        data[i].first *= multiplier;
    }
    for (int i = 0; i < 10; i++) {
        mixInput(data, reference);
    }

    int zero_location = 0;
    for (int i = 0; i < s; i++) {
        if (data[i].first == 0) {
            zero_location = i;
            break;
        }
    }
    std::vector<long long> groves {(1000 + zero_location) % s, (2000 + zero_location) % s, (3000 + zero_location) % s};
    long long output = 0;
    for (long long grove : groves) {
        output += data[grove].first;
    }
    return output;
}


int main() {

    const std::string test_path = "../test.txt";
    const std::string real_path = "../input.txt";
    auto test_input = readInput(test_path);
    auto real_input = readInput(real_path);
    std::cout << part_1(test_input) << "\n";
    std::cout << part_1(real_input) << "\n";
    std::cout << part_2(test_input) << "\n";
    std::cout << part_2(real_input) << "\n";

}