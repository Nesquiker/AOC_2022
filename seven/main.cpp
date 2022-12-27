#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <climits>


struct directory {
    directory* parent;
    std::vector<std::pair<std::string, int>> files;
    std::unordered_map<std::string, directory*> sub_directories;
};

std::pair<int, int> max_val {INT_MAX, 0};


directory* cd (const std::string& directory_name, directory* curr_dir) {
    if (directory_name == "..") {
        if (curr_dir -> parent) {
            return curr_dir -> parent;
        } else {
            return curr_dir;
        }
    }
    if (curr_dir->sub_directories.contains(directory_name)) {
        return curr_dir->sub_directories[directory_name];
    } else {
        auto* n_dir = new directory;
        n_dir-> parent = curr_dir;
        curr_dir->sub_directories[directory_name] = n_dir;
        return n_dir;
    }
}

int fileSize (const std::vector<std::pair<std::string, int>>& files) {
    int output = 0;
    for (auto& file : files) {
        output += file.second;
    }
    return output;
}

std::pair<std::string, int> readFile(const std::string& line_in_ls) {
    if (line_in_ls[0] == 'd') {
        return {"null", -1};
    }
    std::string buffer;
    std::pair<std::string, int> output_file;
    for (char c : line_in_ls) {
        if (c == ' ') {
            output_file.second = std::stoi(buffer);
            buffer = "";
        } else {
            buffer.push_back(c);
        }
    }
    output_file.first = buffer;
    return output_file;
}

directory* createFileTree (const std::string url) {
    std::ifstream data(url);
    std::string line;
    std::getline(data, line);
    auto* root = new directory;
    auto curr_dir = root;
    for (line; std::getline(data, line);) {
        if (line[0] != '$') {
            std::pair<std::string, int> file = readFile(line);
            if (file.first == "null") {
                continue;
            }
            curr_dir -> files.push_back(file);
        } else if (line[2] == 'c') {
            std::string directory_name = line.substr(5);
            curr_dir = cd(directory_name, curr_dir);
        }
    }
    return root;
}

long long recursiveSizeFinderP1 (const directory* dir) {
    auto subs = dir -> sub_directories;
    long long total = fileSize(dir -> files);
    for (auto& sub : subs) {
        total += recursiveSizeFinderP1(sub.second);
    }
    int target_size = 6975962;
    int diff = total - target_size;
    if (diff >= 0 && diff < max_val.first) {
        max_val = std::make_pair(diff, total);
    }
    return total;
}

long long part_1 (const std::string file) {
    directory* root = createFileTree(file);
    long long total = recursiveSizeFinderP1(root);
    std::cout << max_val.second << '\n';
    std::cout << total;




}

int main() {
    const std::string file = "../input.txt";
    const int ans = part_1(file);

}