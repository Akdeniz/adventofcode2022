use std::cell::RefCell;
use std::cmp::Ordering;
use std::collections::{HashMap, VecDeque, HashSet};
use std::fs::File;
use std::io::{self, BufRead};
use std::ops::Deref;
use std::path::Path;
use std::rc::Rc;
use std::vec;
use regex::Regex;
use scanf::sscanf;
use lazy_static::lazy_static;

fn read_all() -> String {
    let file_path = "input.txt";
    std::fs::read_to_string(file_path).unwrap()
}

fn read_lines() -> Vec<String> {
    read_all().split('\n').map(|x| x.parse().unwrap()).collect::<Vec<String>>()
}

fn parse(input: &str) -> Vec<(i32,i32)> {
    lazy_static! {
        static ref RE: Regex = Regex::new(r#"(?P<y>[0-9]+),(?P<x>[0-9]+)"#).unwrap();
    }

    let mut result: Vec<(i32,i32)> = vec![];
    for cap in RE.captures_iter(input) {

        let x = cap.name("x").unwrap().parse::<i32>().unwrap();
        let y = cap.name("y").unwrap().parse::<i32>().unwrap();
        result.push((x,y));
    }
    result
}

fn drop_sand(walls: &mut HashSet<(i32, i32)>, lowest: i32) -> bool {
    let mut sand = (0,500);

    while sand.0 <= lowest {
        if !walls.contains(&(sand.0 + 1, sand.1)) {
            sand = (sand.0 + 1, sand.1);
        } else if !walls.contains(&(sand.0 + 1, sand.1 - 1)) {
            sand = (sand.0 + 1, sand.1 - 1);
        } else if !walls.contains(&(sand.0 + 1, sand.1 + 1)) {
            sand = (sand.0 + 1, sand.1 + 1);
        } else {
            walls.insert(sand);
            return true;
        }
    }
    false
}

fn part1() {
    let lines = read_lines().into_iter().filter(|x| !x.is_empty()).collect::<Vec<String>>();

    let mut walls :HashSet<(i32, i32)> = HashSet::new();
    for line in lines {
        let positions = parse(line.as_str());
        for i in 1..positions.len() {
            let deltax = match positions[i-1].0.cmp(&positions[i].0) {
                Ordering::Less => 1,
                Ordering::Equal => 0,
                Ordering::Greater => -1
            };
            let deltay = match positions[i-1].1.cmp(&positions[i].1) {
                Ordering::Less => 1,
                Ordering::Equal => 0,
                Ordering::Greater => -1
            };

            let mut from = positions[i-1];
            while from!=positions[i] {
                walls.insert(from);
                from = (from.0+deltax, from.1+deltay);
            }
            walls.insert(positions[i]);
        }
    }

    let lowest = walls.iter().map(|x| x.0).reduce(|x,y| x.max(y)).unwrap();

    let mut result = 0;
    while drop_sand(&mut walls, lowest) {
        result+=1;
    }

    println!("Part1: {}", result);
}

fn drop_sand_2(walls: &mut HashSet<(i32, i32)>, base: i32) -> bool {
    let mut sand = (0,500);

    if walls.contains(&sand) {
        return false;
    }

    while true {

        if base == sand.0 + 1 {
            walls.insert(sand);
            return true;
        }

        if !walls.contains(&(sand.0 + 1, sand.1)) {
            sand = (sand.0 + 1, sand.1);
        } else if !walls.contains(&(sand.0 + 1, sand.1 - 1)) {
            sand = (sand.0 + 1, sand.1 - 1);
        } else if !walls.contains(&(sand.0 + 1, sand.1 + 1)) {
            sand = (sand.0 + 1, sand.1 + 1);
        } else {
            walls.insert(sand);
            return true;
        }
    }
    false
}

fn part2() {
    let lines = read_lines().into_iter().filter(|x| !x.is_empty()).collect::<Vec<String>>();

    let mut walls :HashSet<(i32, i32)> = HashSet::new();
    for line in lines {
        let positions = parse(line.as_str());
        for i in 1..positions.len() {
            let deltax = match positions[i-1].0.cmp(&positions[i].0) {
                Ordering::Less => 1,
                Ordering::Equal => 0,
                Ordering::Greater => -1
            };
            let deltay = match positions[i-1].1.cmp(&positions[i].1) {
                Ordering::Less => 1,
                Ordering::Equal => 0,
                Ordering::Greater => -1
            };

            let mut from = positions[i-1];
            while from!=positions[i] {
                walls.insert(from);
                from = (from.0+deltax, from.1+deltay);
            }
            walls.insert(positions[i]);
        }
    }

    let lowest = walls.iter().map(|x| x.0).reduce(|x,y| x.max(y)).unwrap();

    let mut result = 0;
    while drop_sand_2(&mut walls, lowest+2) {
        result+=1;
    }

    println!("Part2: {}", result);
}

fn main() {
    part1();
    part2();
}


