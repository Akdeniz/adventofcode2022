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

fn parse() -> HashSet<(i32,i32)> {
    let mut elves: HashSet<(i32,i32)> = HashSet::new();
    for (i, line) in read_lines().iter().enumerate() {
        for (j, _) in line.chars().enumerate().filter(|(_,c)| *c=='#') {
            elves.insert((i as i32, j as i32));
        }
    }
    elves
}

fn find_proposal(elves:&HashSet<(i32,i32)>, pos: &(i32, i32), proposal_order:&[char;4]) -> (i32, i32) {

    if [(-1,-1),(-1,0),(-1,1),(0,-1),(0,1),(1,-1),(1,0),(1,1)].iter().map(|(x,y)| (x+pos.0, y+pos.1)).filter(|x| elves.contains(x)).count() == 0 {
        return *pos;
    }

    for c in proposal_order {
        match c {
            'N' => {
                if [(-1, -1),(-1, 0),(-1, 1)].iter().map(|(x,y)| (x+pos.0, y+pos.1)).filter(|x| elves.contains(x)).count() == 0 {
                    return (pos.0 - 1, pos.1);
                }
            }
            'S' => {
                if [(1, -1),(1, 0),(1, 1)].iter().map(|(x,y)| (x+pos.0, y+pos.1)).filter(|x| elves.contains(x)).count() == 0 {
                    return (pos.0 + 1, pos.1);
                }
            }
            'W' => {
                if [(-1, -1),(0,  -1),(1,  -1)].iter().map(|(x,y)| (x+pos.0, y+pos.1)).filter(|x| elves.contains(x)).count() == 0 {
                    return (pos.0 , pos.1 - 1);
                }
            }
            'E' => {
                if [(-1, 1),(0,  1),(1,  1)].iter().map(|(x,y)| (x+pos.0, y+pos.1)).filter(|x| elves.contains(x)).count() == 0 {
                    return (pos.0, pos.1 + 1);
                }
            }
            _ => panic!("unknown char {}", c)
        }
    }
    *pos
}

fn execute(elves:&HashSet<(i32,i32)>, proposal_order:&[char;4]) -> (bool, HashSet<(i32, i32)>) {

    let mut proposals: HashMap<(i32,i32),(i32, i32)> = HashMap::new();
    let mut proposal_count: HashMap<(i32,i32),i32> = HashMap::new();
    for elf in elves {
        let proposal = find_proposal(elves, &elf, proposal_order);
        proposals.insert(*elf, proposal);
        match proposal_count.get(&proposal) {
            Some(count) => { proposal_count.insert(proposal, count + 1); }
            None => { proposal_count.insert(proposal, 1); }
        }
    }

    let mut new_positions: HashSet<(i32,i32)> = HashSet::new();
    let mut number_of_changes = 0;
    for elf in elves {
        let proposal = proposals.get(elf).unwrap();
        match proposal_count.get(&proposal) {
            Some(1) => {
                new_positions.insert(*proposal);
                if proposal!=elf {
                    number_of_changes+=1;
                }
            }
            _ => { new_positions.insert(*elf); }
        }
    }
    (number_of_changes>0, new_positions)
}

fn part1() {
    let mut elves = parse();
    let mut p = ['N','S','W','E'];
    for _ in 1..=10 {
        elves = execute(&elves,&p).1;
        p.rotate_left(1);
    }

    let r_min_max = elves.iter().map(|x| x.0).minmax().into_option().unwrap();
    let c_min_max = elves.iter().map(|x| x.1).minmax().into_option().unwrap();

    let result = (r_min_max.1-r_min_max.0+1)*(c_min_max.1-c_min_max.0+1)-elves.len() as i32;
    println!("Part1: {}", result);
}

fn part2() {
    let mut elves = parse();
    let mut p = ['N','S','W','E'];
    let mut round = 1;
    loop {
        let (changed, newpositions) = execute(&elves,&p);
        if !changed {
            break;
        }
        elves = newpositions;
        p.rotate_left(1);
        round+=1;
    }
    println!("Part2: {}", round);
}

fn main() {
    part1();
    part2();
}


