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

std::vector<std::string> split(const std::string &str, char c = ' ') {
    std::istringstream split(str);
    std::vector<std::string> tokens;
    for (std::string each; std::getline(split, each, c); tokens.push_back(each));
    return tokens;
}

struct Monkey {
    std::deque<int64_t> items;
    std::pair<char, std::string> op;
    int64_t test_div = 0;
    int64_t test_true = -1;
    int64_t test_false = -1;
    int64_t inspection_times = 0;
};

std::vector<Monkey> parse() {
    std::regex words_regex("Monkey ([0-9]+):\n"
                           "  Starting items: ([0-9, ]+)\n"
                           "  Operation: new = old ([*+]) ([0-9]+|old)\n"
                           "  Test: divisible by ([0-9]+)\n"
                           "    If true: throw to monkey ([0-9]+)\n"
                           "    If false: throw to monkey ([0-9]+)", std::regex::extended);

    auto data = read_all();

    std::vector<Monkey> monkeys;
    for (std::sregex_iterator it = std::sregex_iterator(data.begin(), data.end(), words_regex);
         it != std::sregex_iterator(); ++it) {
        monkeys.push_back({});
        std::smatch match = *it;
        auto items = split(match[2].str(), ',');
        std::transform(items.begin(), items.end(), std::back_inserter(monkeys.back().items),
                       [](std::string str) { return std::stoi(str); });
        monkeys.back().op = {match[3].str()[0], match[4].str()};
        monkeys.back().test_div = std::stoi(match[5]);
        monkeys.back().test_true = std::stoi(match[6]);
        monkeys.back().test_false = std::stoi(match[7]);
    }
    return monkeys;
}

void part1() {

    auto monkeys = parse();
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < monkeys.size(); ++j) {
            auto &monkey = monkeys[j];
            monkey.inspection_times += monkey.items.size();
            for (auto it = monkey.items.begin(); it != monkey.items.end();) {
                auto value = *it;
                it = monkeys[j].items.erase(it);

                int64_t right_hand = monkey.op.second == "old" ? value : std::stoi(monkey.op.second);
                if (monkey.op.first == '+')
                    value += right_hand;
                else if (monkey.op.first == '*')
                    value *= right_hand;
                else
                    std::abort();

                value /= 3;
                if (value % monkey.test_div == 0)
                    monkeys[monkey.test_true].items.push_back(value);
                else
                    monkeys[monkey.test_false].items.push_back(value);
            }
        }
    }

    std::vector<int64_t> inspection_times;
    for (const auto &monkey: monkeys)
        inspection_times.push_back(monkey.inspection_times);

    std::sort(inspection_times.begin(), inspection_times.end(), std::greater<int64_t>());
    std::cout << "Part1: " << inspection_times[0] * inspection_times[1] << std::endl;
}

void part2() {

    auto monkeys = parse();

    int64_t master_modulo = 1;
    for (const auto &monkey: monkeys) {
        master_modulo *= monkey.test_div;
    }
    for (int i = 0; i < 10000; ++i) {
        for (auto &monkey: monkeys) {
            monkey.inspection_times += monkey.items.size();
            for (auto it = monkey.items.begin(); it != monkey.items.end();) {
                auto value = *it % master_modulo + master_modulo;
                it = monkey.items.erase(it);

                auto right_hand = monkey.op.second == "old" ? value : std::stoi(monkey.op.second);
                if (monkey.op.first == '+') {
                    assert(std::numeric_limits<int64_t>::max() - right_hand > value);
                    value += right_hand;
                } else if (monkey.op.first == '*') {
                    assert(std::numeric_limits<int64_t>::max() / right_hand > value);
                    value *= right_hand;
                } else
                    std::abort();

                if (value % monkey.test_div == 0)
                    monkeys[monkey.test_true].items.push_back(value);
                else
                    monkeys[monkey.test_false].items.push_back(value);
            }
        }
    }

    std::vector<int64_t> inspection_times;
    for (const auto &monkey: monkeys)
        inspection_times.push_back(monkey.inspection_times);

    std::sort(inspection_times.begin(), inspection_times.end(), std::greater<int64_t>());
    std::cout << "Part2: " << inspection_times[0] * inspection_times[1] << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
