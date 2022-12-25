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
#include <array>


template<class T>
inline void hash_combine(std::size_t &seed, const T &v) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<class A, class B>
struct std::hash<std::pair<A, B>> {
    size_t operator()(const std::pair<A, B> &pair) const {
        std::size_t seed = 0;
        hash_combine(seed, pair.first);
        hash_combine(seed, pair.second);
        return seed;
    }
};

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

int64_t to_decimal(const std::string& s)
{
    int64_t result = 0;
    int64_t pow = 1;
    for(int64_t i=s.size()-1; i>=0; --i)
    {
        // =, -, 0, 1, and 2
        switch (s[i]) {
            case '=':
                result+=(pow*-2);
                break;
            case '-':
                result+=(pow*-1);
                break;
            case '0':
                break;
            case '1':
                result+=(pow*1);
                break;
            case '2':
                result+=(pow*2);
                break;
            default:
                std::abort();
        }
        pow*=5;
    }
    return result;
}

bool dfs(int64_t idx, std::string& result, int64_t diff)
{
    if(diff==0)
        return true;

    if(idx<0 || diff<0)
        return false;

    const int64_t pow = std::pow(5, idx);

    if(diff>=pow*5)
        return false;

    // =, -, 0, 1, and 2
    result[idx]='1';
    if(dfs(idx-1, result, diff-pow))
        return true;

    result[idx]='0';
    if(dfs(idx-1, result, diff-pow*2))
        return true;

    result[idx]='-';
    if(dfs(idx-1, result, diff-pow*3))
        return true;

    result[idx]='=';
    if(dfs(idx-1, result, diff-pow*4))
        return true;

    result[idx]='2';
    if(dfs(idx-1, result, diff))
        return true;

    return false;
}

std::string to_snafu(const int64_t decimal)
{
    std::string result;
    int64_t value = 0;
    {
        int64_t pow = 1;
        while (value < decimal) {
            result.push_back('2');
            value += (pow*2);
            pow*=5;
        }
    }
    auto success = dfs(result.size()-1,result,value-decimal);
    std::reverse(result.begin(), result.end());
    assert(success && decimal==to_decimal(result));
    return result;
}

void part1() {
    int64_t decimal = 0;
    for(const auto& line: read())
        decimal += to_decimal(line);

    std::cout << "Part1: " << to_snafu(decimal) << std::endl;
}


int main() {
    part1();
    return 0;
}
