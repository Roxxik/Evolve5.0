use cell::Cell;
use env::{EventType, Direction};

pub type Number = i64;

#[derive(Debug,Clone,Copy)]
pub enum Instruction {
    //pushing to data stack
    Num(Number),
    //controlflow
    Call,
    If,
    Ifelse,
    Loop,
    Condret,
    Ret,
    //stack operations
    Pop,
    Pop2,
    Dup,
    Dup2,
    Conddup,
    Swap,
    Swap2,
    Over,
    Over2,
    Rot,
    Tor,
    Nip,
    Tuck,
    //arithmetics
    //Unary
    Inc,
    Inc2,
    Dec,
    Dec2,
    Half,
    Double,
    Abs,
    Square,
    Sqrt,
    Signum,
    Negate,
    Not,
    Invert,
    Zero,
    //Binary
    Add,
    Sub,
    Mul,
    Div,
    Mod,

    Eq,
    Ne,
    Lt,
    Gt,
    Le,
    Ge,
    Or,
    And,
    Xor,
    Min,
    Max,
    Mov,
    Eat,
    Spawn,
}

use self::Instruction::*;

impl Instruction {
    pub fn exec(self, cell: &mut Cell) -> Option<EventType> {
        cell.next_instr();
        match self {
            Num(n)  => instr_num(cell, n),
            Call    => instr_call(cell),
            If      => instr_if(cell),
            Ifelse  => instr_ifelse(cell),
            Loop    => instr_loop(cell),
            Condret => instr_condret(cell),
            Ret     => instr_ret(cell),
            Pop     => instr_pop(cell),
            Pop2    => instr_pop2(cell),
            Dup     => instr_dup(cell),
            Dup2    => instr_dup2(cell),
            Conddup => instr_conddup(cell),
            Swap    => instr_swap(cell),
            Swap2   => instr_swap2(cell),
            Over    => instr_over(cell),
            Over2   => instr_over2(cell),
            Rot     => instr_rot(cell),
            Tor     => instr_tor(cell),
            Nip     => instr_nip(cell),
            Tuck    => instr_tuck(cell),
            Inc     => instr_unary(cell, |x| x + 1),
            Inc2    => instr_unary(cell, |x| x + 2),
            Dec     => instr_unary(cell, |x| x - 1),
            Dec2    => instr_unary(cell, |x| x - 2),
            Half    => instr_unary(cell, |x| x / 2),
            Double  => instr_unary(cell, |x| x * 2),
            Abs     => instr_unary(cell, |x| x.abs()),
            Square  => instr_unary(cell, |x| x * x),
            Sqrt    => instr_unary_nonnegative(cell, |x| (x as f64).sqrt() as Number),
            Signum  => instr_unary(cell, |x| x.signum()),
            Negate  => instr_unary(cell, |x| x.wrapping_neg()),
            Not     => instr_unary(cell, |x| !x & 1),
            Invert  => instr_unary(cell, |x| !x),
            Zero    => instr_unary(cell, |x| boolToInt(x == 0)),
            Add     => instr_binary(cell, |x, y| x + y),
            Sub     => instr_binary(cell, |x, y| x - y),
            Mul     => instr_binary(cell, |x, y| x * y),
            Div     => instr_binary_notnull(cell, |x, y| x / y),
            Mod     => instr_binary_notnull(cell, |x, y| x % y),
            Eq      => instr_binary(cell, |x, y| boolToInt(x == y)),
            Ne      => instr_binary(cell, |x, y| boolToInt(x != y)),
            Lt      => instr_binary(cell, |x, y| boolToInt(x  < y)),
            Gt      => instr_binary(cell, |x, y| boolToInt(x  > y)),
            Le      => instr_binary(cell, |x, y| boolToInt(x <= y)),
            Ge      => instr_binary(cell, |x, y| boolToInt(x >= y)),
            Or      => instr_binary(cell, |x, y| x | y),
            And     => instr_binary(cell, |x, y| x & y),
            Xor     => instr_binary(cell, |x, y| x ^ y),
            Min     => instr_binary(cell, |x, y| if x > y {x} else {y}),
            Max     => instr_binary(cell, |x, y| if x < y {x} else {y}),
            Mov     => instr_move(cell),
            Eat     => instr_eat(cell),
            Spawn   => instr_spawn(cell),
        }
    }
}

fn boolToInt(b: bool) -> Number {
    if b {
        1
    } else {
        0
    }
}

fn instr_num(cell: &mut Cell, n: Number) -> Option<EventType> {
    cell.push(n);
    None
}

fn instr_call(cell: &mut Cell) -> Option<EventType> {
    cell.pop(1).and_then(|v| Some(cell.call(v[0])));
    None
}

fn instr_if(cell: &mut Cell) -> Option<EventType> {
    cell.pop(2).and_then(|v| {
        if v[0] != 0 {
            cell.call(v[1]);
        }
        None
    })
}

