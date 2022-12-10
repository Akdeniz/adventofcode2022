#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <sstream>

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

/*
    [C]             [L]         [T]
    [V] [R] [M]     [T]         [B]
    [F] [G] [H] [Q] [Q]         [H]
    [W] [L] [P] [V] [M] [V]     [F]
    [P] [C] [W] [S] [Z] [B] [S] [P]
[G] [R] [M] [B] [F] [J] [S] [Z] [D]
[J] [L] [P] [F] [C] [H] [F] [J] [C]
[Z] [Q] [F] [L] [G] [W] [H] [F] [M]
 1   2   3   4   5   6   7   8   9
     */

const std::vector<std::string> original_stacks = {
        {},
        "ZLG",
        "QLRPWFWC",
        "FPMCLGR",
        "LFBWPHM",
        "GCFSVQ",
        "WHJZMQTL",
        "HFSBV",
        "FJZS",
        "MCDPFHBT"
};

void part1() {
    auto stacks = original_stacks;
    for (const auto &line: read()) {
        // move 1 from 5 to 6
        int count = 0, from = 0, to = 0;
        std::sscanf(line.data(), "move %d from %d to %d", &count, &from, &to);
        while (count-- > 0) {
            stacks[to].push_back(stacks[from].back());
            stacks[from].pop_back();
        }
    }

    std::string result;
    std::transform(stacks.begin() + 1, stacks.end(), std::back_inserter(result),
                   [](const std::string &stack) { return stack.back(); });
    std::cout << "Part1: " << result << std::endl;
}


void part2() {
    auto stacks = original_stacks;
    for (const auto &line: read()) {
        // move 1 from 5 to 6
        int count = 0, from = 0, to = 0;
        std::sscanf(line.data(), "move %d from %d to %d", &count, &from, &to);
        auto begin = stacks[from].begin() + stacks[from].size() - count;
        stacks[to].insert(stacks[to].end(), begin, stacks[from].end());
        stacks[from].erase(begin, stacks[from].end());
    }

    std::string result;
    std::transform(stacks.begin() + 1, stacks.end(), std::back_inserter(result),
                   [](const std::string &stack) { return stack.back(); });
    std::cout << "Part2: " << result << std::endl;
}


int main() {
    part1();
    part2();
    return 0;
}
