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

static std::vector<std::array<int64_t, 3>> SIDES = {{1,  0,  0},
                                                    {0,  1,  0},
                                                    {0,  0,  1},
                                                    {-1, 0,  0},
                                                    {0,  -1, 0},
                                                    {0,  0,  -1}};

auto parse() {
    std::set<std::array<int64_t, 3>> points;
    for (const auto &elem: read()) {

        std::stringstream ss(elem);
        std::string token;
        std::vector<int64_t> axis;
        while (std::getline(ss, token, ','))
            axis.push_back(std::stoi(token));

        assert(axis.size() == 3);
        points.insert({axis[0], axis[1], axis[2]});
    }
    return points;
}

auto sides_covered(const std::set<std::array<int64_t, 3>> &points, const std::array<int64_t, 3> &point) {
    int sidecovered = 0;
    for (const auto &side: SIDES) {
        if (points.count({point[0] + side[0], point[1] + side[1], point[2] + side[2]}))
            ++sidecovered;
    }
    return sidecovered;
}

void part1() {

    auto points = parse();
    int64_t result = 0;
    for (const auto &point: points) {
        result += (6 - sides_covered(points, point));
    }

    std::cout << "Part1: " << result << std::endl;
}


void part2() {

    auto points = parse();

    std::array<std::pair<int64_t, int64_t>, 3> max_min;
    for (const auto &point: points) {
        max_min[0].first = std::max(max_min[0].first, point[0] + 1);
        max_min[0].second = std::min(max_min[0].second, point[0] - 1);

        max_min[1].first = std::max(max_min[1].first, point[1] + 1);
        max_min[1].second = std::min(max_min[1].second, point[1] - 1);

        max_min[2].first = std::max(max_min[2].first, point[2] + 1);
        max_min[2].second = std::min(max_min[2].second, point[2] - 1);
    }

    std::deque<std::array<int64_t, 3>> q;
    // x planes
    for (int y = max_min[1].second; y <= max_min[1].first; ++y)
        for (int z = max_min[2].second; z <= max_min[2].first; ++z) {
            q.push_back({max_min[0].first, y, z});
            q.push_back({max_min[0].second, y, z});
        }

    // y planes
    for (int x = max_min[0].second; x <= max_min[0].first; ++x)
        for (int z = max_min[2].second; z <= max_min[2].first; ++z) {
            q.push_back({x, max_min[1].first, z});
            q.push_back({x, max_min[1].second, z});
        }

    // z planes
    for (int x = max_min[0].second; x <= max_min[0].first; ++x)
        for (int y = max_min[1].second; y <= max_min[1].first; ++y) {
            q.push_back({x, y, max_min[2].first});
            q.push_back({x, y, max_min[2].first});
        }

    int64_t result = 0;
    std::set<std::array<int64_t, 3>> visited;
    while (!q.empty()) {
        auto cur = q.front();
        q.pop_front();

        if (!visited.insert(cur).second)
            continue;

        result += sides_covered(points, cur);

        for (const auto &side: SIDES) {
            std::array<int64_t, 3> newpoint = {cur[0] + side[0], cur[1] + side[1], cur[2] + side[2]};
            if (points.count(newpoint) || newpoint[0] < max_min[0].second || newpoint[0] > max_min[0].first ||
                newpoint[1] < max_min[1].second || newpoint[1] > max_min[1].first ||
                newpoint[2] < max_min[2].second || newpoint[2] > max_min[2].first)
                continue;
            q.push_back(newpoint);
        }
    }

    std::cout << "Part2: " << result << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
