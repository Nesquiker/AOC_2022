#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <bitset>




bool checkUniqueness(const std::bitset<26>& letters) {
    int total = 0;
    for (int i = 0; i < 26; i++) {
        if (letters[i]) {
            total++;
        }
    }
    return (total == 14);
}

void fillLetters (std::bitset<26>& letters, const std::deque<char>& packet) {
    for (char letter : packet) {
        letters[letter - 'a'] = true;
    }
}

int part_1 (const std::string& line) {
    std::bitset<26> letters;
    std::deque<char> packet;
    const int end = (int)line.size();
    int index = 0;
    while (packet.size() < 14) {
        packet.push_front(line[index]);
        letters[line[index] - 'a'] = true;
        index++;
        if (checkUniqueness(letters))
            return index;
    }
    while (index < end) {
        packet.push_front(line[index]);
        packet.pop_back();
        letters.reset();
        fillLetters(letters, packet);
        index++;
        if (checkUniqueness(letters)) {
            return index;
        }
    }
    return -1;
}

int main() {
    const std::string file = "../input.txt";
    std::ifstream data(file);
    std::string line;
    std::getline(data, line);
    std::cout << part_1(line);


}