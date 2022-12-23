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

fn read_all() -> String {
    let file_path = "input.txt";
    std::fs::read_to_string(file_path).unwrap()
}

fn read_lines() -> Vec<String> {
    read_all().lines().map(|x| x.parse().unwrap()).collect_vec()
}

enum Node {
    Value(i64),
    Node(String, String, String),
}

fn parse() -> HashMap<String, Node> {
    lazy_static! {
        static ref A_RGX: Regex = Regex::new(r#"(?P<key>[a-z]{4}): (?P<left>[a-z]{4}) (?P<op>[+-/*]) (?P<right>[a-z]{4})"#).unwrap();
        static ref B_RGX: Regex = Regex::new(r#"(?P<key>[a-z]{4}): (?P<value>-?[0-9]+)"#).unwrap();
    }

    let mut tree: HashMap<String, Node> = HashMap::new();

    for line in read_lines() {
        if let Some(cap) = A_RGX.captures(line.as_str()) {
            tree.insert(cap.name("key").unwrap().to_string(), Node::Node(cap.name("op").unwrap().to_string(), cap.name("left").unwrap().to_string(), cap.name("right").unwrap().to_string()));
        }
        else if let Some(cap) = B_RGX.captures(line.as_str()) {
            tree.insert(cap.name("key").unwrap().to_string(), Node::Value(cap.name("value").unwrap().parse().unwrap()));
        } else {
            panic!("no regex match: {}", line);
        }
    }
    tree
}

fn dfs(tree: &HashMap<String, Node>, key: &str) -> i64
{
    let node = tree.get(key).unwrap();

    match node {
        Node::Value(x) => *x,
        Node::Node(op, left, right) => {
            match op.as_str() {
                "+" => dfs(tree, left) + dfs(tree, right),
                "-" => dfs(tree, left) - dfs(tree, right),
                "*" => dfs(tree, left) * dfs(tree, right),
                "/" => dfs(tree, left) / dfs(tree, right),
                _ => panic!("unknown op {}", op)
            }
        }
    }
}

fn part1() {
    let tree = parse();
    println!("Part1: {}", dfs(&tree, "root"));
}

fn part2() {
    let tree = parse();
    println!("Part2: {}", dfs(&tree, "root"));
}

fn main() {
    part1();
    part2();
}


