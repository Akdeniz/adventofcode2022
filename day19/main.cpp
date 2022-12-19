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

struct Blueprint {
    int id = 0;
    std::array<std::array<int, 4>, 4> require = {};
};

auto parse(const std::string &line) {

    // Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 4 ore. Each obsidian robot costs 2 ore and 11 clay. Each geode robot costs 2 ore and 7 obsidian.
    std::regex base_regex("Blueprint ([0-9]+): "
                          "Each ore robot costs ([0-9]+) ore. "
                          "Each clay robot costs ([0-9]+) ore. "
                          "Each obsidian robot costs ([0-9]+) ore and ([0-9]+) clay. "
                          "Each geode robot costs ([0-9]+) ore and ([0-9]+) obsidian.");

    std::smatch base_match;
    auto res = std::regex_match(line, base_match, base_regex);
    assert(res);

    Blueprint blueprint;
    blueprint.id = std::stoi(base_match[1]);

    blueprint.require[0] = {std::stoi(base_match[2]), 0, 0, 0};
    blueprint.require[1] = {std::stoi(base_match[3]), 0, 0, 0};
    blueprint.require[2] = {std::stoi(base_match[4]), std::stoi(base_match[5]), 0, 0};
    blueprint.require[3] = {std::stoi(base_match[6]), 0, std::stoi(base_match[7]), 0};

    return blueprint;
}

inline std::array<int, 4> add(const std::array<int, 4> &a, const std::array<int, 4> &b) {
    return {a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]};
}

inline std::array<int, 4> sub(const std::array<int, 4> &a, const std::array<int, 4> &b) {
    return {a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]};
}

inline bool canmake(const std::array<int, 4> &a, const std::array<int, 4> &b) {
    return a[0] >= b[0] && a[1] >= b[1] && a[2] >= b[2] && a[3] >= b[3];
}

int bfs(const Blueprint &blueprint, int minutes) {

    struct State {
        // ore, clay, obsidian, geode
        std::array<int, 4> mineral_count;
        std::array<int, 4> robot_count;
        std::array<bool, 3> could_make; // but decided not to
    };

    std::array<int, 4> max_robot_needed = {};
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            max_robot_needed[i] = std::max(max_robot_needed[i], blueprint.require[j][i]);

    std::deque<State> q = {State{{0,     0,     0, 0},
                                 {1,     0,     0, 0},
                                 {false, false, false}}};

    int result = 0;
    while (minutes-- >= 0) {
        int qsize = q.size();
        //std::cout << "("<<minutes<<","<<qsize<<"),"<<std::flush;
        while (qsize-- > 0) {
            const auto cur = q.front();
            q.pop_front();

            result = std::max(result, cur.mineral_count[3]);

            // prune a: if can make geode, make it
            if (canmake(cur.mineral_count, blueprint.require[3]))
                q.push_back(State{add(sub(cur.mineral_count, blueprint.require[3]), cur.robot_count),
                                  add(cur.robot_count, {0, 0, 0, 1}), cur.could_make});
            else {
                std::array<bool, 3> canmakearr = {canmake(cur.mineral_count, blueprint.require[0]),
                                                  canmake(cur.mineral_count, blueprint.require[1]),
                                                  canmake(cur.mineral_count, blueprint.require[2])};

                // prune b: if could make in previous turn but decided not to, then no point to do in this turn
                // prune c: since we can only make one robot at a time, if we reached max robot count, no need to make that kind of robot anymore
                if (!cur.could_make[2] && cur.robot_count[2] < max_robot_needed[2] && canmakearr[2])
                    q.push_back(State{add(sub(cur.mineral_count, blueprint.require[2]), cur.robot_count),
                                      add(cur.robot_count, {0, 0, 1, 0}), cur.could_make});

                if (!cur.could_make[1] && cur.robot_count[1] < max_robot_needed[1] && canmakearr[1])
                    q.push_back(State{add(sub(cur.mineral_count, blueprint.require[1]), cur.robot_count),
                                      add(cur.robot_count, {0, 1, 0, 0}), cur.could_make});

                if (!cur.could_make[0] && cur.robot_count[0] < max_robot_needed[0] && canmakearr[0])
                    q.push_back(State{add(sub(cur.mineral_count, blueprint.require[0]), cur.robot_count),
                                      add(cur.robot_count, {1, 0, 0, 0}), cur.could_make});


                q.push_back(State{add(cur.mineral_count, cur.robot_count), cur.robot_count, canmakearr});
            }
        }
    }
    //std::cout << std::endl;

    return result;
}

void part1() {

    std::vector<Blueprint> blueprints;
    for (const auto &line: read())
        blueprints.push_back(parse(line));


    int result = 0;
    for (const auto &blueprint: blueprints) {
        int res = bfs(blueprint, 24);
        result += (res * blueprint.id);
    }

    std::cout << "Part1: " << result << std::endl;
}

void part2() {

    std::vector<Blueprint> blueprints;
    for (const auto &line: read())
        blueprints.push_back(parse(line));

    int result = 1;
    for (int i = 0; i < 3; ++i) {
        int res = bfs(blueprints[i], 32);
        result *= res;
    }

    std::cout << "Part2: " << result << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
