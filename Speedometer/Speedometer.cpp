#include "Speedometer.h"
#include "bakkesmod/wrappers/includes.h"
#include "bakkesmod/wrappers/arraywrapper.h"

using namespace std;

BAKKESMOD_PLUGIN(Speedometer, "Speedometer", "0.1", PLUGINTYPE_FREEPLAY)

void Speedometer::onLoad() {
    useMetric = make_shared<bool>(false);

    cvarManager->registerCvar("Speedometer_Metric", "0", "Show speed in KPH or MPH")
        .bindTo(useMetric);

    xPos = make_shared<int>(0);
    yPos = make_shared<int>(0);
    cvarManager->registerCvar("Speedometer_X_Position", "25", "X position",
            true, true, 0, true, 2560).bindTo(xPos);
    cvarManager->registerCvar("Speedometer_Y_Position", "245", "Y position",
            true, true, 0, true, 1440).bindTo(yPos);

    gameWrapper->HookEvent(
            "Function GameEvent_Soccar_TA.ReplayPlayback.BeginState",
            [&](std::string eventName){ isInGoalReplay = true; });
    gameWrapper->HookEvent(
            "Function GameEvent_Soccar_TA.ReplayPlayback.EndState",
            [&](std::string eventName){isInGoalReplay = false; });

    gameWrapper->RegisterDrawable(bind(&Speedometer::Render, this, std::placeholders::_1));
}

void Speedometer::onUnload() {}

string toMph(float gameSpeed) {
    return to_string((int) (gameSpeed / 44.704)) + " mph";
}

string toKph(float gameSpeed) {
    return to_string((int) (gameSpeed * 0.036)) + " kph";
}

// All draw methods live here.
void Speedometer::drawText(CanvasWrapper canvas, string text) {
    canvas.SetColor(255, 255, 255, 255);
    canvas.SetPosition(Vector2{*xPos, *yPos});
    canvas.DrawString("Speed: " + text);
}

// Get the car's speed, or -1 if we don't want to render.
float Speedometer::getCarSpeed() {
    CameraWrapper camera = gameWrapper->GetCamera();

    if (isInGoalReplay || camera.IsNull()) {
        return -1;
    }

    PriWrapper target = PriWrapper(
            reinterpret_cast<std::uintptr_t>(
                gameWrapper->GetCamera().GetViewTarget().PRI));
    if (target.IsNull()) {
        return -1;
    }

    CarWrapper car = target.GetCar();
    if (car.IsNull()) {
        return -1;
    }

    return car.GetVelocity().magnitude();
}

bool validGameState(shared_ptr<GameWrapper> gameWrapper) {
    bool freeplay = gameWrapper->IsInFreeplay();
    bool training = gameWrapper->IsInCustomTraining();
    bool online = gameWrapper->IsInOnlineGame();

    return freeplay || training || online;
}

void Speedometer::Render(CanvasWrapper canvas) {
    if (!validGameState(gameWrapper)) {
        return;
    }

    float carSpeed = getCarSpeed();
    if (carSpeed == -1) {
        return;
    }

    string text = toMph(carSpeed);

    if (*useMetric) {
        text = toKph(carSpeed);
    }

    drawText(canvas, text);
}
