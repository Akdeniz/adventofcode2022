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

struct Node {
    int rate = 0;
    std::string from;
    std::vector<std::string> to;
};

Node parse(const std::string &data) {

    //Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
    static std::regex base_regex("Valve ([A-Z]+) has flow rate=([0-9]+); tunnels? leads? to valves? ([A-Z ,]+)");
    static std::regex valve_regex("([A-Z]+)");

    std::smatch base_match;
    auto res = std::regex_match(data, base_match, base_regex);
    assert(res);
    Node node;
    node.from = base_match[1];
    node.rate = std::stoi(base_match[2]);
    auto valves = base_match[3].str();
    for (auto it = std::sregex_iterator(valves.begin(), valves.end(), valve_regex);
         it != std::sregex_iterator(); ++it) {
        node.to.push_back(it->str());
    }
    return node;
}

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

namespace std {
    template<size_t N>
    struct less<bitset<N> > : binary_function<bitset<N>, bitset<N>, bool> {
        bool operator()(const bitset<N> &L, const bitset<N> &R) const {
            return L.to_ulong() < R.to_ulong();
        }
    };
}

bool operator<(const std::bitset<16> &l, const std::bitset<16> &r) {
    return l.to_ulong() < r.to_ulong();
}

struct State {
    int node = 0;
    int64_t score = 0;
    std::bitset<16> opened;
};

int64_t bfs(const std::vector<std::vector<int>> &graph, const std::vector<int> &rates, int start, int minutes,
            const std::bitset<16> &opened) {
    int64_t result = 0;
    std::unordered_map<std::pair<std::bitset<16>, int64_t>, int64_t> memo;
    std::deque<State> q = {{start, 0, opened}};
    for (int i = minutes; i > 0; --i) {
        int size = q.size();
        while (size-- > 0) {
            const auto cur = q.front();
            q.pop_front();

            std::pair<std::bitset<16>, int> key = {cur.opened, cur.node};
            auto it = memo.find(key);
            if (it != memo.end() && it->second >= cur.score)
                continue;

            memo[key] = cur.score;

            result = std::max(result, cur.score);

            if (!cur.opened[cur.node] && rates[cur.node] > 0) {
                auto next = cur;
                next.opened[cur.node] = true;
                next.score += (rates[cur.node] * (i - 1));
                q.push_back(next);
            }

            for (int next: graph[cur.node]) {
                q.push_back({next, cur.score, cur.opened});
            }
        }
    }
    return result;
}

struct IndexGen {
    auto operator()(const std::string &name) {
        auto it = indexes.find(name);
        if (it != indexes.end())
            return it->second;
        return indexes[name] = idx++;
    };

    int size() {
        return idx;
    }

private:
    int idx = 0;
    std::map<std::string, int> indexes;
};

void part1() {

    IndexGen index;

    std::vector<int> rates(1000);
    std::vector<std::vector<int>> graph(1000);
    for (const auto &line: read()) {
        Node node = parse(line);
        rates[index(node.from)] = node.rate;
        for (const auto &elem: node.to)
            graph[index(node.from)].push_back(index(elem));
    }
    rates.resize(index.size());
    graph.resize(index.size());

    auto result = bfs(graph, rates, index("AA"), 30, {});
    std::cout << "Part1: " << result << std::endl;
}

std::map<std::bitset<16>, int64_t>
find_pairs(const std::vector<std::vector<int>> &graph, const std::vector<int> &rates, int start, int minutes) {
    std::map<std::bitset<16>, int64_t> result;
    std::unordered_map<std::pair<std::bitset<16>, int64_t>, int64_t> memo;
    std::deque<State> q = {{start, 0, {}}};
    for (int i = minutes; i > 0; --i) {
        int size = q.size();
        while (size-- > 0) {
            const auto cur = q.front();
            q.pop_front();

            std::pair<std::bitset<16>, int> key = {cur.opened, cur.node};
            auto it = memo.find(key);
            if (it != memo.end() && it->second >= cur.score)
                continue;

            memo[key] = cur.score;

            result[cur.opened] = std::max(result[cur.opened], cur.score);

            if (!cur.opened[cur.node] && rates[cur.node] > 0) {
                auto next = cur;
                next.opened[cur.node] = true;
                next.score += (rates[cur.node] * (i - 1));
                q.push_back(next);
            }

            for (int next: graph[cur.node]) {
                q.push_back({next, cur.score, cur.opened});
            }
        }
    }
    return result;
}


void part2() {

    IndexGen index;
    std::vector<int> rates(1000);
    std::vector<std::vector<int>> graph(1000);
    for (const auto &line: read()) {
        Node node = parse(line);
        rates[index(node.from)] = node.rate;
        for (const auto &elem: node.to)
            graph[index(node.from)].push_back(index(elem));
    }
    rates.resize(index.size());
    graph.resize(index.size());

    auto start_idx = index("AA");
    auto possible_comps = find_pairs(graph, rates, start_idx, 26);

    int64_t result = 0;
    for (const auto &elem: possible_comps) {
        auto elephant = bfs(graph, rates, start_idx, 26, elem.first);
        result = std::max(result, elem.second + elephant);
    }

    std::cout << "Part2: " << result << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
