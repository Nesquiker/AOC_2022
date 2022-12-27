#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <ctime>
#include <climits>

struct monkey {
    std::deque<long long> items;
    std::pair<char, int> op;
    std::vector<int> test;
    long long inspections = 0;
};

std::vector<monkey> readMonkeys (const std::string f_path) {
    std::ifstream data(f_path);
    std::vector<monkey> monkeys;
    for (std::string line; std::getline(data, line);) {
        if (line.empty()) {
            continue;
        }
        if (line[0] == 'M') {
            monkeys.emplace_back();
            continue;
        }
        if (line[2] == 'S') {
            std::string snums = line.substr(18);
            std::string buffer;
            std::deque<long long> items;
            for (char c : snums) {
                if (isdigit(c)) {
                    buffer.push_back(c);
                } else if (c == ',') {
                    items.push_back(std::stoi(buffer));
                    buffer = "";
                }
            }
            items.push_back(std::stoi(buffer));
            monkeys.back().items = items;
        } else if (line[2] == 'O') {
            std::string buffer;
            for (char c : line) {
                if (c == '+' || c == '*') {
                    monkeys.back().op.first = c;
                }
                if (isdigit(c)) {
                    buffer.push_back(c);
                }
            }
            if (buffer.empty()) {
                monkeys.back().op.second = INT_MAX;
            } else {
                monkeys.back().op.second = std::stoi(buffer);
            }
        } else if (line[2] == 'T') {
            monkeys.back().test.push_back(std::stoi(line.substr(21)));
        } else {
            monkeys.back().test.push_back(line.back() - '0');
        }
    }
    return monkeys;
}

int part_1 (std::vector<monkey>& monkeys) {
    int boredom_divisor = 3;
    int target_rounds = 20;
    for (int r = 0; r < target_rounds; r++) {
        for (auto & monkey : monkeys) {
            while (!monkey.items.empty()) {
                long long item = monkey.items.front();
                monkey.items.pop_front();
                monkey.inspections++;
                const long long op_val = (monkey.op.second != INT_MAX) ? monkey.op.second : item;
                if (monkey.op.first == '+') {
                    item += op_val;
                } else {
                    item *= op_val;
                }
                item /= boredom_divisor;
                if (item % monkey.test.front() == 0) {
                    monkeys[monkey.test[1]].items.emplace_back(item);
                } else {
                    monkeys[monkey.test[2]].items.emplace_back(item);
                }
            }
        }
    }
    std::vector<int> inspections;
    for (auto& monkey : monkeys) {
        inspections.push_back(monkey.inspections);
    }
    std::sort(inspections.begin(), inspections.end(), std::greater<>());
    return inspections[0] * inspections[1];
}


long long part_2 (std::vector<monkey>& monkeys) {
    int target_rounds = 10000;
    long long modulo_num = 1;
    for (auto& monkey : monkeys) {
        modulo_num *= monkey.test.front();
    }
    for (int r = 0; r < target_rounds; r++) {
        for (auto & monkey : monkeys) {
            while (!monkey.items.empty()) {
                long long item = monkey.items.front();
                monkey.items.pop_front();
                monkey.inspections++;
                const long long op_val = (monkey.op.second != INT_MAX) ? monkey.op.second : item;
                if (monkey.op.first == '+') {
                    item += op_val;
                } else {
                    item *= op_val;
                }
                item %= modulo_num;
                if (item % monkey.test.front() == 0) {
                    monkeys[monkey.test[1]].items.emplace_back(item);
                } else {
                    monkeys[monkey.test[2]].items.emplace_back(item);
                }
            }
        }
    }
    std::vector<long long> inspections;
    for (auto& monkey : monkeys) {
        inspections.push_back(monkey.inspections);
    }
    std::sort(inspections.begin(), inspections.end(), std::greater<>());
    return inspections[0] * inspections[1];
}

int main() {
    std::clock_t start = clock();
    const std::string f_path = "../input.txt";
    std::vector<monkey> monkeys = readMonkeys(f_path);
    std::cout << part_1(monkeys) << "\n";
    monkeys = readMonkeys(f_path);
    std::cout << part_2(monkeys) << "\n";

}