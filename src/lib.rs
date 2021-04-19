use bakkesmod::prelude::*;
use bakkesmod::{game, console};

#[plugin_init]
pub fn on_load() {
    // register a 'notifier' (a console command)
    console::register_notifier("my_notifier", Box::new(move |params: Vec<String>| {
        // callback code
    }));

    // register a hook on a game event
    game::hook_event("Function Engine.GameViewportClient.Tick", Box::new(move || {
        // callback code
    }));

    // use a normal function instead of a lambda
    game::hook_event("Function TAGame.Car_TA.ApplyBallImpactForces", Box::new(my_callback));
}

fn my_callback() {
    // callback code
}
