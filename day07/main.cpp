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

std::vector<std::string> split(const std::string& str)
{
    std::istringstream split(str);
    std::vector<std::string> tokens;
    for (std::string each; std::getline(split, each, ' '); tokens.push_back(each));
    return tokens;
}

auto read() {
    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;
    std::vector<std::string> result;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        result.push_back(line);
    }
    return result;
}


struct Dir
{
    Dir(std::string n): name(n){}
    std::string name;
    std::vector<std::pair<std::string, int>> files;
    std::vector<std::shared_ptr<Dir>> dirs;
};

int64_t get_size(const std::shared_ptr<Dir>& dir)
{
    int64_t result = 0;
    for(const auto& child: dir->dirs)
    {
        result+=get_size(child);
    }
    for(const auto& file: dir->files)
    {
        result+=file.second;
    }
    return result;
}

void traverse(const std::shared_ptr<Dir>& dir, std::function<void(const std::shared_ptr<Dir>&)> function)
{
    function(dir);
    for(const auto& child: dir->dirs)
        traverse(child, function);
}

std::shared_ptr<Dir> parse()
{
    std::vector<std::shared_ptr<Dir>> stack = {std::make_shared<Dir>("/")};

    for(const auto& line: read())
    {
        const auto tokens = split(line);
        if(tokens[0]=="$") // command
        {
            if(tokens[1]=="cd")
            {
                if(tokens[2]=="/")
                {
                    stack = {stack.front()};
                }
                else if(tokens[2]=="..")
                {
                    stack.pop_back();
                }
                else
                {
                    const auto& children = stack.back()->dirs;
                    auto it = std::find_if(children.begin(), children.end(),[&](const std::shared_ptr<Dir>& dir){
                        return dir->name==tokens[2];
                    });
                    assert(it!=children.end());
                    stack.push_back(*it);
                }
            }
            else if(tokens[1]=="ls")
            {
                stack.back()->dirs.clear();
                stack.back()->files.clear();
            }
        }
        else if(tokens[0]=="dir")
        {
            auto& children = stack.back()->dirs;
            auto it = std::find_if(children.begin(), children.end(),[&](const std::shared_ptr<Dir>& dir){
                return dir->name==tokens[1];
            });
            assert(it==children.end());
            children.push_back(std::make_shared<Dir>(tokens[1]));
        }
        else
        {
            stack.back()->files.push_back({tokens[1], std::stoi(tokens[0])});
        }
    }
    return stack.front();
}

void part1() {
    auto root = parse();
    int64_t total = 0;
    traverse(root, [&total](const std::shared_ptr<Dir>& dir){
        int64_t current_size = get_size(dir);
        if(current_size<=100000)
            total+=current_size;
    });
    std::cout << "Part1: " << total << std::endl;
}

void traverse2(const std::shared_ptr<Dir>& dir, int64_t& result, int64_t search_for)
{
    int64_t current_size = get_size(dir);
    if(current_size>=search_for)
        result=std::min(result, current_size);

    for(const auto& child: dir->dirs)
    {
        traverse2(child, result, search_for);
    }

}

void part2() {
    auto root = parse();

    int64_t result = std::numeric_limits<int32_t>::max();
    const int64_t disk_space = 70000000;
    const int64_t update_size = 30000000;
    auto space_needed = update_size-disk_space+get_size(root);

    traverse(root, [&](const std::shared_ptr<Dir>& dir){
        int64_t current_size = get_size(dir);
        if(current_size>=space_needed)
            result=std::min(result, current_size);
    });
    std::cout << "Part2: " << result << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
