#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <sstream>
#include <unordered_set>

auto read() {
    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;
    std::vector<std::string> result;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        result.push_back(line);
    }
    return result;
}

auto part(int length) {

    const auto line = read()[0];

    int i = length;
    for (; i <= line.size(); ++i) {
        std::unordered_set<char> uniques(line.begin() + i - length, line.begin() + i);
        if (uniques.size() == length)
            break;
    }

    return i;
}

int main() {
    std::cout << "Part1: " << part(4) << std::endl;
    std::cout << "Part2: " << part(14) << std::endl;
    return 0;
}
