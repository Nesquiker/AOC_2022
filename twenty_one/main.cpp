#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <climits>

struct monkey {
    bool value_known = false;
    long long value = INT_MAX;
    std::string first_operand = "";
    std::string second_operand = "";
    char operation = ' ';
};

std::unordered_map<std::string, monkey> readMonkeys (const std::string& path) {
    std::ifstream data(path);
    std::unordered_map<std::string, monkey> monkey_map;
    for (std::string line; std::getline(data, line);) {
        std::vector<std::string> monkey_data;
        std::string buffer;
        for (char c : line) {
            if (isalnum(c) || c =='+' || c == '*' || c == '/' || c == '-') {
                buffer.push_back(c);
            } else if (!buffer.empty()) {
                monkey_data.emplace_back(buffer);
                buffer = "";
            }
        }
        monkey_data.emplace_back(buffer);
        monkey current;
        if (isdigit(monkey_data[1][0])) {
            current.value_known = true;
            current.value = std::stoi(monkey_data[1]);
        } else {
            current.value_known = false;
            current.first_operand = monkey_data[1];
            current.second_operand = monkey_data[3];
            current.operation = monkey_data [2][0];
        }
        monkey_map[monkey_data.front()] = current;
    }
    return monkey_map;
}

long long monkey_traveller (std::unordered_map<std::string, monkey>& monkey_map, const std::string& current_name) {

    monkey current = monkey_map[current_name];
    if (current.value_known) {

        return current.value;
    }

    const long long value_1 = monkey_traveller(monkey_map, current.first_operand);
    const long long value_2 = monkey_traveller(monkey_map, current.second_operand);
    const char operand = current.operation;
    long long result = 0;
    if (operand == '*') {
        result = value_1 * value_2;
    } else if (operand == '/') {
        result = value_1 / value_2;
    } else if (operand == '+') {
        result = value_1 + value_2;
    } else if (operand == '-') {
        result = value_1 - value_2;
    }

    return result;
}

long long part_1 (std::unordered_map<std::string, monkey> monkey_map) {
    return monkey_traveller(monkey_map, "root");

}

long long part_2 (std::unordered_map<std::string, monkey> monkey_map) {

    long long input = 0;
    const long long num_left = monkey_traveller(monkey_map, monkey_map["root"].second_operand);
    long long num_right;
    long long high = 100000000000000;
    long long low = -100000000000000;
    while (low <= high) {
        input = (high + low) / 2;
        monkey_map["humn"].value = input;
        num_right = monkey_traveller(monkey_map, monkey_map["root"].first_operand);
        if (num_right == num_left) {
            return input;
        }
        if (num_right > num_left) {
            low = input + 1;
        } else {
            high = input - 1;
        }
    }

    return 0;
}

int main() {
    const std::string test_path = "../test.txt";
    const std::string real_path = "../input.txt";
    auto test_monkeys = readMonkeys(test_path);
    auto real_monkeys = readMonkeys(real_path);
//    std::cout << part_1(test_monkeys) << "\n";
//    std::cout << part_1(real_monkeys) << "\n";
//    std::cout << part_2(test_monkeys) << "\n";
    std::cout << part_2(real_monkeys) << "\n";

}