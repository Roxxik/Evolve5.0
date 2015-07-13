use std::collections::BinaryHeap;
use std::collections::HashMap;
use std::cmp::Ordering;


use cell::{ Cell, CellID };
use field::Field;

//env makes makes the access to field so that the field always only displays the state at on step
pub struct Env {
    step: u64,
    nrg: u64,
    field: Field,//sparse vector of all possible positions
    cells: HashMap<CellID,Cell>,
    blocking: BinaryHeap<Event>,
    runnable: BinaryHeap<(CellID, u64)>,//heap of events(action+cell) sorted by step
}

impl Env {
    pub fn new(x: u64, y: u64, nrg: u64) -> Env {
        Env {
            step: 0,
            nrg: nrg,
            field: Field::new(x,y),
            cells: HashMap::new(),
            blocking: BinaryHeap::new(),
            runnable: BinaryHeap::new(),
        }
    }

    pub fn put(&mut self, mut cell: Cell) {
        cell.step = self.step;
        //self.field.put(cell.id);
        self.runnable.push((cell.id, self.step));
        self.cells.insert(cell.id, cell);
    }

    pub fn step(&mut self) {
        // ignore spawns for the moment


        //this can be executed in parallel to the blocking loop
        //access to datastructures must be synchronized for that
        //currently this only executes things for the current step
        //this things COULD be executed speculative to enhance multiprocessing capability
        //there is no external dependency for
        while self.runnable.peek().map_or(
            false, // runnablequeue is empty -> return false
            |c| c.1 == self.step
            //c.step == self.step
        ) {
            let cell = self.cells.get_mut(&self.runnable.pop().unwrap().0).unwrap();
            println!("found {}",cell.id);
            if let Some(eventtype) = cell.step() {
                self.blocking.push(Event::new(eventtype, cell));
            } else {
                self.runnable.push((cell.id,cell.step));
            }
        }

        // while there are Events for the current step
        // execute them
        //only do this if all cells finished running in this step
        while self.blocking.peek().map_or(
            false, // blockingqueue is empty -> return false
            |e| e.step == self.step
        ) {
            let e = self.blocking.pop().unwrap(); //there must be a value
            e.execute(self); //the event moves here into execute
        }

        self.step += 1;
    }
}

use std::fmt;
impl fmt::Debug for Env {
    fn fmt(&self, f: &mut fmt::Formatter) -> Result<(), fmt::Error> {
        write!(
            f,
            "Env{{step: {}, energy: {}, cells: {:?}}}",
            self.step,
            self.nrg,
            self.cells,
            /*self.field, field: {:?}*/
        )
    }
}

#[derive(Debug,Clone,Copy,Eq,PartialEq)]
pub enum Direction{
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
}

impl Direction {
    pub fn from_coords(x: i64, y: i64) -> Direction {
        let x = x.signum();
        let y = y.signum();
        match (x,y) {
            ( 0, 1) => Direction::North,
            ( 1, 1) => Direction::NorthEast,
            ( 1, 0) => Direction::East,
            ( 1,-1) => Direction::SouthEast,
            ( 0,-1) => Direction::South,
            (-1,-1) => Direction::SouthWest,
            (-1, 0) => Direction::West,
            (-1, 1) => Direction::NorthWest,
            _       => panic!("invalid direction")
        }
    }
}

#[derive(Debug,Clone,Copy,Eq,PartialEq)]
pub enum EventType{
    Mov   { dir: Direction },
    Eat   { dir: Direction },
    Spawn { dir: Direction, nrg: u64 },
    Dead,
}

#[derive(Debug)]
struct Event{
    ty: EventType,
    id: CellID,
    step: u64,
}

impl Event {
    pub fn new(ty: EventType, cell: &mut Cell) -> Event {
        Event{
            ty: ty,
            id: cell.id,
            step: cell.step,
        }
    }

    pub fn execute(self, env: &mut Env) {
        // fixme: this just drops the event...
        let cell = env.cells.get_mut(&self.id).unwrap();
        // manipulate the environment accordingly,
        // push result to cells data stack
        // increment cells step
        cell.step += 1;
        // insert cell into runnable
        env.runnable.push((cell.id, cell.step));
        // increment cellcount of cells step
    }
}

impl Eq for Event {}

impl PartialEq for Event {
    fn eq(&self, other: &Event) -> bool {
        self.ty == other.ty
        && self.id.eq(&other.id)
    }
}

// Events have an ordering based on their step
impl Ord for Event {
    fn cmp(&self, other: &Event) -> Ordering {
        // Notice that the we flip the ordering here
        other.step.cmp(&self.step)
    }
}

// `PartialOrd` needs to be implemented as well.
impl PartialOrd for Event {
    fn partial_cmp(&self, other: &Event) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}
