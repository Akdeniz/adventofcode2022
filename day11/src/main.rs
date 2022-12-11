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
            op: ("+".parse().unwrap(), Value::OldVal),
            test_div: 0,
            test_true: 0,
            test_false: 0,
            inspection_times: 0,
        }
    }
}

fn parse() -> Vec<Monkey> {
    let re = Regex::new(r"Monkey ([0-9]+):\n  Starting items: ([0-9, ]+)\n  Operation: new = old ([*+]) ([0-9]+|old)\n  Test: divisible by ([0-9]+)\n    If true: throw to monkey ([0-9]+)\n    If false: throw to monkey ([0-9]+)").unwrap();
    let text = read().join("\n");

    let mut monkeys: Vec<Monkey> = vec![];
    for cap in re.captures_iter(&*text) {
        let mut monkey = Monkey::new();
        monkey.items = cap[2].split(',').map(|x| x.trim().parse::<i64>().unwrap()).collect::<Vec<i64>>();
        match &cap[4] {
            "old" => {
                monkey.op = (cap[3].parse().unwrap(), OldVal);
            }
            intval => {
                monkey.op = (cap[3].parse().unwrap(), IntVal(intval.parse::<i64>().unwrap()));
            }
        }

        monkey.test_div = cap[5].parse::<i64>().unwrap();
        monkey.test_true = cap[6].parse::<i64>().unwrap();
        monkey.test_false = cap[7].parse::<i64>().unwrap();

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

    let result = times[0] * times[1];
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
                item = item % master_mod + master_mod;
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

    let result = times[0] * times[1];
    println!("Part2: {}", result)
}


fn main() {
    part1();
    part2();
}


