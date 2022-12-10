use std::cell::RefCell;
use std::fs::File;
use std::io::{self, BufRead};
use std::ops::Deref;
use std::path::Path;
use std::rc::Rc;
use std::vec;
use scanf::sscanf;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>> where P: AsRef<Path> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn read() -> Vec<String> {
    let file_path = "/Users/ecem/CLionProjects/adventofcode2022/day10/day10.txt";
    let Ok(lines) = read_lines(file_path) else {
        return vec!();
    };

    return lines.into_iter().map(|x| x.unwrap()).collect();
}


fn part1() {
    let mut result = 0;
    let mut x = 1;
    let mut cycle = 0;
    let mut next_cycle = 20;

    for line in read() {
        let tokens = line.split(' ').collect::<Vec<&str>>();

        match tokens[0] {
            "addx" => {
                let value = tokens[1].parse::<i32>().unwrap();
                cycle += 2;
                if next_cycle <= cycle {
                    result += next_cycle * x;
                    next_cycle += 40;
                }

                x += value;
            }
            "noop" => {
                cycle += 1;
                if next_cycle <= cycle {
                    result += next_cycle * x;
                    next_cycle += 40;
                }
            }
            op => panic!("unknown command {}", op)
        }
    }

    println!("Part1: {}", result)
}

fn check(x: i32, cycle: &mut i32, crt: &mut Vec<char>) {
    let c = *cycle;
    if x - 1 <= (c % 40) && (c % 40) <= x + 1 {
        crt[c as usize] = '#';
    }
    *cycle += 1;
}

fn part2() {
    let mut x = 1;
    let mut cycle = 0;
    let mut crt = vec![' '; 240];

    for line in read() {
        let tokens = line.split(' ').collect::<Vec<&str>>();

        match tokens[0] {
            "addx" => {
                let value = tokens[1].parse::<i32>().unwrap();
                check(x, &mut cycle, &mut crt);
                check(x, &mut cycle, &mut crt);
                x += value;
            }
            "noop" => {
                check(x, &mut cycle, &mut crt);
            }
            op => panic!("unknown command {}", op)
        }
    }

    println!("Part2:");
    for chunk in crt.chunks(40) {
        println!("{}", chunk.iter().collect::<String>())
    }
}

fn main() {
    part1();
    part2();
}


