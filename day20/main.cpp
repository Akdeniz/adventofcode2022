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

std::list<int64_t>::iterator nth(std::list<int64_t> &numbers, std::list<int64_t>::iterator it, int x) {
    while (x-- > 0) {
        it = std::next(it);
        if (it == numbers.end())
            it = numbers.begin();
    }
    return it;
}

std::list<int64_t>::iterator
find_insertion_point(std::list<int64_t> &numbers, std::list<int64_t>::iterator it, int diff) {
    while (diff != 0) {
        if (diff < 0) {
            if (it == numbers.begin())
                it = numbers.end();
            it = std::prev(it);
            diff++;
            if (diff == 0 && it == numbers.begin())
                it = numbers.end();
        } else {
            if (it == numbers.end())
                it = numbers.begin();
            it = std::next(it);
            diff--;
        }
    }
    return it;
}

void part1() {

    std::list<int64_t> numbers;
    std::vector<std::list<int64_t>::iterator> it_list;

    for (const auto &line: read()) {
        it_list.push_back(numbers.insert(numbers.end(), std::stoll(line)));
    }

    for (auto &it: it_list) {
        auto current = *it;
        it = numbers.erase(it);
        auto insertion_it = find_insertion_point(numbers, it, current);
        it = numbers.insert(insertion_it, current);
    }

    auto it = std::find(numbers.begin(), numbers.end(), 0);
    auto a = nth(numbers, it, 1000);
    auto b = nth(numbers, a, 1000);
    auto c = nth(numbers, b, 1000);

    std::cout << "Part1: " << (*a + *b + *c) << std::endl;
}

void part2() {

    const int64_t decryption_key = 811589153;
    std::list<int64_t> numbers;
    std::vector<std::list<int64_t>::iterator> it_list;

    for (const auto &line: read()) {
        it_list.push_back(numbers.insert(numbers.end(), std::stoll(line) * decryption_key));
    }

    const int64_t mod_value = int64_t(numbers.size()) - 1;
    for (int i = 0; i < 10; ++i) {
        for (auto &it: it_list) {
            auto current = *it;
            it = numbers.erase(it);
            auto insertion_it = find_insertion_point(numbers, it, current % mod_value);
            it = numbers.insert(insertion_it, current);
        }

    }

    auto it = std::find(numbers.begin(), numbers.end(), 0);
    auto a = nth(numbers, it, 1000);
    auto b = nth(numbers, a, 1000);
    auto c = nth(numbers, b, 1000);

    std::cout << "Part2: " << (*a + *b + *c) << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
