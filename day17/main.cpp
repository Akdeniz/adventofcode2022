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

struct Direction {
    Direction() : data(read().front()) {}

    auto operator()() {
        auto c = data[idx++];
        idx %= data.size();
        return c;
    }

    int64_t size() {
        return data.size();
    }

    int64_t index() {
        return idx;
    }

private:
    std::string data;
    int idx = 0;
};

/*
####  .#.  ..#  #  ##
      ###  ..#  #  ##
      .#.  ###  #
                #
*/
struct Offset {

    Offset() {
        offsets.push_back({{0, 0},{0, 1},{0, 2},{0, 3}});
        offsets.push_back({{0, 1},{1, 0},{1, 1},{1, 2},{2, 1}});
        offsets.push_back({{0, 0},{0, 1},{0, 2},{1, 2},{2, 2}});
        offsets.push_back({{0, 0},{1, 0},{2, 0},{3, 0}});
        offsets.push_back({{0, 0},{0, 1},{1, 0},{1, 1}});
    }

    auto operator()() {
        auto c = offsets[idx++];
        idx %= offsets.size();
        return c;
    }

    int64_t index() {
        return idx;
    }

    int64_t size() {
        return offsets.size();
    }

private:
    std::vector<std::vector<std::pair<int, int>>> offsets; // left down offsets
    int idx = 0;
};

struct Shape {
    Shape(std::pair<int, int> lbpos, std::vector<std::pair<int, int>> offsets) : lbpos(lbpos),
                                                                                 offsets(std::move(offsets)) {}

    bool go(const std::set<std::pair<int, int>> &points, const std::pair<int,int>& diff)
    {
        for (const auto &offset: offsets) {
            std::pair<int, int> pos = {offset.first + lbpos.first+diff.first, offset.second + lbpos.second +diff.second};
            if (pos.second < 0 || pos.second>=7 || pos.first<0 || points.count(pos) > 0)
                return false;
        }
        lbpos.first+=diff.first;
        lbpos.second+=diff.second;
        return true;
    }

    std::pair<int, int> lbpos;
    std::vector<std::pair<int, int>> offsets;
};

void simulate_drop(Direction &next_dir, Shape &shape, const std::set<std::pair<int, int>> &points) {
    do {
        auto dir = next_dir();
        if (dir == '>') {
            shape.go(points, {0, 1});
        } else {
            assert(dir == '<');
            shape.go(points, {0, -1});
        }
    } while(shape.go(points, {-1, 0}));
}

int64_t calculate_max_height(int64_t count)
{
    std::set<std::pair<int, int>> points;
    Direction next_dir;
    Offset next_offset;
    int max_height = -1;
    for (int i = 0; i < count; ++i) {
        Shape shape({max_height + 4, 2}, next_offset());
        simulate_drop(next_dir, shape, points);
        for (const auto &offset: shape.offsets) {
            std::pair<int, int> pos = {offset.first + shape.lbpos.first, offset.second + shape.lbpos.second};
            max_height = std::max(max_height, pos.first);
            points.insert(pos);
        }
    }
    return max_height+1;
}

void part1() {
    std::cout << "Part1: " << calculate_max_height(2022) << std::endl;
}

auto find_pattern() {
    std::set<std::pair<int, int>> points;
    Direction next_dir;

    Offset next_offset;
    int max_height = -1;
    std::array<int, 7> maxincols = {-1, -1, -1, -1, -1, -1, -1};
    std::array<int, 7> birdseye = {0, 0, 0, 0, 0, 0, 0};

    std::map<std::tuple<std::array<int, 7>, int64_t, int64_t>, std::pair<int64_t, int64_t>> cached;
    cached[{birdseye, next_dir.index(), next_offset.index()}] = {-1, max_height};

    for (int i = 0;; ++i) {
        Shape shape({max_height + 4, 2}, next_offset());
        simulate_drop(next_dir, shape, points);
        for (const auto &offset: shape.offsets) {
            std::pair<int, int> pos = {offset.first + shape.lbpos.first, offset.second + shape.lbpos.second};
            max_height = std::max(max_height, pos.first);
            maxincols[pos.second] = std::max(maxincols[pos.second], pos.first);
            points.insert(pos);
        }

        for (int k = 0; k < birdseye.size(); ++k) {
            birdseye[k] = max_height - maxincols[k];
        }

        if (cached.count({birdseye, next_dir.index(), next_offset.index()})) {
            auto prev = cached[{birdseye, next_dir.index(), next_offset.index()}];
            return std::make_tuple(prev.first, i - prev.first, max_height - prev.second);
        }
        cached[{birdseye, next_dir.index(), next_offset.index()}] = {i, max_height};
    }
}

void part2() {
    int64_t count = 1000000000000;
    auto pattern = find_pattern();
    const int64_t iterate = std::get<0>(pattern) + (count - std::get<0>(pattern)) % std::get<1>(pattern);
    const int64_t pattern_count = (count - std::get<0>(pattern)) / std::get<1>(pattern);
    int64_t result = std::get<2>(pattern) * pattern_count;

    std::cout << "Part2: " << result + calculate_max_height(iterate) << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
