use std::cell::RefCell;
use std::rc::Rc;

use cell::Cell;

#[derive(Debug)]
pub struct Field{
    field: Vec<Vec<Option<Rc<RefCell<Cell>>>>>
}


impl Field {
    pub fn new(cols: u64, rows: u64) -> Field {
        Field {
            field: vec![vec![None; rows as usize]; cols as usize],
        }
    }
}
