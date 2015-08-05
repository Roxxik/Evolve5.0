use types::*;
use organism::Organism;
use event::EventType;

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
    //Eat,
    Spawn,
}

use self::Instruction::*;

impl Instruction {
    pub fn exec(self, org: &mut Organism) -> Option<EventType> {
        org.next_instr();
        match self {
            Num(n)  => instr_num(org, n),
            Call    => instr_call(org),
            If      => instr_if(org),
            Ifelse  => instr_ifelse(org),
            Loop    => instr_loop(org),
            Condret => instr_condret(org),
            Ret     => instr_ret(org),
            Pop     => instr_pop(org),
            Pop2    => instr_pop2(org),
            Dup     => instr_dup(org),
            Dup2    => instr_dup2(org),
            Conddup => instr_conddup(org),
            Swap    => instr_swap(org),
            Swap2   => instr_swap2(org),
            Over    => instr_over(org),
            Over2   => instr_over2(org),
            Rot     => instr_rot(org),
            Tor     => instr_tor(org),
            Nip     => instr_nip(org),
            Tuck    => instr_tuck(org),
            Inc     => instr_unary(org, |x| x + 1),
            Inc2    => instr_unary(org, |x| x + 2),
            Dec     => instr_unary(org, |x| x - 1),
            Dec2    => instr_unary(org, |x| x - 2),
            Half    => instr_unary(org, |x| x / 2),
            Double  => instr_unary(org, |x| x * 2),
            Abs     => instr_unary(org, |x| x.abs()),
            Square  => instr_unary(org, |x| x * x),
            Sqrt    => instr_unary_nonnegative(org, |x| (x as f64).sqrt() as Number),
            Signum  => instr_unary(org, |x| x.signum()),
            Negate  => instr_unary(org, |x| x.wrapping_neg()),
            Not     => instr_unary(org, |x| !x & 1),
            Invert  => instr_unary(org, |x| !x),
            Zero    => instr_unary(org, |x| bool_to_int(x == 0)),
            Add     => instr_binary(org, |x, y| x + y),
            Sub     => instr_binary(org, |x, y| x - y),
            Mul     => instr_binary(org, |x, y| x * y),
            Div     => instr_binary_notnull(org, |x, y| x / y),
            Mod     => instr_binary_notnull(org, |x, y| x % y),
            Eq      => instr_binary(org, |x, y| (x == y) as Number),
            Ne      => instr_binary(org, |x, y| (x != y) as Number),
            Lt      => instr_binary(org, |x, y| (x  < y) as Number),
            Gt      => instr_binary(org, |x, y| (x  > y) as Number),
            Le      => instr_binary(org, |x, y| (x <= y) as Number),
            Ge      => instr_binary(org, |x, y| (x >= y) as Number),
            Or      => instr_binary(org, |x, y| x | y),
            And     => instr_binary(org, |x, y| x & y),
            Xor     => instr_binary(org, |x, y| x ^ y),
            Min     => instr_binary(org, |x, y| if x > y { x } else { y }),
            Max     => instr_binary(org, |x, y| if x < y { x } else { y }),
            Mov     => instr_move(org),
            //Eat     => instr_eat(org),
            Spawn   => instr_spawn(org),
        }
    }
}

fn instr_num(org: &mut Organism, n: Number) -> Option<EventType> {
    org.push(n);
    None
}

fn instr_call(org: &mut Organism) -> Option<EventType> {
    org.pop(1).and_then(|v| Some(org.call(v[0])));
    None
}

fn instr_if(org: &mut Organism) -> Option<EventType> {
    org.pop(2).and_then(|v| {
        if v[0] != 0 {
            org.call(v[1]);
        }
        None
    })
}

fn instr_ifelse(org: &mut Organism) -> Option<EventType> {
    org.pop(3).and_then(|v| {
        if v[0] != 0 {
            org.call(v[1]);
        } else {
            org.call(v[2]);
        }
        None
    })
}

fn instr_loop(org: &mut Organism) -> Option<EventType> {
    org.pop(1).and_then(|v| {
        if v[0] != 0 {
            org.looop();
        }
        None
    })
}

fn instr_condret(org: &mut Organism) -> Option<EventType> {
    org.pop(1).and_then(|v| {
        if v[0] != 0 {
            org.ret();
        }
        None
    })
}
fn instr_ret(org: &mut Organism) -> Option<EventType> {
    org.ret();
    None
}

