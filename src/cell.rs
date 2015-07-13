use instruction::{Instruction, Number};
use env::EventType;

pub type Block = Vec<Instruction>;
pub type Code = Vec<Block>;

pub type CellID = u64;

#[derive(Debug)]
pub struct Cell {
    pub id: CellID,
    pub step: u64,
    x: u64,
    y: u64,
    code: Code,
    cb: usize,
    ip: usize,
    is_zombie: bool,
    call: Vec<(usize, usize)>,
    data: Vec<Number>,
}

impl Cell {
    pub fn new(x: u64, y: u64, step: u64, code: Code) -> Cell {
        Cell {
            id: 0,
            step: step,
            x: x,
            y: y,
            code: code,
            ip: 0,
            cb: 0,
            is_zombie: false,
            call: Vec::new(),
            data: Vec::new(),
        }
    }

    pub fn step(&mut self) -> Option<EventType> {
        let mut res = None;
        if self.is_running() {
            let instr = self.get_current_inst();
            res = instr.exec(self);
            if res.is_none() {
                self.step += 1;
            }
        }
        // add code to check if this cell should die
        res
    }

    fn get_current_inst(&self) -> Instruction {
        if self.ip < self.code[self.cb].len() {
            self.code[self.cb][self.ip]
        } else {
            Instruction::Ret
        }
    }

    pub fn is_running(&self) -> bool {
        !self.is_zombie
    }

    pub fn next_instr(&mut self) {
        self.ip += 1;
    }

    pub fn call(&mut self, cb: Number) {
        assert!(self.is_running());
        if cb >= 0 && cb < self.code.len() as i64 {
            let cb = cb as usize;
            self.call.push((self.cb,self.ip));
            self.ip = 0;
            self.cb = cb;
        }
    }
// loop is a keyword
    pub fn looop(&mut self) {
        assert!(self.is_running());
        self.ip = 0;
    }

    pub fn ret(&mut self) {
        assert!(self.is_running());
        if self.call.is_empty() {
            self.is_zombie = true;
        } else {
            let (cb, ip) = self.call.pop().unwrap();
            self.ip = ip;
            self.cb = cb;
        }
    }
    pub fn push(&mut self, n: Number) {
        self.data.push(n);
    }

    pub fn push_all(&mut self, ns: Vec<Number>) {
        self.data.push_all(&*ns);
    }

    pub fn pop(&mut self, n: Number) -> Option<Vec<Number>> {
        assert!(n >= 0);
        let n = n as usize;
        if self.data.len() >= n {
            let temp = self.data.clone();
            let (base, split) = temp.split_at(self.data.len() - n);
            self.data = base.to_vec();
            Some(split.to_vec())
        } else {
            None
        }
    }

    pub fn peek(&mut self, n: Number) -> Option<Vec<Number>> {
        assert!(n >= 0);
        let n = n as usize;
        if self.data.len() >= n {
            let temp = self.data.clone();
            let (_, split) = temp.split_at(self.data.len() - n);
            Some(split.to_vec())
        } else {
            None
        }
    }
}

impl Eq for Cell {}

impl PartialEq for Cell {
    fn eq(&self, other: &Cell) -> bool {
        self.id == other.id
    }
}

use std::cmp::Ordering;
// Cells have an ordering based on their step
impl Ord for Cell {
    fn cmp(&self, other: &Cell) -> Ordering {
        // Notice that the we flip the ordering here
        other.step.cmp(&self.step)
    }
}

// `PartialOrd` needs to be implemented as well.
impl PartialOrd for Cell {
    fn partial_cmp(&self, other: &Cell) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}
