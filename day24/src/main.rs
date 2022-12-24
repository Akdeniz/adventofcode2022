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
use itertools::Itertools;
use multimap::MultiMap;

fn read_all() -> String {
    let file_path = "input.txt";
    std::fs::read_to_string(file_path).unwrap()
}

fn read_lines() -> Vec<String> {
    read_all().lines().map(|x| x.parse().unwrap()).collect_vec()
}

fn get_next_positions(positions: &MultiMap<(i32, i32), char>, br: (i32, i32)) -> MultiMap<(i32, i32), char> {
    let mut newpositions: MultiMap<(i32, i32), char> = MultiMap::new();
    for (&(x, y), cvec) in positions {
        for &c in cvec {
            let (newx, newy) = match c {
                '^' if x == 1 => (br.0 - 1, y),
                '^' => (x - 1, y),
                'v' if x + 1 == br.0 => (1, y),
                'v' => (x + 1, y),
                '>' if y + 1 == br.1 => (x, 1),
                '>' => (x, y + 1),
                '<' if y == 1 => (x, br.1 - 1),
                '<' => (x, y - 1),
                '#' => (x, y),
                _ => panic!("invalid char {}", c)
            };
            newpositions.insert((newx, newy), c);
        }
    }
    newpositions
}

/*
struct PositionIndex {
    int last_idx = 0;
    std::map<std::multimap<std::pair<int, int>, char>, int> cache;

    int operator()(const std::multimap<std::pair<int, int>, char> &m) {
        auto state = cache.emplace(m, last_idx);
        if(state.second)
            return last_idx++;
        return state.first->second;
    }
};
 */
struct PositionIndex {
    last_idx: i32,
    cache: HashMap<Vec<(i32,i32,char)>, i32>
}

impl PositionIndex {
    fn new() -> PositionIndex {
        PositionIndex{ last_idx: 0, cache: Default::default() }
    }

    fn get(&mut self, positions: &MultiMap<(i32, i32), char>) -> i32 {

        let mut key : Vec<(i32,i32,char)> = Vec::new();
        for (&(x, y), cvec) in positions {
            for &c in cvec {
                key.push((x, y, c));
            }
        }
        key.sort();

        if let Some(&x) = self.cache.get(&key) {
            return x;
        }
        let index = self.last_idx;
        self.last_idx+=1;
        self.cache.insert(key, index);
        return index;
    }
}

fn bfs(mut positions: MultiMap<(i32, i32), char>, start: (i32, i32), end: (i32, i32), bottom_right: (i32, i32)) -> (i32, MultiMap<(i32, i32), char>) {
    let mut cache: HashSet<(i32, (i32, i32))> = HashSet::new();
    let mut positions_cache = PositionIndex::new();

    let mut q: VecDeque<(i32, i32)> = VecDeque::new();
    q.push_back(start);

    let mut round = 0;
    loop {
        if q.is_empty() { break; }

        let position_id = positions_cache.get(&positions);
        let newpositions = get_next_positions(&positions, bottom_right);
        let qsize = q.len();
        for _ in 0..qsize {
            let cur = q.pop_front().unwrap();

            if cur == end {
                return (round, positions);
            }

            if cache.contains(&(position_id, cur)) {
                continue;
            }
            cache.insert((position_id, cur));

            for newpos in [(-1, 0), (1, 0), (0, 0), (0, -1), (0, 1)].iter()
                .map(|(x, y)| (x + cur.0, y + cur.1))
                .filter(|&(x, y)| !(x < 0 || x > bottom_right.0 || y < 0 || y > bottom_right.1 || newpositions.contains_key(&(x, y)))) {
                q.push_back(newpos);
            }
        }
        positions = newpositions;
        round += 1;
    }
    (round, positions)
}

fn get_positions(table: &Vec<String>) -> MultiMap<(i32, i32), char> {
    let mut positions: MultiMap<(i32, i32), char> = MultiMap::new();
    for (i, line) in table.iter().enumerate() {
        for (j, c) in line.chars().enumerate() {
            if c != '.' {
                positions.insert((i as i32, j as i32), c);
            }
        }
    }
    positions
}

fn part1() {
    let table = read_lines();
    let start = (0, 1);
    let end = ((table.len() - 1) as i32, (table[0].len() - 2) as i32);
    let bottom_right = ((table.len() - 1) as i32, (table[0].len() - 1) as i32);

    let (result, _) = bfs(get_positions(&table), start, end, bottom_right);
    println!("Part1: {}", result);
}

fn part2() {
    let table = read_lines();
    let start = (0, 1);
    let end = ((table.len() - 1) as i32, (table[0].len() - 2) as i32);
    let bottom_right = ((table.len() - 1) as i32, (table[0].len() - 1) as i32);

    let (round_a, a) = bfs(get_positions(&table), start, end, bottom_right);
    let (round_b, b) = bfs(a, end, start, bottom_right);
    let (round_c, _) = bfs(b, start, end, bottom_right);
    println!("Part2: {}", round_a+round_b+round_c);
}

fn main() {
    part1();
    part2();
}


