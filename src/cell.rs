use instruction::{Instruction, Number};
use env::EventType;

pub type Block = Vec<Instruction>;
pub type Code = Vec<Block>;

pub type CellID = u64;

#[derive(Debug)]
pub struct Cell {
    pub id: CellID,
    pub step: u64,
    pub x: u64,
    pub y: u64,
    pub nrg: u64,
    code: Code,
    cb: usize,
    ip: usize,
    is_running: bool,
    call: Vec<(usize, usize)>,
    data: Vec<Number>,
}

impl Cell {
    pub fn new(id: CellID, x: u64, y: u64, step: u64, nrg: u64, code: Code) -> Cell {
        Cell {
            id: id,
            step: step,
            x: x,
            y: y,
            nrg: nrg,
            code: code,
            ip: 0,
            cb: 0,
            is_running: true,
            call: Vec::new(),
            data: Vec::new(),
        }
    }

    pub fn step(&mut self) -> Option<EventType> {
        let res;
        if self.is_running() {
            let instr = self.get_current_instr();
            res = instr.exec(self);
            if res.is_none() {
                self.step += 1;
            }
        } else {
            res = Some(EventType::Zombify);
        }
        // add code to check if this cell should die
        res
    }

    fn get_current_instr(&self) -> Instruction {
        if self.ip < self.code[self.cb].len() {
            self.code[self.cb][self.ip]
        } else {
            Instruction::Ret
        }
    }

    pub fn is_running(&self) -> bool {
        self.is_running
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
            self.is_running = false;
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
