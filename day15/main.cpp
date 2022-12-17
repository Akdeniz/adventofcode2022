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

struct Node
{
    std::pair<int,int> sensor;
    std::pair<int,int> beacon;
};
Node parse(const std::string &data) {

    // Sensor at x=3556832, y=3209801: closest beacon is at x=3520475, y=3164417
    std::regex base_regex("Sensor at x=(-?[0-9]+), y=(-?[0-9]+): closest beacon is at x=(-?[0-9]+), y=(-?[0-9]+)");

    std::smatch base_match;
    auto res = std::regex_match(data, base_match, base_regex);
    assert(res);
    std::pair<int,int> sensor = {std::stoi(base_match[1]), std::stoi(base_match[2])};
    std::pair<int,int> beacon = {std::stoi(base_match[3]), std::stoi(base_match[4])};
    return {sensor, beacon};
}


void part1() {

    std::vector<Node> nodes;
    for(const auto& line: read())
    {
        nodes.push_back(parse(line));
    }

    const int line_y = 2000000;
    std::set<int> covered; // y=2000000
    for(const auto& node: nodes)
    {
        int distance = std::abs(node.sensor.first-node.beacon.first) + std::abs(node.sensor.second-node.beacon.second);


        int cancover_in_x = distance - std::abs(line_y-node.sensor.second);
        if(cancover_in_x<0)
            continue;

        int center = node.sensor.first;
        for(int i=center-cancover_in_x; i<=center+cancover_in_x; ++i)
        {
            covered.insert(i);
        }
    }

    std::set<int> beacons_in_y;
    for(const auto& node: nodes)
    {
        if(node.beacon.second==line_y)
            beacons_in_y.insert(node.beacon.first);
    }

    std::cout << "Part1: " << covered.size()-beacons_in_y.size() << std::endl;
}

bool covered(const std::vector<Node>& nodes, const std::pair<int,int>& pos)
{
    for(const auto& node: nodes) {
        int distance = std::abs(node.sensor.first - node.beacon.first) +
                       std::abs(node.sensor.second - node.beacon.second);

        int pos_distance = std::abs(node.sensor.first-pos.first) + std::abs(node.sensor.second-pos.second);
        if(pos_distance<=distance)
            return true;
    }
    return false;
}

void part2() {

    std::vector<Node> nodes;
    for(const auto& line: read())
    {
        nodes.push_back(parse(line));
    }

    int upper_limit = 4000000;
    std::set<std::pair<int,int>> positions;
    for(const auto& node: nodes) {
        int distance = std::abs(node.sensor.first - node.beacon.first) +
                       std::abs(node.sensor.second - node.beacon.second);

        for(int i=0; i <= distance; ++i)
        {
            std::pair<int,int> left = {node.sensor.first+i, node.sensor.second-(distance-i+1)};
            if(left.first>=0 && left.first<=upper_limit && left.second>=0 && left.second<=upper_limit)
                positions.insert(left);
            std::pair<int,int> right = {node.sensor.first+i, node.sensor.second+(distance-i+1)};
            if(right.first>=0 && right.first<=upper_limit && right.second>=0 && right.second<=upper_limit)
                positions.insert(right);
        }
    }

    std::cout << "positions: " <<positions.size() << std::endl;

    for(const auto& pos: positions)
    {
        if(!covered(nodes, pos))
        {
            std::cout << pos.first << "," << pos.second << std::endl;
            std::cout << "Part1: " << int64_t(pos.first)*upper_limit+pos.second << std::endl;
        }
    }
}

int main() {
    //part1();
    part2();
    return 0;
}
