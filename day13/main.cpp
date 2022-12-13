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
    Node(std::vector<Node> nodes) : data(std::move(nodes)) {}
    Node(int value) : data(value) {}

    bool is_int() const {
        return std::holds_alternative<int>(data);
    }

    std::vector<Node> &get_array() {
        return std::get<std::vector<Node>>(data);
    }

    const std::vector<Node> get_array() const {
        return std::get<std::vector<Node>>(data);
    }

    int& get_int() {
        return std::get<int>(data);
    }

    int get_int() const {
        return std::get<int>(data);
    }

    std::string to_str() const
    {
        std::stringstream result;
        if(is_int())
            result << get_int();
        else
        {
            const auto& array = get_array();
            result << '[';
            for(int i=0; i < array.size(); ++i) {
                result << array[i].to_str();
                if(i+1<array.size())
                    result << ',';
            }
            result << ']';
        }
        return result.str();
    }

private:
    std::variant<int, std::vector<Node>> data;
};

Node parse(const std::string& str) {

    Node root(std::vector<Node>{});
    std::vector<Node*> stack = {&root};
    std::optional<int> current_value;
    for(int i =0; i < str.size(); ++i)
    {
        if(str[i]=='[')
        {
            stack.back()->get_array().emplace_back(std::vector<Node>{});
            stack.push_back(&stack.back()->get_array().back());
            current_value = std::nullopt;
        }
        else if(str[i]==']'||str[i]==',')
        {
            if(current_value!=std::nullopt)
                stack.back()->get_array().emplace_back(*current_value);
            current_value = std::nullopt;
        }
        else {
            if(!current_value)
                current_value=0;
            *current_value*=10;
            *current_value+=(str[i]-'0');
        }

        if(str[i]==']')
            stack.pop_back();
    }
    return root.get_array().back();
}

int compare(const Node& a, const Node& b)
{
    if(a.is_int() && b.is_int())
    {
        if(a.get_int() == b.get_int())
            return  0;

        return a.get_int() < b.get_int() ? -1 : 1;
    }

    if(!a.is_int() && !b.is_int())
    {
        const auto& a_array  = a.get_array();
        const auto& b_array  = b.get_array();
        for(int i=0; i < std::min(a_array.size(), b_array.size()); ++i)
        {
            auto res = compare(a_array[i], b_array[i]);
            if(res != 0)
                return res;
        }
        if(a_array.size() == b_array.size())
            return 0;
        return a_array.size() < b_array.size() ? -1 : 1;
    }

    if(a.is_int() && !b.is_int())
    {
        return compare(Node(std::vector<Node>{a}), b);
    }

    assert(!a.is_int() && b.is_int());
    return compare(a, Node(std::vector<Node>{b}));
}

void part1() {
    auto table = read();

    int idx = 1;
    int result = 0;
    for(int i=0; i < table.size(); i+=2)
    {
        auto first = parse(table[i]);
        auto second = parse(table[i+1]);

        int res = compare(first, second);
        if(res!=1) {
            result += idx;
        }
        ++idx;
    }

    std::cout << "Part1: " << result << std::endl;
}


void part2() {
    auto table = read();

    std::vector<Node> nodes;
    nodes.push_back(parse("[[2]]"));
    nodes.push_back(parse("[[6]]"));

    for(int i=0; i < table.size(); i+=2)
    {
        nodes.push_back(parse(table[i]));
        nodes.push_back(parse(table[i+1]));
    }

    std::sort(nodes.begin(), nodes.end(), [](const Node& a, const Node& b){
        return compare(a, b)<0;
    });

    int x=0,y=0;
    for(int i=0; i < nodes.size(); ++i) {
        auto str = nodes[i].to_str();
        if(str=="[[2]]")
            x = i+1;
        if(str=="[[6]]")
            y = i+1;
    }

    std::cout << "Part2: " << x*y << std::endl;
}


int main() {
    part1();
    part2();
    return 0;
}
