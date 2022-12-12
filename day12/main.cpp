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

std::pair<std::pair<int, int>, std::pair<int, int>> find_start_end(const std::vector<std::string> &table) {
    std::pair<int, int> s = {0, 0};
    std::pair<int, int> e = {0, 0};
    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table[0].size(); ++j) {
            if (table[i][j] == 'S')
                s = {i, j};
            if (table[i][j] == 'E')
                e = {i, j};
        }
    }
    return {s, e};
}

struct Node {
    std::pair<int, int> pos;
    int distance = 0;
};

int distance(const std::vector<std::string> &table, std::deque<Node> q, const std::pair<int, int> &destination) {

    std::map<std::pair<int, int>, int> distances;
    while (!q.empty()) {
        auto cur = q.front();
        q.pop_front();

        if (cur.pos == destination)
            return cur.distance;

        if (distances.count(cur.pos) > 0 && distances[cur.pos] <= cur.distance)
            continue;
        distances[cur.pos] = cur.distance;

        static std::vector<std::pair<int, int>> nexts = {{-1, 0},
                                                         {0,  1},
                                                         {1,  0},
                                                         {0,  -1}};
        for (const auto &next: nexts) {
            int newx = next.first + cur.pos.first;
            int newy = next.second + cur.pos.second;
            if (newx < 0 || newx >= table.size() || newy < 0 || newy >= table[0].size())
                continue;
            if (table[newx][newy] > table[cur.pos.first][cur.pos.second] + 1)
                continue;

            q.push_back({{newx, newy}, cur.distance + 1});
        }
    }
    return -1;
}

void part1() {
    auto table = read();
    auto se = find_start_end(table);
    table[se.first.first][se.first.second] = 'a';
    table[se.second.first][se.second.second] = 'z';

    std::cout << "Part1: " << distance(table, {{se.first, 0}}, se.second) << std::endl;
}

void part2() {
    auto table = read();
    auto se = find_start_end(table);
    table[se.first.first][se.first.second] = 'a';
    table[se.second.first][se.second.second] = 'z';

    std::deque<Node> starting_points;
    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table[0].size(); ++j) {
            if (table[i][j] == 'a')
                starting_points.push_back({{i, j}, 0});
        }
    }

    std::cout << "Part2: " << distance(table, starting_points, se.second) << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
