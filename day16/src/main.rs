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
use regex::{Captures, Regex};
use scanf::sscanf;
use lazy_static::lazy_static;

fn read_all() -> String {
    let file_path = "input.txt";
    std::fs::read_to_string(file_path).unwrap()
}

fn read_lines() -> Vec<String> {
    read_all().split('\n').map(|x| x.parse().unwrap()).filter(|x: &String| !x.is_empty()).collect()
}

struct Node {
    rate: i64,
    from: String,
    tos: Vec<String>,
}

fn parse(input: &str) -> Node {
    lazy_static! {
        static ref BASE_RE: Regex = Regex::new(r#"Valve (?P<from>[A-Z]+) has flow rate=(?P<rate>[0-9]+); tunnels? leads? to valves? (?P<tos>[A-Z ,]+)"#).unwrap();
        static ref VALVE_RE: Regex = Regex::new(r#"(?P<to>[A-Z]+)"#).unwrap();
    }

    match BASE_RE.captures(input) {
        Some(cap) => {
            return Node {
                rate: cap.name("rate").unwrap().parse::<i64>().unwrap(),
                from: cap.name("from").unwrap().to_string(),
                tos: VALVE_RE.captures_iter(cap.name("tos").unwrap()).map(|x| x.name("to").unwrap().to_string()).collect(),
            };
        }
        _ => panic!("no regex match")
    }
}

struct State {
    node: i64,
    score: i64,
    opened: [bool; 64],
}

fn bfs(graph: &HashMap<i64, (i64, Vec<i64>)>, start: i64, minutes: i64, opened: [bool; 64]) -> i64 {
    let mut q: VecDeque<State> = VecDeque::new();
    q.push_back(State { node: start, score: 0, opened });

    let mut memo: HashMap<(i64, [bool; 64]), i64> = HashMap::new();

    let mut result = 0i64;
    for minute in (1..=minutes).rev() {
        let qsize = q.len();
        for _ in 0..qsize {
            let cur = q.pop_front().unwrap();

            if let Some(&cached) = memo.get(&(cur.node, cur.opened)) {
                if cached >= cur.score {
                    continue;
                }
            }
            memo.insert((cur.node, cur.opened), cur.score);

            result = result.max(cur.score);

            if !cur.opened[cur.node as usize] {
                let node = graph.get(&cur.node).unwrap();
                if node.0 > 0 {
                    let mut opened = cur.opened.clone();
                    opened[cur.node as usize] = true;
                    q.push_back(State { node: cur.node, score: &cur.score + (node.0 * (minute - 1)), opened });
                }
            }

            for next in &graph.get(&cur.node).unwrap().1 {
                q.push_back(State { node: *next, score: cur.score, opened: cur.opened });
            }
        }
    }
    result
}

struct Index {
    size: i64,
    map: HashMap<String, i64>,
}

impl Index {
    fn new() -> Index {
        Index {
            size: 0,
            map: HashMap::new(),
        }
    }

    fn get(&mut self, val: String) -> i64 {
        if let Some(x) = self.map.get(&val) {
            return *x;
        }
        let newidx = self.size;
        self.map.insert(val, newidx);
        self.size += 1;
        return newidx;
    }
}

fn part1() {
    let mut index = Index::new();
    let mut graph: HashMap<i64, (i64, Vec<i64>)> = HashMap::new();

    {
        let nodes: Vec<Node> = read_lines().iter().map(|x| parse(x)).collect();
        for node in nodes {
            graph.insert(index.get(node.from), (node.rate, node.tos.iter().map(|x| index.get(x.clone())).collect()));
        }
    }

    let result = bfs(&graph, index.get("AA".to_string()), 30, [false; 64]);
    println!("Part1: {}", result);
}

fn find_pairs(graph: &HashMap<i64, (i64, Vec<i64>)>, start: i64, minutes: i64) -> HashMap<[bool;64], i64> {
    let mut result = HashMap::new();

    let mut q: VecDeque<State> = VecDeque::new();
    q.push_back(State { node: start, score: 0, opened: [false;64] });

    let mut memo: HashMap<(i64, [bool; 64]), i64> = HashMap::new();

    for minute in (1..=minutes).rev() {
        let qsize = q.len();
        for _ in 0..qsize {
            let cur = q.pop_front().unwrap();

            if let Some(&cached) = memo.get(&(cur.node, cur.opened)) {
                if cached >= cur.score {
                    continue;
                }
            }
            memo.insert((cur.node, cur.opened), cur.score);

            match result.get(&cur.opened) {
                Some(x) => result.insert(cur.opened, cur.score.max(*x)),
                None => result.insert(cur.opened, cur.score)
            };

            if !cur.opened[cur.node as usize] {
                let node = graph.get(&cur.node).unwrap();
                if node.0 > 0 {
                    let mut opened = cur.opened.clone();
                    opened[cur.node as usize] = true;
                    q.push_back(State { node: cur.node, score: &cur.score + (node.0 * (minute - 1)), opened });
                }
            }

            for next in &graph.get(&cur.node).unwrap().1 {
                q.push_back(State { node: *next, score: cur.score, opened: cur.opened });
            }
        }
    }

    result
}

fn part2() {
    let mut index = Index::new();
    let mut graph: HashMap<i64, (i64, Vec<i64>)> = HashMap::new();

    {
        let nodes: Vec<Node> = read_lines().iter().map(|x| parse(x)).collect();
        for node in nodes {
            graph.insert(index.get(node.from), (node.rate, node.tos.iter().map(|x| index.get(x.clone())).collect()));
        }
    }

    let start_idx = index.get("AA".to_string());
    let combinations = find_pairs(&graph, start_idx, 26);

    let mut result = 0i64;
    for comb in combinations {
        let elephant = bfs(&graph, start_idx, 26, comb.0);
        result = result.max(comb.1+elephant);
    }

    println!("Part2: {}", result);
}

fn main() {
    part1();
    part2();
}


