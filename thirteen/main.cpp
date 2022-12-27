#include <iostream>
#include <fstream>
#include <stack>
#include <vector>

constexpr int findClose (int pointer, const std::string& s) {
    int count = 1;
    while (count > 0) {
        pointer++;
        if (s[pointer] == '[') {
            count++;
        } else if (s[pointer] == ']') {
            count--;
        }
    }
    return pointer;
}

std::vector<std::string> inititiatePackets (const std::string& f_path) {
    std::ifstream data(f_path);
    std::string left;
    std::string right;
    std::string empty;
    std::vector<std::string> output;
    for (left; std::getline(data, left);) {
        std::getline(data, right);
        output.emplace_back(left);
        output.emplace_back(right);
        std::getline(data, empty);
    }
    return output;
}

std::string findBuffer (int& pointer, const std::string& s) {
    std::string buffer;
    while (pointer < s.size()) {
        if (isdigit(s[pointer])) {
            buffer.push_back(s[pointer]);
        } else if (s[pointer] == '[') {
            const int close = findClose(pointer, s);
            buffer = s.substr(pointer + 1, close - pointer - 1);
            pointer = close + 1;
            break;
        } else if (s[pointer] == ',' && !buffer.empty()) {
            pointer++;
            break;
        }
        pointer++;
    }
    return buffer;
}

int correctOrderCheck(const std::string& left, const std::string& right) {
    int p_left = 0;
    int p_right = 0;
    while (p_left < left.size() && p_right < right.size()) {
        const std::string left_buffer = findBuffer(p_left, left);
        const std::string right_buffer = findBuffer(p_right, right);
        int left_num = -1;
        int right_num = -1;
        bool is_list = false;
        if (!left_buffer.empty()) {
            for (char c : left_buffer) {
                if (c == ',' || c == '[') {
                    is_list = true;
                    break;
                }
            }
            if (!is_list) {
                left_num = std::stoi(left_buffer);
            }
        }
        if (!right_buffer.empty()) {
            for (char c : right_buffer) {
                if (c == ',' || c == '[') {
                    is_list = true;
                    break;
                }
            }
            if (!is_list) {
                right_num = std::stoi(right_buffer);
            }
        }

        if (is_list) {
            int val = correctOrderCheck(left_buffer, right_buffer);
            if (val != 0) {
                return val;
            }
        } else if (left_num > -1 && right_num == -1) {
            return 1;
        } else if (left_num < right_num) {
            return 2;
        } else if (left_num > right_num) {
            return 1;
        }
    }
    if (p_left >= left.size() && p_right >= right.size()) {
        return 0;
    } else if (p_left >= left.size()) {
        return 2;
    } else {
        return 1;
    }

}

int partition (std::vector<std::string>& array, int low, int high) {
    std::string pivot = array[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (correctOrderCheck(array[j], pivot) != 1) {
            i++;
            std::swap(array[i], array[j]);
        }
    }
    std::swap(array[i + 1], array[high]);
    return i + 1;
}

void quickSort (std::vector<std::string>& array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);
        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}

void packetSorter(std::vector<std::string>& packets) {
    quickSort(packets, 0, packets.size() - 1);
}

int part_2 (const std::string& f_path) {
    auto packets = inititiatePackets(f_path);
    std::string key_1 = "[[2]]";
    std::string key_2 = "[[6]]";
    packets.emplace_back(key_1);
    packets.emplace_back(key_2);
    packetSorter(packets);
    int output = 1;
    for (int i = 0; i < packets.size(); i++) {
        if (packets[i] == key_1 || packets[i] == key_2) {
            output *= (i + 1);
        }
    }
    return output;
}


int part_1 (const std::string& f_path) {
    std::ifstream data(f_path);
    std::string left;
    std::string right;
    std::string empty;
    int count = 0;
    int output = 0;
    for (left; std::getline(data, left);) {
        std::getline(data, right);
        count++;
        if (correctOrderCheck(left, right) == 2) {
            output += count;
        }
        std::getline(data, empty);
    }
    return output;
}

int main() {
    const std::string f_path = "../input.txt";
    std::cout << part_2(f_path);

}