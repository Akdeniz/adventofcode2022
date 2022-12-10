#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

int main() {

    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;

    std::vector<int64_t> sums = {0};
    while(std::getline(ifs, line))
    {
        if(line.empty())
            sums.push_back(0);
        else
            sums.back()+=std::stoll(line);
    }

    std::sort(sums.begin(), sums.end(), std::greater<int64_t>());

    std::cout << "Part1: " << sums.front() << std::endl;
    std::cout << "Part2: " << std::accumulate(sums.begin(), sums.begin()+std::min<int>(3, sums.size()), 0) << std::endl;
    return 0;
}
