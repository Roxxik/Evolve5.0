#![allow(dead_code)]
#![feature(vec_push_all)]


mod cell;
mod instruction;
mod env;
mod field;

use instruction::Instruction::*;
use cell::Cell;
use env::Env;

fn main() {
    let code = vec![
        vec![//main
            Num(1),
            Dup2,
            Mov,
            Pop,
            Loop,
        ],
    ];
    let mut cell = Cell::new(0,0,0,code);
    let mut env = Env::new(10,10,10);
    env.put(cell);
    //println!("{:?}", cell);
    for _ in 0..40 {
        //cell.step();
        println!("{:?}", env);
        env.step();
    }
}
