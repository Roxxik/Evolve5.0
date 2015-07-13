use types::*;
use organism::OrgRef;

#[derive(Debug,Clone,Copy)]
pub enum EventType{
    Mov     { dir: Direction },
    Spawn   { dir: Direction, nrg: u64 },
    Zombify,
    Die,
}

#[derive(Debug)]
pub struct Event{
    pub ty: EventType,
    pub org: OrgRef,
}
/*
impl Event {
    pub fn execute(self, env: &mut Env) {
        match self.ty {
            EventType::Zombify => {
                env.blocking.push(Event::new(
                    EventType::Die,
                    cell.id,
                    env.step + 100,//hardcoded for now will be replaced later by some kind of decay
                ));
                //replace cell with corpse
                env.field.zombify(cell);
            }
            EventType::Die => {

            }
            _ => {
                let cell = env.cells.get_mut(&self.id).unwrap();
                env.field.execute(self, cell);
            }
        }*/
        /*
        if self.ty == EventType::Zombify {
            let cell = env.cells.remove(&self.ty.cellID).unwrap();
            env.blocking.push(Event{
                ty: EventType::Die,
                step: env.step + 100,//hardcoded for now will be replaced later by some kind of decay
            });
            //replace cell with corpse
            env.field.zombify(cell);
        } else if self.ty == EventType::Die {
            //cleanup
        } else {
            let cell = env.cells.get_mut(&self.ty.cellID).unwrap();
            env.field.execute(self, cell);
            env.runnable.push((cell.id, cell.step));
        }*/

        // fixme: this just drops the event...
        //let cell = env.cells.get_mut(&self.id).unwrap();
        // manipulate the environment accordingly,
        // push result to cells data stack
        // increment cells step
        //cell.step += 1;
        // insert cell into runnable
        //env.runnable.push((cell.id, cell.step));
        // increment cellcount of cells step
//    }
//}
