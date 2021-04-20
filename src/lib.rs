use bakkesmod::prelude::*;
use bakkesmod::{game};

struct Speedometer {
    in_goal_replay: bool,
    running_average: f32,
    samples: u32,
}

impl Speedometer {
    fn toggle_replay_state<'a>(&'a mut self) -> Box<(dyn FnMut() + 'a)> {
    }
}

#[plugin_init]
pub fn on_load() {
    let mut speedometer = Speedometer{
        in_goal_replay: false,
        running_average: 0.0,
        samples: 0,
    };

    game::hook_event("Function GameEvent_Soccar_TA.ReplayPlayback.BeginState",
        speedometer.toggle_replay_state());

    // speedometer;
}
