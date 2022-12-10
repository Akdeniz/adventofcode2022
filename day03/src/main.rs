use std::collections::HashSet;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;


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

fn score(c: char) -> i32 {
    if c >= 'a' && c <= 'z' {
        return (c as i32) - ('a' as i32) + 1;
    }
    assert!(c >= 'A' && c <= 'Z');
    return (c as i32) - ('A' as i32) + 27;
}

fn part1() {
    let result: i32 = read().into_iter().map(|s| {
        let mut chunks = s.as_bytes().chunks(s.len() / 2);
        let first = chunks.next().unwrap().iter().cloned().collect::<HashSet<u8>>();
        let second = chunks.next().unwrap().iter().cloned().collect::<HashSet<u8>>();
        let intersection = *first.intersection(&second).next().unwrap();
        return score(intersection as char);
    }).sum();

    println!("Part1: {}", result)
}

fn part2() {
    let result: i32 = read().chunks(3).into_iter().map(|s| {
        assert_eq!(s.len(), 3);
        let first = s[0].as_bytes().iter().cloned().collect::<HashSet<u8>>();
        let second = s[1].as_bytes().iter().cloned().collect::<HashSet<u8>>();
        let third = s[2].as_bytes().iter().cloned().collect::<HashSet<u8>>();

        let mut intersection = first.intersection(&second).cloned().collect::<HashSet<u8>>();
        intersection = third.intersection(&intersection).cloned().collect::<HashSet<u8>>();
        assert_eq!(intersection.len(), 1);
        return score(*intersection.iter().next().unwrap() as char);
    }).sum();

    println!("Part2: {}", result)
}

fn main() {
    part1();
    part2();
}


