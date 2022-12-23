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

int count(const std::unordered_set<std::pair<int, int>> &elves, const std::vector<std::pair<int, int>> &offsets,
          const std::pair<int, int> &pos) {
    int total = 0;
    for (const auto &dir: offsets)
        total += elves.count({pos.first + dir.first, pos.second + dir.second});
    return total;
}

/*
If there is no Elf in the N, NE, or NW adjacent positions, the Elf proposes moving north one step.
If there is no Elf in the S, SE, or SW adjacent positions, the Elf proposes moving south one step.
If there is no Elf in the W, NW, or SW adjacent positions, the Elf proposes moving west one step.
If there is no Elf in the E, NE, or SE adjacent positions, the Elf proposes moving east one step.
 */
std::pair<int, int> find_proposal(const std::unordered_set<std::pair<int, int>> &elves, const std::pair<int, int> &pos,
                                  const std::array<char,4> &proposal_order) {

    static const std::vector<std::pair<int, int>> alldir = {{-1, -1},{-1, 0},{-1, 1},{0,  -1},{0,  1},{1,  -1},{1,  0},{1,  1}};
    if (count(elves, alldir, pos) == 0) return pos;

    for (char c: proposal_order) {
        switch (c) {
            case 'N': {
                static const std::vector<std::pair<int, int>> dirs = {{-1, -1},{-1, 0},{-1, 1}};
                if (count(elves, dirs, pos) == 0) return {pos.first - 1, pos.second};
                break;
            }
            case 'S': {
                static const std::vector<std::pair<int, int>> dirs = {{1, -1},{1, 0},{1, 1}};
                if (count(elves, dirs, pos) == 0) return {pos.first + 1, pos.second};
                break;
            }
            case 'W': {
                static const std::vector<std::pair<int, int>> dirs = {{-1, -1},{0,  -1},{1,  -1}};
                if (count(elves, dirs, pos) == 0) return {pos.first, pos.second - 1};
                break;
            }
            case 'E': {
                static const std::vector<std::pair<int, int>> dirs = {{-1, 1},{0,  1},{1,  1}};
                if (count(elves, dirs, pos) == 0) return {pos.first, pos.second + 1};
                break;
            }
        }
    }
    return pos;
}


auto parse() {
    std::unordered_set<std::pair<int, int>> elves;
    const auto table = read();
    for (int i = 0; i < table.size(); ++i)
        for (int j = 0; j < table[i].size(); ++j)
            if (table[i][j] == '#') elves.insert({i, j});
    return elves;
}

auto execute(const std::unordered_set<std::pair<int, int>>& elves, const std::array<char,4> &proposal_order) {
    std::unordered_map<std::pair<int, int>, std::pair<int,int>> proposals;
    std::unordered_map<std::pair<int, int>, int> proposal_count;

    for (const auto &elf: elves) {
        auto proposal = find_proposal(elves, elf, proposal_order);
        proposals[elf] = proposal;
        proposal_count[proposal]++;
    }

    int number_of_changes = 0;
    std::unordered_set<std::pair<int, int>> newpositions;
    for (const auto &elf: elves) {
        const auto& proposal = proposals[elf];
        if (proposal_count[proposal] == 1) {
            newpositions.insert(proposal);
            if(elf!=proposal) ++number_of_changes;
        }
        else
            newpositions.insert(elf);
    }
    return std::make_pair(number_of_changes>0 ,newpositions);
}

void part1() {

    auto elves = parse();
    std::array<char,4> proposal_order = {'N','S','W','E'};
    for (int i = 0; i < 10; ++i) {
        auto newpositions = execute(elves, proposal_order);
        std::swap(newpositions.second, elves);
        proposal_order = { proposal_order[1], proposal_order[2], proposal_order[3], proposal_order[0]};
    }

    std::pair<int, int> tl, br;
    tl = br = *elves.begin();
    for (const auto &elf: elves) {
        tl.first = std::min(tl.first, elf.first);
        tl.second = std::min(tl.second, elf.second);

        br.first = std::max(br.first, elf.first);
        br.second = std::max(br.second, elf.second);
    }

    int result = (br.first - tl.first + 1) * (br.second - tl.second + 1) - elves.size();
    std::cout << "Part1: " << result << std::endl;
}


void part2() {

    auto elves = parse();
    std::array<char,4> proposal_order = {'N','S','W','E'};
    int round = 1;
    for (;; ++round) {
        auto newpositions = execute(elves, proposal_order);
        std::swap(newpositions.second, elves);
        if(!newpositions.first)
            break;
        proposal_order = { proposal_order[1], proposal_order[2], proposal_order[3], proposal_order[0]};
    }

    std::cout << "Part2: " << round << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
