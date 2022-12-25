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

fn to_decimal(input: &str) -> i64 {
    input.chars().rev().enumerate().map(|(i, c)| match c {
        '=' => -(5i64.pow(i as u32)) * 2,
        '-' => -(5i64.pow(i as u32)),
        '0' => 0,
        '1' => 5i64.pow(i as u32),
        '2' => 5i64.pow(i as u32) * 2,
        c => panic!("unknown digit {}", c)
    }).sum()
}
/*

bool dfs(int64_t idx, std::string& result, int64_t diff)
{
    if(diff==0)
        return true;

    if(idx<0 || diff<0)
        return false;

    const int64_t pow = std::pow(5, idx);

    if(diff>=pow*5)
        return false;

    // =, -, 0, 1, and 2
    result[idx]='1';
    if(dfs(idx-1, result, diff-pow))
        return true;

    result[idx]='0';
    if(dfs(idx-1, result, diff-pow*2))
        return true;

    result[idx]='-';
    if(dfs(idx-1, result, diff-pow*3))
        return true;

    result[idx]='=';
    if(dfs(idx-1, result, diff-pow*4))
        return true;

    result[idx]='2';
    if(dfs(idx-1, result, diff))
        return true;

    return false;
}

std::string to_snafu(const int64_t decimal)
{

    auto success = dfs(result.size()-1,result,value-decimal);
    std::reverse(result.begin(), result.end());
    assert(success && decimal==to_decimal(result));
    return result;
}
 */

fn dfs(i: usize, result: &mut Vec<char>, diff: i64) -> bool {

    if diff==0 {
        return true;
    }

    if i >= result.len() || diff < 0 {
        return false;
    }

    let pow = 5i64.pow((result.len()-i-1) as u32);
    if diff >= pow*5 {
        return false;
    }

    if dfs(i+1, result, diff) {
        return true;
    }
    if dfs(i+1, result, diff-pow) {
        result[i] = '1';
        return true;
    }
    if dfs(i+1, result, diff-pow*2) {
        result[i] = '0';
        return true;
    }
    if dfs(i+1, result, diff-pow*3) {
        result[i] = '-';
        return true;
    }
    if dfs(i+1, result, diff-pow*4) {
        result[i] = '=';
        return true;
    }

    false
}

fn to_snafu(input: i64) -> String {
    let mut result: Vec<char> = vec![];

    let mut total = 0;
    {
        let mut pow = 1;
        while total < input {
            total += pow * 2;
            result.push('2');
            pow *= 5;
        }
    }

    if !dfs(0, &mut result, total-input) {
        panic!("could not find result");
    }

    result.iter().collect()
}

fn part1() {
    let result = to_snafu(read_lines().iter().map(|x| to_decimal(x.as_str())).sum());
    println!("Part1: {}", result);
}

fn main() {
    part1();
}


