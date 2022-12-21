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
    Node(int64_t value) : calculated(true), value(value) {
    }

    Node(char op, std::string left, std::string right) : op(op), calculated(false), value(0), left(std::move(left)),
                                                         right(std::move(right)) {
    }

    char op;
    bool calculated;
    int64_t value;
    std::string left, right;
};


std::pair<std::string, Node> parse(const std::string &line) {
    // qpmq: nzhs / wdmm
    static std::regex a_regex("([a-z]{4}): ([a-z]{4}) ([+-/*]) ([a-z]{4})");
    static std::regex b_regex("([a-z]{4}): (-?[0-9]+)");
    std::smatch match;
    if (std::regex_match(line, match, a_regex)) {
        return {match[1].str(), Node(match[3].str().front(), match[2].str(), match[4].str())};
    } else if (std::regex_match(line, match, b_regex)) {
        return {match[1].str(), Node(std::stoll(match[2]))};
    } else {
        std::abort();
    }
}

int64_t dfs(const std::unordered_map<std::string, Node> &tree, const std::string &key) {
    auto it = tree.find(key);
    assert(it != tree.end());
    const Node &node = it->second;

    if (node.calculated)
        return node.value;

    const auto l = dfs(tree, node.left);
    const auto r = dfs(tree, node.right);
    switch (node.op) {
        case '+':
            return l + r;
        case '-':
            return l - r;
        case '/':
            return l / r;
        case '*':
            return l * r;
        default:
            abort();
    }
}

void part1() {
    std::unordered_map<std::string, Node> tree;
    for (const auto &line: read()) {
        auto elem = parse(line);
        tree.emplace(elem.first, elem.second);
    }
    std::cout << "Part1: " << dfs(tree, "root") << std::endl;
}

bool find_human(const std::unordered_map<std::string, Node> &tree, const std::string &key, std::vector<bool> &path) {
    if (key == "humn")
        return true;

    auto it = tree.find(key);
    assert(it != tree.end());
    const Node &node = it->second;

    if (node.calculated)
        return false;

    path.push_back(true);
    if (find_human(tree, node.left, path))
        return true;

    path.back() = false;
    if (find_human(tree, node.right, path))
        return true;

    path.pop_back();
    return false;
}

void part2() {
    std::unordered_map<std::string, Node> tree;
    for (const auto &line: read()) {
        auto elem = parse(line);
        tree.emplace(elem.first, elem.second);
    }

    std::vector<bool> path;
    find_human(tree, "root", path);

    auto root = tree.find("root");
    int64_t value = dfs(tree, path.front() ? root->second.right : root->second.left);
    auto current = path.front() ? root->second.left : root->second.right;

    for (int i = 1; i < path.size(); ++i) {
        auto it = tree.find(current);
        assert(it != tree.end());
        const Node &node = it->second;

        int64_t subvalue = dfs(tree, path[i] ? node.right : node.left);
        current = path[i] ? node.left : node.right;

        switch (node.op) {
            case '+': {
                value = value - subvalue;
                break;
            }
            case '-': {
                if (path[i])
                    value = value + subvalue;
                else
                    value = subvalue - value;
                break;
            }
            case '/': {
                if (path[i])
                    value = value * subvalue;
                else
                    value = subvalue / value;
                break;
            }
            case '*': {
                value = value / subvalue;
                break;
            }
            default:
                abort();
        }
    }
    assert(current == "humn");
    std::cout << "Part2: " << value << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
