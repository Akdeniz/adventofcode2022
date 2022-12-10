use std::cell::RefCell;
use std::fs::File;
use std::io::{self, BufRead};
use std::ops::Deref;
use std::path::Path;
use std::rc::Rc;
use scanf::sscanf;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>> where P: AsRef<Path> {
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn read() -> Vec<String> {
    let file_path = "/Users/ecem/CLionProjects/adventofcode2022/day07/day7.txt";
    let Ok(lines) = read_lines(file_path) else {
        return vec!();
    };

    return lines.into_iter().map(|x| x.unwrap()).collect();
}

fn parse(line: &str) -> (usize, usize, usize) {
    let mut count :usize = 0;
    let mut from :usize = 0;
    let mut to :usize = 0;

    // move 1 from 5 to 6
    let Ok(_) = sscanf!(&line, "move {} from {} to {}", count, from, to) else {
        panic!("Line does not match");
    };
    (count, from, to)
}

struct Dir<'a>
{
    name: &'a str,
    files: Vec<(String, i64)>,
    dirs: Vec<Rc<RefCell<Dir<'a>>>>
}


fn part1() {

    let mut stack = vec![Rc::new(RefCell::new(Dir{name:"/", files: vec![], dirs: vec![] }))];
    let lines = read();
    for line in lines {

        let tokens = line.split(' ').collect::<Vec<&str>>();

        match tokens[0] {
            "$" => {
                match tokens[1] {
                    "cd" => {
                        match tokens[2] {
                            "/" => { stack.drain(1..stack.len()); }
                            ".." => { stack.pop(); }
                            dirname => {
                                let last_ptr = stack.last().unwrap().clone();
                                let last = last_ptr.borrow();
                                let mut found = last.dirs.iter().find(|x| {
                                    x.borrow().name==dirname
                                }).unwrap();
                                stack.push(found.clone());
                            }
                        };
                    }
                    "ls" => {
                        let last_ptr = stack.last().unwrap().clone();
                        let mut last = last_ptr.borrow_mut();
                        last.files.clear();
                        last.dirs.clear();
                    }
                    c => panic!("unknown command {}", c)
                }
            }
            "dir" => {
                let last_ptr = stack.last().unwrap().clone();
                let mut last = last_ptr.borrow_mut();
                last.dirs.push(Rc::new(RefCell::new(Dir{name:tokens[1], files: vec![], dirs: vec![] })));
            }
            filename => {
                let last_ptr = stack.last().unwrap().clone();
                let mut last = last_ptr.borrow_mut();
                last.files.push((filename.parse().unwrap(), tokens[1].parse::<i64>().unwrap()));
            }
        }

        // for _ in 0..count {
        //     let c = stacks[from].pop().unwrap();
        //     stacks[to].push(c);
        // }
    }

    // let mut result = String::new();
    // for i in 1..stacks.len() {
    //     result.push(stacks[i].chars().last().unwrap());
    // }

    println!("Part1: {}", 1)
}

fn part2(input: &Vec<&str>) {

    let mut stacks: Vec<String> = input.iter().map(|&x| x.into()).collect();

    for x in read() {
        let (count,from,to) = parse(&*x);

        let len = stacks[from].len();
        let s = stacks[from].drain(len-count..len).collect::<String>();
        stacks[to].push_str(&*s);
    }

    let result = stacks[1..stacks.len()].iter().map(|x| x.chars().last().unwrap()).collect::<String>();
    println!("Part2: {}", result)
}

fn main() {

    part1();
    //part2(&stacks);
}


