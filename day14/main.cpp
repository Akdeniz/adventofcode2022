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
#include <list>
#include <regex>
#include <variant>
#include <vector>

auto read() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    std::string line;
    std::vector<std::string> result;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        result.push_back(line);
    }
    return result;
}

auto read_all() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    std::stringstream data;
    data << ifs.rdbuf();
    return data.str();
}

std::vector<std::pair<int, int>> parse(const std::string &data) {

    std::regex pos_regex("([0-9]+),([0-9]+)");

    std::vector<std::pair<int, int>> result;
    for (std::sregex_iterator it = std::sregex_iterator(data.begin(), data.end(), pos_regex);
         it != std::sregex_iterator(); ++it) {
        std::smatch match = *it;

        result.push_back({std::stoi(match[2]), std::stoi(match[1])});
    }
    return result;
}

bool drop_sand(std::set<std::pair<int, int>> &table, int lowest) {
    std::pair<int, int> sand = {0, 500};

    while (sand.first <= lowest) {
        if (table.count({sand.first + 1, sand.second}) == 0) // down
            sand = {sand.first + 1, sand.second};
        else if (table.count({sand.first + 1, sand.second - 1}) == 0) // left-down
            sand = {sand.first + 1, sand.second - 1};
        else if (table.count({sand.first + 1, sand.second + 1}) == 0) // right-down
            sand = {sand.first + 1, sand.second + 1};
        else {
            table.insert(sand);
            return true;
        }
    }
    return false;
}

int delta(int a, int b) {
    if (a > b) return 1;
    if (a < b) return -1;
    return 0;
}

auto parse() {
    std::set<std::pair<int, int>> table;
    for (const auto &line: read()) {
        auto positions = parse(line);
        assert(positions.size() > 1);

        for (int i = 1; i < positions.size(); ++i) {
            auto from = positions[i - 1];
            auto to = positions[i];

            auto deltax = delta(to.first, from.first);
            auto deltay = delta(to.second, from.second);

            while (from != to) {
                table.insert(from);
                from = {from.first + deltax, from.second + deltay};
            }
            table.insert(to);
        }
    }
    return table;
}

void part1() {

    auto table = parse();
    auto lowest = std::reduce(table.begin(), table.end(), 0,
                              [](int init, const auto &pos) { return std::max(init, pos.first); });

    int result = 0;
    while (drop_sand(table, lowest)) {
        ++result;
    }
    std::cout << "Part1: " << result << std::endl;
}


bool drop_sand_part2(std::set<std::pair<int, int>> &table, int base) {
    std::pair<int, int> sand = {0, 500};

    if (table.count(sand) > 0)
        return false;

    while (true) {
        if (base == sand.first + 1) {
            table.insert(sand);
            return true;
        }

        if (table.count({sand.first + 1, sand.second}) == 0)
            sand = {sand.first + 1, sand.second};
        else if (table.count({sand.first + 1, sand.second - 1}) == 0)
            sand = {sand.first + 1, sand.second - 1};
        else if (table.count({sand.first + 1, sand.second + 1}) == 0)
            sand = {sand.first + 1, sand.second + 1};
        else {
            table.insert(sand);
            return true;
        }
    }
}

void part2() {

    auto table = parse();
    auto lowest = std::reduce(table.begin(), table.end(), 0,
                              [](int init, const auto &pos) { return std::max(init, pos.first); });

    int result = 0;
    while (drop_sand_part2(table, lowest + 2)) {
        ++result;
    }

    std::cout << "Part2: " << result << std::endl;
}


int main() {
    part1();
    part2();
    return 0;
}
