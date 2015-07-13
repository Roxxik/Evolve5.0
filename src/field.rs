use std::collections::BTreeMap;

use cell::{ Cell, CellID };
use env::{ Event, EventType };

#[derive(Debug)]
pub struct Field{
    cols: u64,
    rows: u64,
    field: BTreeMap<(u64,u64),CellState>,
}


impl Field {
    pub fn new(cols: u64, rows: u64) -> Field {
        Field{
            cols: cols,
            rows: rows,
            field: BTreeMap::new(),
        }
    }

    pub fn put(&mut self, cell: &Cell) {
        self.field.insert((cell.x, cell.y), CellState::Living(cell.id));
    }

    pub fn execute(&mut self, event: Event, cell: &mut Cell) -> Option<CellID> {
        match event.ty {
            EventType::Mov  { dir: dir }           => {
                self.field.remove((cell.x, cell.y));
                //change this to wrapping
                match self.field.insert((cell.x + dir.get_x(), cell.y + dir.get_y())) {
                    CellState::Living(id) => Some(id)
                    CellState::Dead(nrg) => {
                        cell.nrg += nrg;
                        None
                    }
                }
            }
            EventType::Spawn{ dir: dir, nrg: nrg } => {

            }
            _ => panic!("wrong event got executed in Field"),
        }
    }

    pub fn zombify(&mut self, mut cell: Cell) {
        self.field.insert((cell.x, cell.y), CellState::Dead(cell.nrg));
    }
}

#[derive(Debug,Clone,Copy)]
enum CellState {
    Living(Cell),
    Dead(u64),//the energy contained in the corpse
}
