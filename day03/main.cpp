#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>

auto read()
{
    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;
    std::vector<std::string> result;
    while(std::getline(ifs, line))
    {
        if(line.empty())
            continue;
        result.push_back(line);
    }
    return result;
}

int64_t convert(char c)
{
    if(c>='a' && c<='z')
    {
        return (c-'a'+1);
    }

    assert(c>='A' && c<='Z');
    return (c-'A'+27);
}

void part1()
{
    int64_t result = 0;
    for(const auto& line: read())
    {
        const int len = line.size();
        std::set<char> first(line.begin(), line.begin()+len/2);
        std::set<char> second(line.begin()+len/2, line.end());

        std::vector<char> intersection;
        std::set_intersection(first.begin(), first.end(),second.begin(), second.end(), std::back_inserter(intersection));
        assert(intersection.size()==1);
        const char c = intersection.front();
        result+=convert(c);
    }
    std::cout << "Part1: " << result << std::endl;
}

void part2()
{
    int64_t result = 0;
    const auto lines = read();

    for(int i=0; i < lines.size(); i+=3)
    {
        std::string first = lines[i];
        std::string second = lines[i+1];
        std::string third = lines[i+2];
        std::sort(first.begin(), first.end());
        std::sort(second.begin(), second.end());
        std::sort(third.begin(), third.end());

        std::string first_intersect;
        std::set_intersection(first.begin(), first.end(),second.begin(), second.end(), std::back_inserter(first_intersect));

        std::string second_intersect;
        std::set_intersection(third.begin(), third.end(),first_intersect.begin(), first_intersect.end(), std::back_inserter(second_intersect));

        result+=convert(*second_intersect.begin());
    }
    std::cout << "Part2: " << result << std::endl;
}

int main() {

    part1();
    part2();
    return 0;
}
