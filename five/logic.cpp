#include <iostream>
#include <fstream>
#include <vector>
#include <stack>


std::string supplyStacks_p1 (const std::string file) {
    std::ifstream data(file);
    std::string line;
    std::getline(data, line);
    std::vector<std::stack<char>> crate_stacks (9);
    std::vector<std::stack<char>> temp_stacks (9);
    while (!line.empty()) {
        int space_count = 0;
        int location = 0;
        for (char c : line) {
            if (isalpha(c)) {
                location += space_count / 4;
                space_count = 0;
                temp_stacks[location].emplace(c);
                location++;
            } else if (c == ' ') {
                space_count++;
            }
        }

        std::getline(data, line);
    }
    for (int i = 0; i < temp_stacks.size(); i++) {
        while (!temp_stacks[i].empty()) {
            crate_stacks[i].emplace(temp_stacks[i].top());
            temp_stacks[i].pop();
        }
    }
    while (std::getline(data, line)) {
        std::vector<int> move_from_to;
        std::string number_buffer;
        for (char c : line) {
            if (isdigit(c)) {
                number_buffer.push_back(c);
            } else if (!number_buffer.empty()) {
                const int number = std::stoi(number_buffer);
                move_from_to.emplace_back(number);
                number_buffer = "";
            }
        }
        move_from_to.push_back(std::stoi(number_buffer));
        for (int i = 0; i < move_from_to[0]; i++) {
            const int from = move_from_to[1] - 1;
            const int to = move_from_to[2] - 1;
            if (crate_stacks[from].empty()) {
                break;
            }
            const char crate_moving = crate_stacks[from].top();
            crate_stacks[from].pop();
            crate_stacks[to].emplace(crate_moving);
        }
    }
    std::string top_crates;
    for (auto& stack : crate_stacks) {
        if (!stack.empty()) {
            top_crates.push_back(stack.top());
        }
    }
    return top_crates;

}


std::string supplyStacks_p2 (const std::string file) {
    std::ifstream data(file);
    std::string line;
    std::getline(data, line);
    std::vector<std::stack<char>> crate_stacks (9);
    std::vector<std::stack<char>> temp_stacks (9);
    while (!line.empty()) {
        int space_count = 0;
        int location = 0;
        for (char c : line) {
            if (isalpha(c)) {
                location += space_count / 4;
                space_count = 0;
                temp_stacks[location].emplace(c);
                location++;
            } else if (c == ' ') {
                space_count++;
            }
        }

        std::getline(data, line);
    }
    for (int i = 0; i < temp_stacks.size(); i++) {
        while (!temp_stacks[i].empty()) {
            crate_stacks[i].emplace(temp_stacks[i].top());
            temp_stacks[i].pop();
        }
    }
    while (std::getline(data, line)) {
        std::vector<int> move_from_to;
        std::string number_buffer;
        for (char c : line) {
            if (isdigit(c)) {
                number_buffer.push_back(c);
            } else if (!number_buffer.empty()) {
                const int number = std::stoi(number_buffer);
                move_from_to.emplace_back(number);
                number_buffer = "";
            }
        }
        move_from_to.push_back(std::stoi(number_buffer));
        const int from = move_from_to[1] - 1;
        const int to = move_from_to[2] - 1;
        std::stack<int> moving_stack;
        for (int i = 0; i < move_from_to[0]; i++) {
            if (crate_stacks[from].empty()) {
                break;
            }
            const char crate_moving = crate_stacks[from].top();
            crate_stacks[from].pop();
            moving_stack.emplace(crate_moving);
        }
        while (!moving_stack.empty()) {
            crate_stacks[to].emplace(moving_stack.top());
            moving_stack.pop();
        }
    }
    std::string top_crates;
    for (auto& stack : crate_stacks) {
        if (!stack.empty()) {
            top_crates.push_back(stack.top());
        }
    }
    return top_crates;

}

int main() {
    const std::string file = "../input.txt";
    const std::string crates = supplyStacks_p2(file);
    std::cout << crates;
}