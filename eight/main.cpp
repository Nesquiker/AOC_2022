#include <iostream>
#include <fstream>
#include <vector>

std::vector<int> createMonoStack (const std::vector<int>& nums) {
    std::vector<int> stack;
    std::vector<int> output (nums.size(), -1);
    for (int i = 0; i < nums.size(); i++) {
        while (!stack.empty() && nums[stack.back()] <= nums[i]) {
            output[stack.back()] = i;
            stack.pop_back();
        }
        stack.emplace_back(i);
    }
    for (int i = 0; i < nums.size(); i++) {
        if (output[i] == -1) {
            output[i] = nums.size() - 1 - i;
        } else {
            output[i] -= i;
        }
    }
    return output;
}


int trees_part_2 (const std::vector<std::vector<int>>& tree_grid) {
    const int rows = tree_grid.size();
    const int columns = tree_grid[0].size();
    std::vector<int> d_row (columns, 0);
    std::vector<std::vector<int>> empty_grid (rows, d_row);
    auto right_next = empty_grid;
    auto left_next = empty_grid;
    auto up_next = empty_grid;
    auto down_next = empty_grid;
    for (int i = 0; i < rows; i++) {
        std::vector<int> row_right;
        row_right = tree_grid[i];
        auto right = createMonoStack(row_right);
        for (int j = 0; j < columns; j++) {
            right_next[i][j] = right[j];
        }
        std::reverse(row_right.begin(), row_right.end());
        auto left = createMonoStack(row_right);
        std::reverse(left.begin(), left.end());
        for (int j = 0; j < columns; j++) {
            left_next[i][j] = left[j];
        }
    }
    for (int c = 0; c < columns; c++) {
        std::vector<int> column_down;
        for (int i = 0; i < rows; i++){
            column_down.emplace_back(tree_grid[i][c]);
        }
        auto down = createMonoStack(column_down);
        for (int j = 0; j < rows; j++) {
            down_next[j][c] = down[j];
        }
        std::reverse(column_down.begin(), column_down.end());
        auto up = createMonoStack(column_down);
        std::reverse(up.begin(), up.end());
        for (int j = 0; j < rows; j++) {
            up_next[j][c] = up[j];
        }
    }
    int running_max = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            const int scenic_score = right_next[r][c] * left_next[r][c] * up_next[r][c] * down_next[r][c];
            running_max = std::max(running_max, scenic_score);
        }
    }
    return running_max;
}

int main() {
    const std::string url = "../input.txt";
    std::vector<std::vector<int>> tree_size_grid;
    std::ifstream data(url);
    std::string line;
    for (line; std::getline(data, line);) {
        std::vector<int> tree_row;
        for (char c : line) {
            const int num = c - '0';
            tree_row.emplace_back(num);
        }
        tree_size_grid.emplace_back(tree_row);
    }
    std::cout << trees_part_2(tree_size_grid);

}