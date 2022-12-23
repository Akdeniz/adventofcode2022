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

std::pair<std::vector<std::string>, std::string> parse() {
    std::vector<std::string> table = read();
    std::string directions = table.back();
    table.pop_back();

    size_t maxlen = 0;
    for(const auto& row: table)
        maxlen = std::max(maxlen, row.size());

    for(auto& row: table)
        row.append(maxlen-row.size(), ' ');

    return {table, directions};
}

struct Pos{ int x,y; };

Pos move(const std::vector<std::string> &table, char dir, const std::vector<std::pair<int, int>> &row_limits,
          const std::vector<std::pair<int, int>> &col_limits, Pos pos, int count) {

    while(count-- > 0) {
        switch (dir) {
            case '>': {
                auto next = Pos{pos.x, pos.y + 1};
                if (pos.y == row_limits[pos.x].second || table[next.x][next.y] == ' ')
                    next = Pos{pos.x, row_limits[pos.x].first};

                if (table[next.x][next.y] == '#')
                    return pos;
                assert(table[next.x][next.y] == '.');
                pos = next;
            }
                break;
            case '<': {
                auto next = Pos{pos.x, pos.y - 1};
                if (pos.y == row_limits[pos.x].first || table[next.x][next.y] == ' ')
                    next = Pos{pos.x, row_limits[pos.x].second};

                if (table[next.x][next.y] == '#')
                    return pos;
                assert(table[next.x][next.y] == '.');
                pos = next;
            }
                break;
            case 'v': {
                auto next = Pos{pos.x + 1, pos.y};
                if (pos.x == col_limits[pos.y].second || table[next.x][next.y] == ' ')
                    next = Pos{col_limits[pos.y].first, pos.y};

                if (table[next.x][next.y] == '#')
                    return pos;
                assert(table[next.x][next.y] == '.');
                pos = next;
            }
                break;
            case '^': {
                auto next = Pos{pos.x - 1, pos.y};
                if (pos.x == col_limits[pos.y].first || table[next.x][next.y] == ' ')
                    next = Pos{col_limits[pos.y].second, pos.y};

                if (table[next.x][next.y] == '#')
                    return pos;
                assert(table[next.x][next.y] == '.');
                pos = next;
            }
                break;
        }
    }
    return pos;
}

void part1() {

    std::vector<std::string> table;
    std::string directions;
    std::tie(table, directions) = parse();

    std::vector<std::pair<int, int>> row_limits;
    for (int i = 0; i < table.size(); ++i)
        row_limits.emplace_back(table[i].find_first_not_of(' '), table[i].find_last_not_of(' '));

    std::vector<std::pair<int, int>> col_limits;
    for (int i = 0; i < table[0].size(); ++i) {
        int u = 0;
        for (; u + 1 < table.size() && table[u][i] == ' '; ++u);
        int l = table.size() - 1;
        for (; l > 0 && table[l][i] == ' '; --l);
        col_limits.emplace_back(u, l);
    }

    static std::string to_right = ">v<^>";
    static std::string to_left = "^<v>^";

    char current_dir = '>';
    Pos pos = {0, row_limits[0].first};

    for (int i = 0; i < directions.size();) {
        if (std::isdigit(directions[i])) {
            int count = 0;
            while (i < directions.size() && std::isdigit(directions[i]))
                count = count * 10 + directions[i++] - '0';
            // move
            pos = move(table,current_dir,row_limits,col_limits,pos,count);
        } else {
            // direction change

            char lr = directions[i++];
            if (lr == 'R')
                current_dir = to_right[to_right.find(current_dir) + 1];
            else {
                assert(lr == 'L');
                current_dir = to_left[to_left.find(current_dir) + 1];
            }
        }
    }

    std::cout << pos.x << "," << pos.y << "," << current_dir << std::endl;
    std::cout << "Part1: " << (1000*(pos.x+1)+4*(pos.y+1)+to_right.find(current_dir)) << std::endl;
}


std::vector<std::string> parse_face(const std::vector<std::string>& table, Pos tl, int size) {
    std::vector<std::string> result;
    for(int i=0; i < size; ++i)
        result.push_back(table[tl.x+i].substr(tl.y, size));
    return result;
}


std::pair<std::string, Pos> face_move(const std::unordered_map<char, std::vector<std::string>>& faces, const std::unordered_map<std::string, std::pair<std::string, std::function<Pos(Pos)>>>& edges, std::string face_dir, Pos pos, int count) {

    int face_size = faces.find(face_dir[0])->second.size();
    while(count-- > 0) {
        const auto& edge = edges.find(face_dir)->second;

        switch (face_dir[1]) {
            case '>': {
                auto next = Pos{pos.x, pos.y + 1};
                auto next_face_dir = face_dir;
                if (pos.y+1==face_size) {
                    next = edge.second(pos);
                    next_face_dir = edge.first;
                }

                const auto& face = faces.find(next_face_dir[0])->second;

                if (face[next.x][next.y] == '#')
                    return {face_dir, pos};
                assert(face[next.x][next.y] == '.');
                pos = next;
                face_dir = next_face_dir;
            }
                break;
            case '<': {
                auto next = Pos{pos.x, pos.y - 1};
                auto next_face_dir = face_dir;
                if (pos.y==0) {
                    next = edge.second(pos);
                    next_face_dir = edge.first;
                }

                const auto& face = faces.find(next_face_dir[0])->second;

                if (face[next.x][next.y] == '#')
                    return {face_dir, pos};
                assert(face[next.x][next.y] == '.');
                pos = next;
                face_dir = next_face_dir;
            }
                break;
            case 'v': {
                auto next = Pos{pos.x + 1, pos.y};
                auto next_face_dir = face_dir;
                if (pos.x+1==face_size) {
                    next = edge.second(pos);
                    next_face_dir = edge.first;
                }

                const auto& face = faces.find(next_face_dir[0])->second;

                if (face[next.x][next.y] == '#')
                    return {face_dir, pos};
                assert(face[next.x][next.y] == '.');
                pos = next;
                face_dir = next_face_dir;
            }
                break;
            case '^': {
                auto next = Pos{pos.x - 1, pos.y};
                auto next_face_dir = face_dir;
                if (pos.x==0) {
                    next = edge.second(pos);
                    next_face_dir = edge.first;
                }

                const auto& face = faces.find(next_face_dir[0])->second;

                if (face[next.x][next.y] == '#')
                    return {face_dir, pos};
                assert(face[next.x][next.y] == '.');
                pos = next;
                face_dir = next_face_dir;
            }
                break;
        }
    }
    return {face_dir, pos};
}

