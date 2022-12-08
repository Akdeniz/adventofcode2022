use std::cell::RefCell;
use std::cmp::min;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;
use std::rc::Rc;

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

struct Dir
{
    name: String,
    files: Vec<(String, i64)>,
    dirs: Vec<Rc<RefCell<Dir>>>,
}

impl Dir {
    fn new(name: &str) -> Rc<RefCell<Dir>> {
        Rc::new(RefCell::new(Dir { name: name.to_string(), files: vec![], dirs: vec![] }))
    }
}

fn parse() -> Rc<RefCell<Dir>> {
    let mut stack = vec![Dir::new("/")];

    for line in read() {
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
                                let found = last.dirs.iter().find(|x| {
                                    x.borrow().name == dirname
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
                last.dirs.push(Dir::new(tokens[1]));
            }
            filesize => {
                let last_ptr = stack.last().unwrap().clone();
                let mut last = last_ptr.borrow_mut();
                last.files.push((tokens[1].to_string(), filesize.parse::<i64>().unwrap()));
            }
        }
    }

    return stack.first().unwrap().clone();
}

fn traverse<F: FnMut(&Rc<RefCell<Dir>>)>(node: &Rc<RefCell<Dir>>, func: &mut F) {
    func(node);

    for dir in &node.borrow().dirs {
        traverse(dir, func);
    }
}

fn getsize(node: &Rc<RefCell<Dir>>) -> i64 {
    let mut result: i64 = 0;
    for (_, size) in &node.borrow().files {
        result += size;
    }
    for dir in &node.borrow().dirs {
        result += getsize(dir);
    }

    result
}

fn part1() {
    let root = parse();

    let mut total: i64 = 0;
    traverse(&root, &mut |dir| {
        let size = getsize(dir);
        if size <= 100000 {
            total += size;
        }
    });

    println!("Part1: {}", total);
}

fn part2() {
    let root = parse();

    let used_size = getsize(&root);
    let disk_space = 70_000_000;
    let update_size = 30_000_000;
    let space_needed = update_size - (disk_space - used_size);

    let mut result: i64 = disk_space;
    traverse(&root, &mut |dir| {
        let size = getsize(dir);
        if size >= space_needed {
            result = min(result, size);
        }
    });

    println!("Part2: {}", result);
}

fn main() {
    part1();
    part2();
}


