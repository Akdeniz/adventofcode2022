#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <sstream>

auto read() {
    std::fstream ifs("/Users/ecem/CLionProjects/adventofcode2022/day04/day4.txt");
    std::string line;
    std::vector<std::string> result;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        result.push_back(line);
    }
    return result;
}

std::pair<int64_t, int64_t> parse(const std::string &input) {
    auto pos = input.find('-');
    assert(pos != std::string::npos);
    return std::make_pair(std::stoll(input.substr(0, pos)), std::stoll(input.substr(pos + 1)));
}

void part1() {
    int64_t result = 0;
    for (const auto &line: read()) {
        auto pos = line.find(',');
        assert(pos != std::string::npos);
        auto a = parse(line.substr(0, pos));
        auto b = parse(line.substr(pos + 1));
        if ((a.first <= b.first && a.second >= b.second) || (b.first <= a.first && b.second >= a.second))
            ++result;
    }
    std::cout << "Part1: " << result << std::endl;
}


void part2() {
    int64_t result = 0;
    for (const auto &line: read()) {
        std::istringstream ss(line);
        std::string first, second;
        std::getline(ss, first, ',');
        std::getline(ss, second, ',');
        auto a = parse(first);
        auto b = parse(second);
        if (std::min(a.second, b.second) >= std::max(a.first, b.first))
            ++result;
    }
    std::cout << "Part2: " << result << std::endl;
}

int main() {

    part1();
    part2();
    return 0;
}
