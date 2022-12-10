#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>

auto read() {
    std::fstream ifs("/Users/ecem/CLionProjects/adventofcode2022/day10/day10.txt");
    std::string line;
    std::vector<std::string> result;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        result.push_back(line);
    }
    return result;
}

std::vector<std::string> split(const std::string &str) {
    std::istringstream split(str);
    std::vector<std::string> tokens;
    for (std::string each; std::getline(split, each, ' '); tokens.push_back(each));
    return tokens;
}

void part1() {

    int64_t result = 0;
    int64_t x = 1;
    int64_t cycle = 0;
    int64_t next_cycle = 20;

    for (const auto &line: read()) {
        auto tokens = split(line);

        if (tokens[0] == "addx") {
            int64_t value = std::stoi(tokens[1]);
            cycle += 2;

            if (next_cycle <= cycle) {
                result += (next_cycle * x);
                next_cycle += 40;
            }

            x += value;
        } else {
            assert(tokens[0] == "noop");
            cycle += 1;

            if (next_cycle <= cycle) {
                result += (next_cycle * x);
                next_cycle += 40;
            }
        }
    }

    std::cout << "Part1: " << result << std::endl;
}

void check(int64_t x, int64_t &cycle, std::vector<char> &crt) {
    if (x - 1 <= (cycle % 40) && (cycle % 40) <= x + 1) {
        crt[cycle] = '#';
    }

    ++cycle;
}

void part2() {

    int64_t x = 1;
    int64_t cycle = 0;
    std::vector<char> crt = std::vector<char>(240, ' ');

    for (const auto &line: read()) {
        auto tokens = split(line);

        if (tokens[0] == "addx") {
            int64_t value = std::stoi(tokens[1]);
            check(x, cycle, crt);
            check(x, cycle, crt);
            x += value;
        } else {
            assert(tokens[0] == "noop");
            check(x, cycle, crt);
        }
    }

    std::cout << "Part2:" << std::endl;
    for (int i = 0; i + 40 <= crt.size(); i += 40) {
        std::cout << std::string(crt.begin() + i, crt.begin() + i + 40) << std::endl;
    }
}

int main() {
    part1();
    part2();
    return 0;
}
