use std::cell::RefCell;
use std::cmp::Ordering;
use std::collections::{HashMap, VecDeque};
use std::fs::File;
use std::io::{self, BufRead};
use std::ops::Deref;
use std::path::Path;
use std::rc::Rc;
use std::vec;
use regex::Regex;
use scanf::sscanf;

fn read_all() -> String {
    let file_path = "input.txt";
    std::fs::read_to_string(file_path).unwrap()
}

fn read_lines() -> Vec<String> {
    read_all().split('\n').map(|x| x.parse().unwrap()).collect::<Vec<String>>()
}

struct Node {
    pos: (usize, usize),
    distance: usize,
}

fn distance(table: &Vec<&[u8]>, mut q: Vec<Node>, dest: (usize, usize)) -> Option<usize> {
    let nexts: Vec<(i32, i32)> = vec![(-1, 0), (0, 1), (1, 0), (0, -1)];

    let mut visited :HashMap<(usize, usize), usize> = HashMap::new();
    loop {
        if q.is_empty() {
            break;
        }

        let mut newq: Vec<Node> = vec![];
        for i in 0..q.len() {
            let node = &q[i];

            if let Some(value) = visited.get(&node.pos) {
                if value <= &node.distance {
                    continue;
                }
            }

            visited.insert(node.pos, node.distance);

            if node.pos == dest {
                return Some(node.distance);
            }

            for next in &nexts {
                let newx = next.0 + (q[i].pos.0 as i32);
                let newy = next.1 + (q[i].pos.1 as i32);
                if newx < 0 || newx >= table.len() as i32 || newy < 0 || newy >= table[0].len() as i32 {
                    continue;
                }

                if table[newx as usize][newy as usize] > table[node.pos.0][node.pos.1] + 1 {
                    continue;
                }
                newq.push(Node { pos: (newx as usize, newy as usize), distance: node.distance + 1 });
            }
        }
        q = newq;
    }
    None
}

fn part1() {
    let mut lines = read_lines().into_iter().filter(|x| !x.is_empty()).collect::<Vec<String>>();

    let mut start = (0, 0);
    let mut end = (0, 0);
    for i in 0..lines.len() {
        let bytes = lines[i].as_bytes();
        for j in 0..bytes.len() {
            if bytes[j] as char == 'S' {
                start = (i, j);
            }
            if bytes[j] as char == 'E' {
                end = (i, j);
            }
        }
        lines[i] = lines[i].replace("S", "a").replace("E", "z");
    }

    let table = &lines.iter().map(|x| x.as_bytes()).collect::<Vec<&[u8]>>();

    let result = distance(table, vec![Node { pos: start, distance: 0 }], end);
    println!("Part1: {}", result.unwrap())
}

fn part2() {
    let mut lines = read_lines().into_iter().filter(|x| !x.is_empty()).collect::<Vec<String>>();

    let mut starting_points: Vec<Node> = vec![];
    let mut end = (0, 0);
    for i in 0..lines.len() {
        let bytes = lines[i].as_bytes();
        for j in 0..bytes.len() {
            if bytes[j] as char == 'S' || bytes[j] as char == 'a' {
                starting_points.push(Node{pos: (i, j), distance:0});
            }
            if bytes[j] as char == 'E' {
                end = (i, j);
            }
        }
        lines[i] = lines[i].replace("S", "a").replace("E", "z");
    }

    let table = &lines.iter().map(|x| x.as_bytes()).collect::<Vec<&[u8]>>();

    let result = distance(table, starting_points, end);
    println!("Part2: {}", result.unwrap())
}


fn main() {
    part1();
    part2();
}