void part2() {

    std::vector<std::string> table;
    std::string directions;
    std::tie(table, directions) = parse();

    std::unordered_map<char, Pos> offsets;
    offsets['A'] = {0, 50};
    offsets['B'] = {0, 100};
    offsets['C'] = {50, 50};
    offsets['D'] = {100, 50};
    offsets['E'] = {100, 0};
    offsets['F'] = {150, 0};

    std::unordered_map<char, std::vector<std::string>> faces;
    for(const auto& elem: offsets)
        faces[elem.first] = parse_face(table, elem.second, 50);

    std::unordered_map<std::string, std::pair<std::string, std::function<Pos(Pos)>>> edges;
    edges["A^"] = {"F>", [](Pos pos){ return Pos{pos.y, pos.x}; }};
    edges["A<"] = {"E>", [](Pos pos){ return Pos{49-pos.x, pos.y}; }};
    edges["Av"] = {"Cv", [](Pos pos){ return Pos{0, pos.y}; }};
    edges["A>"] = {"B>", [](Pos pos){ return Pos{pos.x, 0}; }};

    edges["B^"] = {"F^", [](Pos pos){ return Pos{49, pos.y}; }};
    edges["B<"] = {"A<", [](Pos pos){ return Pos{pos.x, 49}; }};
    edges["Bv"] = {"C<", [](Pos pos){ return Pos{pos.y, pos.x}; }};
    edges["B>"] = {"D<", [](Pos pos){ return Pos{49-pos.x, pos.y}; }};

    edges["C^"] = {"A^", [](Pos pos){ return Pos{49, pos.y}; }};
    edges["C<"] = {"Ev", [](Pos pos){ return Pos{pos.y, pos.x}; }};
    edges["Cv"] = {"Dv", [](Pos pos){ return Pos{0, pos.y}; }};
    edges["C>"] = {"B^", [](Pos pos){ return Pos{pos.y, pos.x}; }};

    edges["D^"] = {"C^", [](Pos pos){ return Pos{49, pos.y}; }};
    edges["D<"] = {"E<", [](Pos pos){ return Pos{pos.x, 49}; }};
    edges["Dv"] = {"F<", [](Pos pos){ return Pos{pos.y, pos.x}; }};
    edges["D>"] = {"B<", [](Pos pos){ return Pos{49-pos.x, pos.y}; }};

    edges["E^"] = {"C>", [](Pos pos){ return Pos{pos.y, pos.x}; }};
    edges["E<"] = {"A>", [](Pos pos){ return Pos{49-pos.x, pos.y}; }};
    edges["Ev"] = {"Fv", [](Pos pos){ return Pos{0, pos.y}; }};
    edges["E>"] = {"D>", [](Pos pos){ return Pos{pos.x, 0}; }};

    edges["F^"] = {"E^", [](Pos pos){ return Pos{49, pos.y}; }};
    edges["F<"] = {"Av", [](Pos pos){ return Pos{pos.y, pos.x}; }};
    edges["Fv"] = {"Bv", [](Pos pos){ return Pos{0, pos.y}; }};
    edges["F>"] = {"D^", [](Pos pos){ return Pos{pos.y, pos.x}; }};


    static std::string to_right = ">v<^>";
    static std::string to_left = "^<v>^";

    std::string face_dir = "A>";
    Pos pos = {0, 0};

    for (int i = 0; i < directions.size();) {
        if (std::isdigit(directions[i])) {
            int count = 0;
            while (i < directions.size() && std::isdigit(directions[i]))
                count = count * 10 + directions[i++] - '0';
            // move
            std::tie(face_dir, pos) = face_move(faces,edges, face_dir,pos,count);
        } else {
            // direction change
            char lr = directions[i++];
            if (lr == 'R')
                face_dir = std::string()+face_dir[0]+to_right[to_right.find(face_dir[1]) + 1];
            else {
                assert(lr == 'L');
                face_dir = std::string()+face_dir[0]+to_left[to_left.find(face_dir[1]) + 1];
            }
        }
    }

    std::cout << pos.x << "," << pos.y << "," << face_dir << std::endl;
    const auto& offset = offsets.find(face_dir[0])->second;

    std::cout << "Part2: " << (1000*(offset.x+pos.x+1)+4*(offset.y+pos.y+1)+to_right.find(face_dir[1])) << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
