pub type Number = i64;
pub type CB = usize;
pub type IP = usize;
pub type CellID = u64;
pub type Step = u64;
pub type Energy = u64;
pub type Coord = u64;

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
    pub fn from_coords(x: Number, y: Number) -> Direction {
        use self::Direction::*;
        let x = x.signum();
        let y = y.signum();

        match (x,y) {
            ( 0, 1) => North,
            ( 1, 1) => NorthEast,
            ( 1, 0) => East,
            ( 1,-1) => SouthEast,
            ( 0,-1) => South,
            (-1,-1) => SouthWest,
            (-1, 0) => West,
            (-1, 1) => NorthWest,
            _       => panic!("invalid direction")
        }
    }

    pub fn get_x(self) -> i8 {
        use self::Direction::*;
        match self {
            North | South                =>  0,
            NorthEast | East | SouthEast =>  1,
            NorthWest | West | SouthWest => -1,
        }
    }

    pub fn get_y(self) -> i8 {
        use self::Direction::*;
        match self {
            East | West                   =>  0,
            NorthEast | North | NorthWest =>  1,
            SouthEast | South | SouthWest => -1,
        }
    }
}
