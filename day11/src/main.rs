use std::cell::RefCell;
use std::cmp::Ordering;
use std::collections::VecDeque;
use std::fs::File;
use std::io::{self, BufRead};
use std::ops::Deref;
use std::path::Path;
use std::rc::Rc;
use std::vec;
use regex::Regex;
use scanf::sscanf;
use crate::Value::{IntVal, OldVal};
use lazy_static::lazy_static;

fn read_all() -> String {
    let file_path = "input.txt";
    std::fs::read_to_string(file_path).unwrap()
}

fn read_lines() -> Vec<String> {
    read_all().split('\n').map(|x| x.parse().unwrap()).collect::<Vec<String>>()
}

enum Value {
    IntVal(i64),
    OldVal,
}

struct Monkey {
    items: Vec<i64>,
    op: (String, Value),
    test_div: i64,
    test_true: i64,
    test_false: i64,
    inspection_times: usize,
}

impl Monkey {
    fn new() -> Monkey {
        Monkey {
            items: Default::default(),
            op: ("+".parse().unwrap(), OldVal),
            test_div: 0,
            test_true: 0,
            test_false: 0,
            inspection_times: 0,
        }
    }
}

fn parse() -> Vec<Monkey> {
     lazy_static! {
        static ref RE: Regex = Regex::new(
r#"Monkey ([0-9]+):
\s*Starting items: (?P<items>[0-9, ]+)
\s*Operation: new = old (?P<op>[*+]) (?P<value>[0-9]+|old)
\s*Test: divisible by (?P<div>[0-9]+)
\s*If true: throw to monkey (?P<true>[0-9]+)
\s*If false: throw to monkey (?P<false>[0-9]+)"#).unwrap();
    }

    let text = read_all();

    let mut monkeys: Vec<Monkey> = vec![];
    for cap in RE.captures_iter(&*text) {
        let mut monkey = Monkey::new();
        monkey.items = cap.name("items").unwrap().split(',').map(|x| x.trim().parse::<i64>().unwrap()).collect::<Vec<i64>>();
        match cap.name("value") {
            Some("old") => {
                monkey.op = (cap[3].parse().unwrap(), OldVal);
            }
            Some(intval) => {
                monkey.op = (cap[3].parse().unwrap(), IntVal(intval.parse::<i64>().unwrap()));
            }
            _ => panic!("Unknown operation")
        }

        monkey.test_div = cap.name("div").unwrap().parse::<i64>().unwrap();
        monkey.test_true = cap.name("true").unwrap().parse::<i64>().unwrap();
        monkey.test_false = cap.name("false").unwrap().parse::<i64>().unwrap();

        monkeys.push(monkey);
    }
    monkeys
}

fn part1() {
    let mut monkeys = parse();
    for _ in 0..20 {
        for idx in 0..monkeys.len() {
            monkeys[idx].inspection_times += monkeys[idx].items.len();
            let items = monkeys[idx].items.drain(..).collect::<Vec<i64>>();
            for mut item in items {
                match (monkeys[idx].op.0.as_str(), &monkeys[idx].op.1) {
                    ("*", OldVal) => {
                        item *= item;
                    }
                    ("+", OldVal) => {
                        item += item;
                    }
                    ("*", IntVal(x)) => {
                        item *= x;
                    }
                    ("+", IntVal(x)) => {
                        item += x;
                    }
                    (_, _) => panic!("Invalid operation")
                }

                item /= 3;

                let dest_idx;
                if item % monkeys[idx].test_div == 0 {
                    dest_idx = monkeys[idx].test_true;
                } else {
                    dest_idx = monkeys[idx].test_false;
                }
                monkeys[dest_idx as usize].items.push(item);
            }
        }
    }

    let mut times = monkeys.iter().map(|x| x.inspection_times).collect::<Vec<usize>>();
    times.sort_by(|a, b| b.cmp(a));

    let result = times[0..2].iter().cloned().reduce(|accum, x| accum * x).unwrap();
    println!("Part1: {}", result)
}

fn part2() {
    let mut monkeys = parse();

    let master_mod = monkeys.iter().map(|x| x.test_div).reduce(|accum, x| accum * x).unwrap();

    for _ in 0..10000 {
        for idx in 0..monkeys.len() {
            monkeys[idx].inspection_times += monkeys[idx].items.len();
            let items = monkeys[idx].items.drain(..).collect::<Vec<i64>>();
            for mut item in items {
                item = item % master_mod;
                match (monkeys[idx].op.0.as_str(), &monkeys[idx].op.1) {
                    ("*", OldVal) => {
                        item *= item;
                    }
                    ("+", OldVal) => {
                        item += item;
                    }
                    ("*", IntVal(x)) => {
                        item *= x;
                    }
                    ("+", IntVal(x)) => {
                        item += x;
                    }
                    (_, _) => panic!("Invalid operation")
                }

                let dest_idx;
                if item % monkeys[idx].test_div == 0 {
                    dest_idx = monkeys[idx].test_true;
                } else {
                    dest_idx = monkeys[idx].test_false;
                }
                monkeys[dest_idx as usize].items.push(item);
            }
        }
    }

    let mut times = monkeys.iter().map(|x| x.inspection_times).collect::<Vec<usize>>();
    times.sort_by(|a, b| b.cmp(a));

    let result = times[0..2].iter().cloned().reduce(|accum, x| accum * x).unwrap();
    println!("Part2: {}", result);
}


fn main() {
    part1();
    part2();
}


