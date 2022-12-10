#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

enum class Move
{
    Rock = 0,
    Paper,
    Scissors
};

struct Turn
{
    Move op, you;
};

auto read()
{
    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;
    std::vector<Turn> result;
    while(std::getline(ifs, line))
    {
        if(line.empty())
            continue;

        // A for Rock, B for Paper, and C for Scissors
        // X for Rock, Y for Paper, and Z for Scissors
        result.push_back({static_cast<const Move>(line[0] - 'A'), static_cast<const Move>(line[2] - 'X')});
    }
    return result;
}

int score(Move op, Move you)
{
    // Rock defeats Scissors, Scissors defeats Paper, and Paper defeats Rock.
    // If both players choose the same shape, the round instead ends in a draw.
    int res = 0;
    if(op == you)
        res = 3;
    else if((you == Move::Rock && op == Move::Scissors) || (you == Move::Scissors && op == Move::Paper) || (you == Move::Paper && op == Move::Rock))
        res = 6;

    // 1 for Rock, 2 for Paper, and 3 for Scissors) plus
    // the res for the outcome of the round (0 if you lost, 3 if the round was a draw, and 6 if you won)
    return res + static_cast<int>(you) + 1;
}

void part1()
{

    int64_t result = 0;
    for(const auto& turn: read())
        result+=score(turn.op, turn.you);

    std::cout << "Part1: " << result << std::endl;
}

void part2()
{
    static std::vector<std::vector<Move>> table = {
            // lose, draw, win
            {Move::Scissors, Move::Rock,     Move::Paper}, // Rock
            {Move::Rock,     Move::Paper,    Move::Scissors,}, // Paper
            {Move::Paper,    Move::Scissors, Move::Rock}, // Scissors
    };

    int64_t result = 0;
    for(const auto& turn: read())
    {
        const auto you = table[static_cast<int>(turn.op)][static_cast<int>(turn.you)];
        result+= score(turn.op, you);
    }

    std::cout << "Part2: " << result << std::endl;
}

int main() {

    part1();
    part2();
    return 0;
}
