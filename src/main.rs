#![allow(dead_code)]
#![feature(vec_push_all)]


mod organism;
mod instruction;
mod env;
mod field;
mod types;
mod event;

use env::Env;

fn main() {
    let mut env = Env::new(10,10,10);
    //println!("{:?}", cell);
    for _ in 0..40 {
        //cell.step();
        println!("{:?}", env);
        env.step();
    }
}
