use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;


fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>> where P: AsRef<Path> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn read() -> Vec<String> {
    let file_path = "/Users/ecem/CLionProjects/adventofcode2022/day04/day4.txt";
    let Ok(lines) = read_lines(file_path) else {
        return vec!();
    };

    return lines.into_iter().map(|x| x.unwrap()).collect();
}

fn parse(s: &str) -> (i64, i64) {
    let elems: Vec<&str> = s.split('-').collect();
    return (elems[0].parse::<i64>().unwrap(), elems[1].parse::<i64>().unwrap());
}

fn part1() {
    let result: i32 = read().iter().map(|s| {
        let elems: Vec::<&str> = s.split(',').collect();
        assert_eq!(elems.len(), 2);
        let a = parse(elems[0]);
        let b = parse(elems[1]);
        if (a.0 <= b.0 && a.1 >= b.1) || (b.0 <= a.0 && b.1 >= a.1) {
            return 1;
        }
        return 0;
    }).sum();

    println!("Part1: {}", result)
}

fn part2() {
    let result: i32 = read().iter().map(|s| {
        let elems: Vec::<&str> = s.split(',').collect();
        assert_eq!(elems.len(), 2);
        let a = parse(elems[0]);
        let b = parse(elems[1]);
        if a.1.min(b.1) >= a.0.max(b.0) {
            return 1;
        }
        return 0;
    }).sum();

    println!("Part2: {}", result)
}

fn main() {
    part1();
    part2();
}