fn instr_ifelse(cell: &mut Cell) -> Option<EventType> {
    cell.pop(3).and_then(|v| {
        if v[0] != 0 {
            cell.call(v[1]);
        } else {
            cell.call(v[2]);
        }
        None
    })
}

fn instr_loop(cell: &mut Cell) -> Option<EventType> {
    cell.pop(1).and_then(|v| {
        if v[0] != 0 {
            cell.looop();
        }
        None
    })
}

fn instr_condret(cell: &mut Cell) -> Option<EventType> {
    cell.pop(1).and_then(|v| {
        if v[0] != 0 {
            cell.ret();
        }
        None
    })
}
fn instr_ret(cell: &mut Cell) -> Option<EventType> {
    cell.ret();
    None
}

fn instr_pop(cell: &mut Cell) -> Option<EventType> {
    cell.pop(1);
    None
}

fn instr_pop2(cell: &mut Cell) -> Option<EventType> {
    cell.pop(2);
    None
}

fn instr_dup(cell: &mut Cell) -> Option<EventType> {
    cell.peek(1).and_then(|v| {
        cell.push(v[0]);
        None
    })
}

fn instr_dup2(cell: &mut Cell) -> Option<EventType> {
    cell.peek(2).and_then(|v| {
        cell.push_all(v);
        None
    })
}

fn instr_conddup(cell: &mut Cell) -> Option<EventType> {
    cell.peek(1).and_then(|v| {
        if v[0] != 0 {
            cell.push(v[0]);
        }
        None
    })
}

fn instr_swap(cell: &mut Cell) -> Option<EventType> {
    cell.pop(2).and_then(|mut v| {
        v.reverse();
        cell.push_all(v);
        None
    })
}

fn instr_swap2(cell: &mut Cell) -> Option<EventType> {
    cell.pop(4).and_then(|mut v| {
        v.swap(0,1);
        v.swap(2,3);
        cell.push_all(v);
        None
    })
}

fn instr_over(cell: &mut Cell) -> Option<EventType> {
    cell.peek(2).and_then(|v| {
        cell.push(v[0]);
        None
    })
}

fn instr_over2(cell: &mut Cell) -> Option<EventType> {
    cell.peek(4).and_then(|mut v| {
        v.truncate(2);
        cell.push_all(v);
        None
    })
}

fn instr_rot(cell: &mut Cell) -> Option<EventType> {
    cell.pop(3).and_then(|mut v| {
        let tmp = v.remove(0);
        v.push(tmp);
        cell.push_all(v);
        None
    })
}

fn instr_tor(cell: &mut Cell) -> Option<EventType> {
    cell.pop(3).and_then(|mut v| {
        let tmp = v.pop().unwrap();
        v.insert(0, tmp);
        cell.push_all(v);
        None
    })
}

fn instr_nip(cell: &mut Cell) -> Option<EventType> {
    cell.pop(2).and_then(|v| {
        cell.push(v[1]);
        None
    })
}

fn instr_tuck(cell: &mut Cell) -> Option<EventType> {
    cell.pop(2).and_then(|v| {
        cell.push(v[1]);
        cell.push_all(v);
        None
    })
}


//change this to a wrapping semantic or some random runtime failures will show
fn instr_unary<F>(cell: &mut Cell, f: F) -> Option<EventType>
    where F: Fn(Number) -> Number {
    cell.pop(1).and_then(|v| {
        cell.push(f(v[0]));
        None
    })
}

fn instr_unary_nonnegative<F>(cell: &mut Cell, f: F) -> Option<EventType>
    where F: Fn(Number) -> Number {
    cell.pop(1).and_then(|v| {
        if v[0] >= 0 {
            cell.push(f(v[0]));
        }
        None
    })
}

fn instr_binary<F>(cell: &mut Cell, f: F) -> Option<EventType>
    where F: Fn(Number, Number) -> Number {
    cell.pop(2).and_then(|v| {
        cell.push(f(v[0],v[1]));
        None
    })
}

fn instr_binary_notnull<F>(cell: &mut Cell, f: F) -> Option<EventType>
    where F: Fn(Number, Number) -> Number {
    cell.pop(2).and_then(|v| {
        if v[1] != 0 {
            cell.push(f(v[0],v[1]));
        }
        None
    })
}

fn instr_move(cell: &mut Cell) -> Option<EventType> {
    cell.pop(2).and_then(|v| {
        Some(EventType::Mov{ cellID: cell.id, dir: Direction::from_coords(v[0], v[1]) })
    })
}

fn instr_eat(cell: &mut Cell) -> Option<EventType> {
    cell.pop(2).and_then(|v| {
        Some(EventType::Eat{ cellID: cell.id, dir: Direction::from_coords(v[0], v[1]) })
    })
}

fn instr_spawn(cell: &mut Cell) -> Option<EventType> {
    cell.pop(3).and_then(|v| {
        if v[2] >= 0 {
            Some(EventType::Spawn { cellID: cell.id, dir: Direction::from_coords(v[0], v[1]), nrg: v[2] as u64 })
        } else {
            None
        }
    })
}
