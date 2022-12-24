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
#include <future>
#include <array>


template<class T>
inline void hash_combine(std::size_t &seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<class A, class B>
struct std::hash<std::pair<A, B>> {
    size_t operator()(const std::pair<A, B> &pair) const {
        std::size_t seed = 0;
        hash_combine(seed, pair.first);
        hash_combine(seed, pair.second);
        return seed;
    }
};

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

std::multimap<std::pair<int, int>, char>
getnext(const std::multimap<std::pair<int, int>, char> &positions, const std::pair<int, int> &tl,
        const std::pair<int, int> &br) {
    std::multimap<std::pair<int, int>, char> newpositions;
    for (const auto &elem: positions) {
        auto newpos = elem.first;
        switch (elem.second) {
            case '^': {
                newpos.first--;
                if (newpos.first == tl.first)
                    newpos.first = br.first - 1;
                break;
            }
            case 'v': {
                newpos.first++;
                if (newpos.first == br.first)
                    newpos.first = tl.first + 1;
                break;
            }
            case '>': {
                newpos.second++;
                if (newpos.second == br.second)
                    newpos.second = tl.second + 1;
                break;
            }
            case '<': {
                newpos.second--;
                if (newpos.second == tl.second)
                    newpos.second = br.second - 1;
                break;
            }
        }
        newpositions.insert({newpos, elem.second});
    }
    return newpositions;
}

struct PositionIndex {
    int last_idx = 0;
    std::map<std::multimap<std::pair<int, int>, char>, int> cache;

    int operator()(const std::multimap<std::pair<int, int>, char> &m) {
        auto state = cache.emplace(m, last_idx);
        if(state.second)
            return last_idx++;
        return state.first->second;
    }
};

std::pair<int, std::multimap<std::pair<int, int>, char>>
bfs(std::multimap<std::pair<int, int>, char> positions, const std::pair<int, int> &start,
    const std::pair<int, int> &end, const std::pair<int, int> &tl, const std::pair<int, int> &br) {

    std::unordered_set<std::pair<int, std::pair<int, int>>> cache;
    PositionIndex position_index_gen;
    int round = 0;
    std::deque<std::pair<int, int>> q = {start};
    while (!q.empty()) {
        auto newpositions = getnext(positions, tl, br);
        int qsize = q.size();
        int position_index = position_index_gen(positions);
        while (qsize-- > 0) {
            auto cur = q.front();
            q.pop_front();

            if (cur == end)
                return {round, positions};

            if (!cache.insert({position_index, cur}).second)
                continue;

            static std::vector<std::pair<int, int>> neighs = {{-1, 0},
                                                              {1,  0},
                                                              {0,  0},
                                                              {0,  -1},
                                                              {0,  1}};
            for (const auto &neigh: neighs) {
                std::pair<int, int> next = {neigh.first + cur.first, neigh.second + cur.second};
                if (next.first < tl.first || next.first > br.first || next.second < tl.second ||
                    next.second > br.second || newpositions.count(next) > 0) {
                    continue;
                }
                q.push_back(next);
            }
        }
        std::swap(positions, newpositions);
        ++round;
    }
    return {round, positions};
}

void part1() {

    auto table = read();
    const std::pair<int, int> tl = {0, 0};
    const std::pair<int, int> rb = {table.size() - 1, table[0].size() - 1};
    const std::pair<int, int> start = {0, 1};
    const std::pair<int, int> end = {table.size() - 1, table[0].size() - 2};

    std::multimap<std::pair<int, int>, char> positions;
    for (int i = 0; i < table.size(); ++i)
        for (int j = 0; j < table[0].size(); ++j)
            if (table[i][j] != '.') positions.emplace(std::make_pair(i, j), table[i][j]);

    auto result = bfs(positions, start, end, tl, rb).first;
    std::cout << "Part1: " << result << std::endl;
}

void part2() {

    auto table = read();
    const std::pair<int, int> tl = {0, 0};
    const std::pair<int, int> rb = {table.size() - 1, table[0].size() - 1};
    const std::pair<int, int> start = {0, 1};
    const std::pair<int, int> end = {table.size() - 1, table[0].size() - 2};

    std::multimap<std::pair<int, int>, char> positions;
    for (int i = 0; i < table.size(); ++i)
        for (int j = 0; j < table[0].size(); ++j)
            if (table[i][j] != '.') positions.emplace(std::make_pair(i, j), table[i][j]);

    auto a = bfs(positions, start, end, tl, rb);
    auto b = bfs(a.second, end, start, tl, rb);
    auto c = bfs(b.second, start, end, tl, rb);
    std::cout << "Part2: " << a.first + b.first + c.first << std::endl;
}


int main() {
    part1();
    part2();
    return 0;
}
