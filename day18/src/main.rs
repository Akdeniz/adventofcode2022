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
    read_all().split('\n').map(|x| x.parse().unwrap()).collect::<Vec<String>>()
}

fn sides_covered(points: &HashSet<(i32, i32, i32)>, point: &(i32, i32, i32)) -> i32 {
    lazy_static! {
        static ref SIDES: [(i32,i32,i32);6] = [(1,  0,  0), (0,  1,  0), (0,  0,  1), (-1, 0,  0), (0,  -1, 0),(0,  0,  -1)];
    }
    SIDES.iter().map(|x| points.contains(&(point.0 + x.0, point.1 + x.1, point.2 + x.2)) as i32).sum()
}

fn part1() {
    let mut points: HashSet<(i32, i32, i32)> = HashSet::new();
    for line in read_lines().iter().filter(|x| !x.is_empty()) {
        let (x, y, z) = line.split(',').map(|x| x.parse::<i32>().unwrap()).next_tuple().unwrap();
        points.insert((x, y, z));
    }
    let result: i32 = points.iter().map(|point| 6 - sides_covered(&points, point)).sum();
    println!("Part1: {}", result);
}

fn part2() {
    let mut points: HashSet<(i32, i32, i32)> = HashSet::new();
    for line in read_lines().iter().filter(|x| !x.is_empty()) {
        let (x, y, z) = line.split(',').map(|x| x.parse::<i32>().unwrap()).next_tuple().unwrap();
        points.insert((x, y, z));
    }

    let mut x_min_max = points.iter().map(|x| (x.0, x.0)).reduce(|a, b| (a.0.min(b.0), a.1.max(b.1))).unwrap();
    let mut y_min_max = points.iter().map(|x| (x.1, x.1)).reduce(|a, b| (a.0.min(b.0), a.1.max(b.1))).unwrap();
    let mut z_min_max = points.iter().map(|x| (x.2, x.2)).reduce(|a, b| (a.0.min(b.0), a.1.max(b.1))).unwrap();

    // add margin
    x_min_max = (x_min_max.0 - 1, x_min_max.1 + 1);
    y_min_max = (y_min_max.0 - 1, y_min_max.1 + 1);
    z_min_max = (z_min_max.0 - 1, z_min_max.1 + 1);

    // println!("x: {:?}, y: {:?}, z: {:?}", x_min_max, y_min_max, z_min_max);

    let mut q: VecDeque<(i32, i32, i32)> = VecDeque::new();
    for y in y_min_max.0..=y_min_max.1 {
        for z in z_min_max.0..=z_min_max.1 {
            q.push_back((x_min_max.0, y, z));
            q.push_back((x_min_max.1, y, z));
        }
    }

    for x in x_min_max.0..=x_min_max.1 {
        for z in z_min_max.0..=z_min_max.1 {
            q.push_back((x, y_min_max.0, z));
            q.push_back((x, y_min_max.1, z));
        }
    }

    for x in x_min_max.0..=x_min_max.1 {
        for y in y_min_max.0..=y_min_max.1 {
            q.push_back((x, y, z_min_max.0));
            q.push_back((x, y, z_min_max.1));
        }
    }

    let mut result = 0;
    let mut visited: HashSet<(i32, i32, i32)> = HashSet::new();

    let sides = [(1, 0, 0), (0, 1, 0), (0, 0, 1), (-1, 0, 0), (0, -1, 0), (0, 0, -1)];
    loop {
        if q.is_empty() {
            break;
        }

        let cur = q.pop_front().unwrap();
        if visited.contains(&cur) {
            continue;
        }
        visited.insert(cur);

        result += sides_covered(&points, &cur);

        for side in sides {
            let next = (side.0 + cur.0, side.1 + cur.1, side.2 + cur.2);
            if points.contains(&next)
                || next.0 < x_min_max.0 || next.0 > x_min_max.1
                || next.1 < y_min_max.0 || next.1 > y_min_max.1
                || next.2 < z_min_max.0 || next.2 > z_min_max.1 {
                continue;
            }
            q.push_back(next);
        }
    }

    println!("Part2: {}", result);
}

fn main() {
    part1();
    part2();
}


