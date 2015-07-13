use std::collections::BTreeMap;

use types::*;
use field::Field;
use event::Event;
use organism::OrgRef;


//env makes makes the access to field so that the field always only displays the state at on step
#[derive(Debug)]
pub struct Env {
    step: u64,
    nrg: u64,
    field: Field,//sparse vector of all possible positions
    blocking: BTreeMap<Step, Vec<Event>>,
    runnable: BTreeMap<Step, Vec<OrgRef>>,//heap of events(action+cell) sorted by step
}

impl Env {
    pub fn new(nrg: Energy, cols: Coord, rows: Coord) -> Env {
        Env {
            step: 0,
            nrg: nrg,
            field: Field::new(cols,rows),
            blocking: BTreeMap::new(),
            runnable: BTreeMap::new(),
        }
    }
/*
    pub fn put_org(&mut self, mut cell: Cell) {
        cell.step = self.step;
        self.field.put(&cell);
        self.runnable.push((cell.id, self.step));
        self.cells.insert(cell.id, cell);
    }*/

    fn smallest_blocking(&self) -> Option<Step> {
        self.blocking.keys().next().map(|s| *s)
    }

    fn smallest_runnable(&self) -> Option<Step> {
        self.runnable.keys().next().map(|s| *s)
    }

    fn get_next_blocking(&mut self) -> Vec<Event> {
        assert!(!self.blocking.is_empty());
        let step = self.smallest_blocking().unwrap();
        self.blocking.remove(&step).unwrap()
    }

    fn get_next_runnable(&mut self) -> Vec<OrgRef> {
        assert!(!self.runnable.is_empty());
        let step = self.smallest_runnable().unwrap();
        self.runnable.remove(&step).unwrap()
    }

    fn push_blocking(&mut self, step: Step, event: Event) {
        self.blocking.entry(step).or_insert(Vec::new()).push(event);
    }

    fn push_runnable(&mut self, step: Step, org: OrgRef) {
        self.runnable.entry(step).or_insert(Vec::new()).push(org);
    }

    pub fn step(&mut self) {
        // ignore spawns for the moment

        if self.smallest_runnable().map_or(false, |s| s == self.step) {
            for orgref in self.get_next_runnable() {
                let mut org = orgref.borrow_mut();
                if let Some(eventtype) = org.step() {
                    self.push_blocking(org.step, Event{ ty: eventtype, org: orgref.clone() })
                } else {
                    self.push_runnable(org.step, orgref.clone());
                }
            }
        }

        if self.smallest_blocking().map_or(false, |s| s == self.step) {
            for event in self.get_next_blocking() {
                self.perform_event(event);
            }
        }
        self.step += 1;
    }
    fn perform_event(&mut self, event: Event) {

    }

}
