#include "Speedometer.h"
#include "bakkesmod/wrappers/includes.h"
#include "bakkesmod/wrappers/arraywrapper.h"

using namespace std;

BAKKESMOD_PLUGIN(Speedometer, "Show car speed", "0.1", PLUGINTYPE_FREEPLAY)

void Speedometer::onLoad()
{
	useMetric = make_shared<bool>(false);

    cvarManager->registerCvar("Speedometer_Metric", "0", "Show speed in KPH or MPH")
        .bindTo(useMetric);

	xPos = make_shared<int>(0);
	yPos = make_shared<int>(0);
	cvarManager->registerCvar("Speedometer_Position", "25", "Horizontal position of speedometer",
            true, true, 0, true, 1920).bindTo(xPos);
	cvarManager->registerCvar("Speedometer_Position", "245", "Vertical position of speedometer",
            true, true, 0, true, 1080).bindTo(yPos);

    gameWrapper->HookEvent(
            "Function GameEvent_Soccar_TA.ReplayPlayback.BeginState",
            [&](std::string eventName){ isInGoalReplay = true; });
    gameWrapper->HookEvent(
            "Function GameEvent_Soccar_TA.ReplayPlayback.EndState",
            [&](std::string eventName){isInGoalReplay = false; });

	gameWrapper->RegisterDrawable(bind(&Speedometer::Render, this, std::placeholders::_1));
}

void Speedometer::onUnload(){}

int toMph(float gameSpeed) {
    return (int) (gameSpeed / 44.704);
}

int toKph(float gameSpeed) {
    return (int) (gameSpeed * 0.036);
}

Vector2 Speedometer::textPosition(CanvasWrapper canvas) {
	float SCREEN_WIDTH = canvas.GetSize().X;
	float SCREEN_HEIGHT = canvas.GetSize().Y;

	float relativeHeight = SCREEN_HEIGHT / 1080.f;
	float boxHeight = 40.f * relativeHeight;

    return Vector2{*xPos, *yPos};
}

// All draw functions live here. Abstract away from this as much as possible.
void Speedometer::drawInt(CanvasWrapper canvas, int number) {
    canvas.SetColor(255, 255, 255, 255);
    canvas.SetPosition(textPosition(canvas));
    string output = "Speed: " + to_string(number) + " mph";
    canvas.DrawString(output);
}

void Speedometer::Render(CanvasWrapper canvas) {
    // Checks whether we should render.
	CameraWrapper camera = gameWrapper->GetCamera();

	if (isInGoalReplay || camera.IsNull()) {
		return;
	}

	PriWrapper target = PriWrapper(
            reinterpret_cast<std::uintptr_t>(
                gameWrapper->GetCamera().GetViewTarget().PRI));
	if (target.IsNull()) {
		return;
	}

	CarWrapper car = target.GetCar();
    if (car.IsNull()) {
        return;
    }

	float carSpeed = car.GetVelocity().magnitude();

    int humanSpeed = toMph(carSpeed);
    drawInt(canvas, humanSpeed);
}