fn instr_pop(org: &mut Organism) -> Option<EventType> {
    org.pop(1);
    None
}

fn instr_pop2(org: &mut Organism) -> Option<EventType> {
    org.pop(2);
    None
}

fn instr_dup(org: &mut Organism) -> Option<EventType> {
    org.peek(1).and_then(|v| {
        org.push(v[0]);
        None
    })
}

fn instr_dup2(org: &mut Organism) -> Option<EventType> {
    org.peek(2).and_then(|v| {
        org.push_all(v);
        None
    })
}

fn instr_conddup(org: &mut Organism) -> Option<EventType> {
    org.peek(1).and_then(|v| {
        if v[0] != 0 {
            org.push(v[0]);
        }
        None
    })
}

fn instr_swap(org: &mut Organism) -> Option<EventType> {
    org.pop(2).and_then(|mut v| {
        v.reverse();
        org.push_all(v);
        None
    })
}

fn instr_swap2(org: &mut Organism) -> Option<EventType> {
    org.pop(4).and_then(|mut v| {
        v.swap(0,1);
        v.swap(2,3);
        org.push_all(v);
        None
    })
}

fn instr_over(org: &mut Organism) -> Option<EventType> {
    org.peek(2).and_then(|v| {
        org.push(v[0]);
        None
    })
}

fn instr_over2(org: &mut Organism) -> Option<EventType> {
    org.peek(4).and_then(|mut v| {
        v.truncate(2);
        org.push_all(v);
        None
    })
}

fn instr_rot(org: &mut Organism) -> Option<EventType> {
    org.pop(3).and_then(|mut v| {
        let tmp = v.remove(0);
        v.push(tmp);
        org.push_all(v);
        None
    })
}

fn instr_tor(org: &mut Organism) -> Option<EventType> {
    org.pop(3).and_then(|mut v| {
        let tmp = v.pop().unwrap();
        v.insert(0, tmp);
        org.push_all(v);
        None
    })
}

fn instr_nip(org: &mut Organism) -> Option<EventType> {
    org.pop(2).and_then(|v| {
        org.push(v[1]);
        None
    })
}

fn instr_tuck(org: &mut Organism) -> Option<EventType> {
    org.pop(2).and_then(|v| {
        org.push(v[1]);
        org.push_all(v);
        None
    })
}


//change this to a wrapping semantic or some random runtime failures will show
fn instr_unary<F>(org: &mut Organism, f: F) -> Option<EventType>
    where F: Fn(Number) -> Number {
    org.pop(1).and_then(|v| {
        org.push(f(v[0]));
        None
    })
}

fn instr_unary_nonnegative<F>(org: &mut Organism, f: F) -> Option<EventType>
    where F: Fn(Number) -> Number {
    org.pop(1).and_then(|v| {
        if v[0] >= 0 {
            org.push(f(v[0]));
        }
        None
    })
}

fn instr_binary<F>(org: &mut Organism, f: F) -> Option<EventType>
    where F: Fn(Number, Number) -> Number {
    org.pop(2).and_then(|v| {
        org.push(f(v[0],v[1]));
        None
    })
}

fn instr_binary_notnull<F>(org: &mut Organism, f: F) -> Option<EventType>
    where F: Fn(Number, Number) -> Number {
    org.pop(2).and_then(|v| {
        if v[1] != 0 {
            org.push(f(v[0],v[1]));
        }
        None
    })
}

fn instr_move(org: &mut Organism) -> Option<EventType> {
    org.pop(2).and_then(|v| {
        Some(EventType::Mov{ dir: Direction::from_coords(v[0], v[1]) })
    })
}

/*
fn instr_eat(org: &mut Organism) -> Option<EventType> {
    org.pop(2).and_then(|v| {
        Some(EventType::Eat{ orgID: org.id, dir: Direction::from_coords(v[0], v[1]) })
    })
}*/

fn instr_spawn(org: &mut Organism) -> Option<EventType> {
    org.pop(3).and_then(|v| {
        if v[2] >= 0 {
            Some(EventType::Spawn { dir: Direction::from_coords(v[0], v[1]), nrg: v[2] as u64 })
        } else {
            None
        }
    })
}
