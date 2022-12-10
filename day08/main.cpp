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

void part1() {
    auto table = read();
    const int row = table.size();
    const int col = table[0].size();
    std::vector<std::vector<bool>> visible(row, std::vector<bool>(col, false));

    for (int i = 0; i < row; ++i) {
        // left to right
        {
            char largest = 0;
            for (int j = 0; j < col; ++j) {
                if (largest < table[i][j]) {
                    visible[i][j] = true;
                    largest = table[i][j];
                }
            }
        }

        // right to left
        {
            char largest = 0;
            for (int j = col - 1; j >= 0; --j) {
                if (largest < table[i][j]) {
                    visible[i][j] = true;
                    largest = table[i][j];
                }
            }
        }
    }

    for (int j = 0; j < col; ++j) {
        // up to down
        {
            char largest = 0;
            for (int i = 0; i < col; ++i) {
                if (largest < table[i][j]) {
                    visible[i][j] = true;
                    largest = table[i][j];
                }
            }
        }

        // down to up
        {
            char largest = 0;
            for (int i = row - 1; i >= 0; --i) {
                if (largest < table[i][j]) {
                    visible[i][j] = true;
                    largest = table[i][j];
                }
            }
        }
    }

    int64_t total = 0;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            if (visible[i][j]) ++total;

    std::cout << "Part1: " << total << std::endl;
}

int count(const std::vector<std::string> &table, int i, int j, int deltai, int deltaj) {
    const char value = table[i][j];
    i += deltai;
    j += deltaj;

    int result = 0;
    while ((i >= 0 && i < table.size() && j >= 0 && j < table[0].size()) && table[i][j] < value) {
        i += deltai;
        j += deltaj;
        ++result;
    }
    if ((i >= 0 && i < table.size() && j >= 0 && j < table[0].size()))
        ++result;
    return result;
}

void part2() {
    auto table = read();
    const int row = table.size();
    const int col = table[0].size();

    int64_t total = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            total = std::max<int64_t>(total,
                                      count(table, i, j, -1, 0) * count(table, i, j, 1, 0) *
                                      count(table, i, j, 0, -1) * count(table, i, j, 0, 1));
        }
    }

    std::cout << "Part2: " << total << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
