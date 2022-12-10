use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;
use scanf::sscanf;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>> where P: AsRef<Path> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn read() -> Vec<String> {
    let file_path = "input.txt";
    let Ok(lines) = read_lines(file_path) else {
        return vec!();
    };

    return lines.into_iter().map(|x| x.unwrap()).collect();
}

fn parse(line: &str) -> (usize, usize, usize) {
    let mut count :usize = 0;
    let mut from :usize = 0;
    let mut to :usize = 0;

    // move 1 from 5 to 6
    let Ok(_) = sscanf!(&line, "move {} from {} to {}", count, from, to) else {
        panic!("Line does not match");
    };
    (count, from, to)
}

fn part1(input: &Vec<&str>) {

    let mut stacks: Vec<String> = input.iter().map(|&x| x.into()).collect();

    for x in read() {
        let (count,from,to) = parse(&*x);

        for _ in 0..count {
            let c = stacks[from].pop().unwrap();
            stacks[to].push(c);
        }
    }

    let mut result = String::new();
    for i in 1..stacks.len() {
        result.push(stacks[i].chars().last().unwrap());
    }

    println!("Part1: {}", result)
}

fn part2(input: &Vec<&str>) {

    let mut stacks: Vec<String> = input.iter().map(|&x| x.into()).collect();

    for x in read() {
        let (count,from,to) = parse(&*x);

        let len = stacks[from].len();
        let s = stacks[from].drain(len-count..len).collect::<String>();
        stacks[to].push_str(&*s);
    }

    let result = stacks[1..stacks.len()].iter().map(|x| x.chars().last().unwrap()).collect::<String>();
    println!("Part2: {}", result)
}

fn main() {

    /*
   [C]             [L]         [T]
   [V] [R] [M]     [T]         [B]
   [F] [G] [H] [Q] [Q]         [H]
   [W] [L] [P] [V] [M] [V]     [F]
   [P] [C] [W] [S] [Z] [B] [S] [P]
[G] [R] [M] [B] [F] [J] [S] [Z] [D]
[J] [L] [P] [F] [C] [H] [F] [J] [C]
[Z] [Q] [F] [L] [G] [W] [H] [F] [M]
1   2   3   4   5   6   7   8   9
    */

    let stacks = vec![
        "",
        "ZLG",
        "QLRPWFWC",
        "FPMCLGR",
        "LFBWPHM",
        "GCFSVQ",
        "WHJZMQTL",
        "HFSBV",
        "FJZS",
        "MCDPFHBT"
    ];

    part1(&stacks);
    part2(&stacks);
}


