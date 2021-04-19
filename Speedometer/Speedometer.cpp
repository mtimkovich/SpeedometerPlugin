#include "Speedometer.h"
#include <sstream>
#include "bakkesmod/wrappers/includes.h"

using namespace std;

BAKKESMOD_PLUGIN(Speedometer, "Speedometer", "0.1", PLUGINTYPE_FREEPLAY)

void Speedometer::onLoad() {
    xPos = make_shared<int>(0);
    yPos = make_shared<int>(0);
    cvarManager->registerCvar("Speedometer_X_Position", "25", "X position",
            true, true, 0, true, 2560).bindTo(xPos);
    cvarManager->registerCvar("Speedometer_Y_Position", "245", "Y position",
            true, true, 0, true, 1440).bindTo(yPos);

    gameWrapper->HookEvent(
            "Function GameEvent_Soccar_TA.ReplayPlayback.BeginState",
            [&](string eventName){ isInGoalReplay = true; });
    gameWrapper->HookEvent(
            "Function GameEvent_Soccar_TA.ReplayPlayback.EndState",
            [&](string eventName){ isInGoalReplay = false; });

    gameWrapper->HookEvent(
            "Function TAGame.GameEvent_TA.PostBeginPlay",
            std::bind(&Speedometer::resetAverage, this, std::placeholders::_1));
    gameWrapper->HookEvent(
            countdownEvent,
            std::bind(&Speedometer::resetAverage, this, std::placeholders::_1));

    gameWrapper->RegisterDrawable(bind(&Speedometer::Render, this, std::placeholders::_1));
}

void Speedometer::onUnload() {}

void Speedometer::resetAverage(string eventName) {
    if (eventName == countdownEvent && !gameWrapper->IsInFreeplay()) {
        return;
    }

    samples = 0;
}

int toMph(float gameSpeed) {
    return (int) (gameSpeed / 44.704);
}

int toKph(float gameSpeed) {
    return (int) (gameSpeed * 0.036);
}

string speedOutput(float speed, bool metric) {
    if (metric) {
        return to_string(toKph(speed)) + " kph";
    } else {
        return to_string(toMph(speed)) + " mph";
    }
}

// All draw methods live here.
void Speedometer::drawSpeed(CanvasWrapper canvas, string label, float speed, int index) {
    const int offset = 20;
    int y = *yPos + index * offset;

    canvas.SetColor(255, 255, 255, 255);
    canvas.SetPosition(Vector2{*xPos, y});

    string units = speedOutput(speed, gameWrapper->GetbMetric());
    ostringstream output;
    output << label << ": " << units;
    canvas.DrawString(output.str());
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
    bool replay = gameWrapper->IsInReplay();

    return freeplay || training || online || replay;
}

void Speedometer::updateAverage(float carSpeed) {
    samples += 1;

    if (samples == 1) {
        runningAverage = carSpeed;
        return;
    }

    runningAverage -= runningAverage / samples;
    runningAverage += carSpeed / samples;
}

void Speedometer::Render(CanvasWrapper canvas) {
    if (!validGameState(gameWrapper)) {
        return;
    }

    float carSpeed = getCarSpeed();
    if (carSpeed == -1) {
        return;
    }

    updateAverage(carSpeed);

    drawSpeed(canvas, "Speed", carSpeed, 0);
    drawSpeed(canvas, "Average", runningAverage, 1);
}
