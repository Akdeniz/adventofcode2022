use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;
use std::convert::TryFrom;

#[derive(Copy, Clone, PartialEq)]
enum Move { R=0, P=1, S=2}

impl TryFrom<i32> for Move {
    type Error = ();

    fn try_from(v: i32) -> Result<Self, Self::Error> {
        match v {
            x if x == Move::R as i32 => Ok(Move::R),
            x if x == Move::P as i32 => Ok(Move::P),
            x if x == Move::S as i32 => Ok(Move::S),
            _ => Err(()),
        }
    }
}

impl Move {
    pub fn index(&self) -> usize {
        *self as usize
    }
}

fn parse(s: String) -> (Move, Move) {
    let op_digit = s.as_bytes()[0] as i32;
    let op =  Move::try_from(op_digit-'A' as i32).unwrap();

    let you_digit = s.as_bytes()[2] as i32;
    let you = Move::try_from(you_digit-'X' as i32).unwrap();
    return (op, you)
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>> where P: AsRef<Path> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn read_moves() -> Vec<(Move, Move)> {
    let file_path = "input.txt";
    let mut moves : Vec<(Move, Move)> = vec!();
    let Ok(lines) = read_lines(file_path) else {
        return moves;
    };

    for line in lines {
        moves.push(parse(line.unwrap()));
    }
    return moves;
}

fn score(op: Move, you:Move) -> i32 {
    let mut res = 0;
    if op == you {
        res = 3;
    } else if you==Move::R && op==Move::S || you==Move::S && op==Move::P || you==Move::P && op==Move::R {
        res = 6;
    }
    return res + (you as i32) + 1;
}

fn part1() {
    let result :i32 = read_moves().into_iter().map(|m| score(m.0, m.1)).sum();

    println!("Part1: {}", result)
}

fn part2() {
    let table = vec![
        [Move::S, Move::R, Move::P], // R
        [Move::R, Move::P, Move::S], // P
        [Move::P, Move::S, Move::R]  // S
    ];

    let result :i32 = read_moves().into_iter().map(|m| {
        score(m.0, table[m.0.index()][m.1.index()])
    }).sum();

    println!("Part2: {}", result)
}

fn main() {
    part1();
    part2();
}


