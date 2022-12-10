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
    std::fstream ifs("/Users/ecem/CLionProjects/adventofcode2022/day09/day9.txt");
    std::string line;
    std::vector<std::string> result;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        result.push_back(line);
    }
    return result;
}

std::vector<std::string> split(const std::string& str)
{
    std::istringstream split(str);
    std::vector<std::string> tokens;
    for (std::string each; std::getline(split, each, ' '); tokens.push_back(each));
    return tokens;
}

std::pair<int,int> calculate_tail(std::pair<int,int> H, std::pair<int,int> T)
{
    int x_diff = std::abs(H.first-T.first);
    int y_diff = std::abs(H.second-T.second);

    int x_delta = x_diff==0?0:(H.first>T.first?1:-1);
    int y_delta = y_diff==0?0:(H.second>T.second?1:-1);

    if(x_diff>1 || y_diff>1)
    {
        return {T.first+x_delta, T.second+y_delta};
    }

    return T;
}

void part1() {

    std::set<std::pair<int,int>> tail_visited;
    std::pair<int,int> H = {0,0};
    std::pair<int,int> T = {0,0};
    tail_visited.insert(T);

    for(const auto& line: read())
    {
        auto tokens = split(line);

        int64_t steps = std::stoi(tokens[1]);

        std::pair<int,int> delta = {0, 0};
        switch (tokens[0][0]) {
            case 'R': delta = {0, 1}; break;
            case 'L': delta = {0, -1}; break;
            case 'U': delta = {-1, 0}; break;
            case 'D': delta = {1, 0}; break;
        }

        while(steps-->0)
        {
            H = {H.first+delta.first, H.second+delta.second};
            T = calculate_tail(H, T);
            tail_visited.insert(T);
        }
    }

    std::cout << "Part1: " << tail_visited.size() << std::endl;
}

void calculate_knots(std::vector<std::pair<int,int>>& knots)
{
    for(int i=1; i < knots.size(); ++i)
    {
        knots[i] = calculate_tail(knots[i-1], knots[i]);
    }
}

void part2() {

    std::set<std::pair<int,int>> tail_visited;

    std::vector<std::pair<int,int>> knots = {10, {0,0}};
    tail_visited.insert(knots.back());

    for(const auto& line: read())
    {
        auto tokens = split(line);

        int64_t steps = std::stoi(tokens[1]);

        std::pair<int,int> delta = {0, 0};
        switch (tokens[0][0]) {
            case 'R': delta = {0, 1}; break;
            case 'L': delta = {0, -1}; break;
            case 'U': delta = {-1, 0}; break;
            case 'D': delta = {1, 0}; break;
        }

        while(steps-->0)
        {
            knots[0] = {knots[0].first+delta.first, knots[0].second+delta.second};
            calculate_knots(knots);
            tail_visited.insert(knots.back());
        }
    }

    std::cout << "Part2: " << tail_visited.size() << std::endl;
}


int main() {
    part1();
    part2();
    return 0;
}
