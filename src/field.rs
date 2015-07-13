use cell::CellID;

#[derive(Debug)]
pub struct Field{
    field: Vec<Vec<Option<CellID>>>
}


impl Field {
    pub fn new(cols: u64, rows: u64) -> Field {
        Field {
            field: vec![vec![None; rows as usize]; cols as usize],
        }
    }
}
