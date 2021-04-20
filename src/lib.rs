use std::cell::RefCell;
use std::cell::RefMut;
use std::rc::Rc;

use bakkesmod::prelude::*;
use bakkesmod::{game};

struct Plugin {
    in_goal_replay: bool,
    // running_average: f32,
    // samples: u32,
}

// fn hook_event(_name: &str, mut f: Box<dyn FnMut()>) {
//     f();
// }

fn hook(name: &str, plugin: &Rc<RefCell<Plugin>>, mut f: Box<dyn FnMut(RefMut<Plugin>)>) {
    let plugin_ref = Rc::clone(plugin);

    game::hook_event(name, Box::new(move || f(plugin_ref.borrow_mut())));
}

fn process_hooks(plugin_base: &Rc<RefCell<Plugin>>) {
    hook("Function GameEvent_Soccar_TA.ReplayPlayback.BeginState", &plugin_base,
        Box::new(|mut p| {
            p.in_goal_replay = true;
        }));

    hook("Function GameEvent_Soccar_TA.ReplayPlayback.EndState", &plugin_base,
        Box::new(|mut p| {
            p.in_goal_replay = false;
        }));
}

#[plugin_init]
pub fn on_load() {
    let plugin = Plugin{
        in_goal_replay: false,
        // running_average: 0.0,
        // samples: 0,
    };

    let plugin_base = Rc::new(RefCell::new(plugin));
    process_hooks(&plugin_base);

    let plugin_ref = Rc::clone(&plugin_base);
    println!("{}", plugin_ref.borrow().in_goal_replay);
}
