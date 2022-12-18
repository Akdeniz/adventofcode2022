#![allow(unused_imports)]
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

struct Direction {
    data: Vec<char>,
    idx: usize
}

impl Direction {
    fn new() -> Direction
    {
        Direction {
            data: read_lines().first().unwrap().chars().collect(),
            idx: 0,
        }
    }
    fn next(&mut self) -> char {
        let ch = self.data[self.idx];
        self.idx=(self.idx+1) % self.data.len();
        ch
    }
}

/*
####  .#.  ..#  #  ##
      ###  ..#  #  ##
      .#.  ###  #
                #
*/
struct Offset {
    data: Vec<Vec<(i64, i64)>>,
    idx: usize
}

impl Offset {
    fn new() -> Offset {
        Offset{
            data: vec![
                vec![(0, 0),(0, 1),(0, 2),(0, 3)],
                vec![(0, 1),(1, 0),(1, 1),(1, 2),(2, 1)],
                vec![(0, 0),(0, 1),(0, 2),(1, 2),(2, 2)],
                vec![(0, 0),(1, 0),(2, 0),(3, 0)],
                vec![(0, 0),(0, 1),(1, 0),(1, 1)],
            ],
            idx:0
        }
    }

    fn next(&mut self) -> &Vec<(i64, i64)> {
        let ch = &self.data[self.idx];
        self.idx=(self.idx+1) % self.data.len();
        ch
    }
}

struct Shape<'a> {
    offset: &'a Vec<(i64, i64)>,
    pos: (i64, i64) // left bottom
}

impl<'a> Shape<'a> {
    fn go(&mut self, points: &HashSet<(i64,i64)>, xdiff:i64, ydiff:i64) -> bool {

        for off in self.offset {
            let rpos = (off.0 + self.pos.0 + xdiff, off.1 + self.pos.1 + ydiff);
            if rpos.1 < 0 || rpos.1 >= 7 || rpos.0 < 0 || points.contains(&rpos) {
                return false;
            }
        }
        self.pos.0+=xdiff;
        self.pos.1+=ydiff;
        return true
    }

    fn positions(&self) -> Vec<(i64, i64)> {
        return self.offset.iter().map(|x| (x.0+self.pos.0, x.1+self.pos.1)).collect();
    }
}

fn simulate_drop(direction:&mut Direction, shape:&mut Shape, points: &HashSet<(i64,i64)>) {
    loop {
        match direction.next() {
            '>' => { shape.go(points, 0, 1); }
            '<' => { shape.go(points, 0, -1); }
            _ => panic!("unknown direction character")
        }
        if !shape.go(points, -1, 0) {
            break;
        }
    }
}

fn calculate_max_height(count: usize) -> i64 {
    let mut points : HashSet<(i64,i64)> = HashSet::new();
    let mut direction = Direction::new();
    let mut offset = Offset::new();
    let mut max_height = -1i64;

    for _ in 0..count {
        let mut shape = Shape{ offset: offset.next(), pos: (max_height + 4, 2) };
        simulate_drop(&mut direction, &mut shape, &points);

        for pos in shape.positions() {
            max_height = max_height.max(pos.0 as i64);
            points.insert(pos);
        }
    }
    max_height + 1
}

fn part1() {
    println!("Part1: {}", calculate_max_height(2022));
}

fn find_pattern() -> (i64, i64, i64) {
    let mut points : HashSet<(i64,i64)> = HashSet::new();
    let mut direction = Direction::new();
    let mut offset = Offset::new();
    let mut max_height = -1i64;
    let mut maxincols = [-1i64, -1, -1, -1, -1, -1, -1];
    let mut birdseye = [0i64, 0, 0, 0, 0, 0, 0];

    let mut cache : HashMap<([i64;7],usize,usize),(i64, i64)> = HashMap::new();
    cache.insert((birdseye, direction.idx, offset.idx), (-1, max_height));

    for i in 0.. {
        let mut shape = Shape{ offset: offset.next(), pos: (max_height + 4, 2) };
        simulate_drop(&mut direction, &mut shape, &points);

        for pos in shape.positions() {
            max_height = max_height.max(pos.0 as i64);
            maxincols[pos.1 as usize] = maxincols[pos.1 as usize].max(pos.0);
            points.insert(pos);
        }

        for k in 0..birdseye.len() {
            birdseye[k] = max_height - maxincols[k]
        }

        if let Some(prev) = cache.get(&(birdseye, direction.idx, offset.idx)) {
            return (prev.0, i - prev.0, max_height - prev.1);
        }
        cache.insert((birdseye, direction.idx, offset.idx), (i, max_height));
    }
    panic!("can not reach here");
}

fn part2() {
    let count = 1000000000000i64;
    let pattern = find_pattern();
    let iterate  = (pattern.0 + (count - pattern.0) % pattern.1) as usize;
    let pattern_count = (count-pattern.0) / pattern.1;
    let result = pattern.2 * pattern_count;

    println!("Part2: {}", result + calculate_max_height(iterate));
}


fn main() {
    part1();
    part2();
}


