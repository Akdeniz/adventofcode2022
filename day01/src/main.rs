use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

fn main() {

    let file_path = "/Users/ecem/CLionProjects/adventofcode2022/day01/day1.txt";

    let mut sums : Vec<i64> = vec![0];
    if let Ok(lines) = read_lines(file_path) {
        for line in lines {
            if let Ok(ip) = line {
                if let Ok(value) = ip.parse::<i64>() {
                    *sums.last_mut().unwrap() += value;
                } else {
                    sums.push(0);
                }
            }
        }
    }

    sums.sort_by(|a,b| b.cmp(a));
    println!("Part1: {}", sums.first().unwrap());
    println!("Part2: {}", sums[0..3].iter().sum::<i64>());
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>> where P: AsRef<Path> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

