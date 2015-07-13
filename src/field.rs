use cell::{ Cell, CellID };
use env::Event;

#[derive(Debug)]
pub struct Field{
    field: Vec<Vec<Tile>>
}


impl Field {
    pub fn new(cols: u64, rows: u64) -> Field {
        Field {
            field: vec![vec![Tile::Empty; rows as usize]; cols as usize],
        }
    }

    pub fn put(&mut self, cell: &Cell) {
        self.field[cell.getY()][cell.getX()] = Tile::Living(cell.id);
    }

    pub fn execute(&mut self, event: Event) {

    }

    pub fn zombify(&mut self, mut cell: Cell) {
        self.field[cell.getY()][cell.getX()] = Tile::Dead(cell.nrg);
    }
}

#[derive(Debug,Clone,Copy)]
enum Tile {
    Living(CellID),
    Dead(u64),//the energy contained in the corpse
    Empty,
}
