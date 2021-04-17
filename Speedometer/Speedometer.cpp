#include "Speedometer.h"
#include "bakkesmod/wrappers/includes.h"
#include "bakkesmod/wrappers/arraywrapper.h"

using namespace std;

BAKKESMOD_PLUGIN(Speedometer, "Show car speed", "1.3", PLUGINTYPE_FREEPLAY)

void Speedometer::onLoad()
{
	useMetric = make_shared<bool>(false);

	cvarManager->registerCvar("Speedometer_Metric", "0", "Show speed in KPH or MPH").bindTo(useMetric);

	prefPosition = make_shared<float>(0.f);
	cvarManager->registerCvar("Speedometer_Position", "90", "Vertical position of speedometer", true, true, 0, true, 100).bindTo(prefPosition);

    // ! Look up docs for gamewrapper.
    // Interesting
	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.ReplayPlayback.BeginState", [&](std::string eventName){isInGoalReplay = true;});
	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.ReplayPlayback.EndState", [&](std::string eventName){isInGoalReplay = false;});

    // Draw
	gameWrapper->RegisterDrawable(bind(&Speedometer::Render, this, std::placeholders::_1));
}
void Speedometer::onUnload(){}

int toMph(float gameSpeed) {
    return (int) (gameSpeed / 44.704);
}

int toKph(float gameSpeed) {
    return (int) (gameSpeed * 0.036);
}

Vector textPosition(CanvasWrapper canvas) {
	float SCREEN_WIDTH = canvas.GetSize().X;
	float SCREEN_HEIGHT = canvas.GetSize().Y;

	float positionPerc = *prefPosition / 100.f;

	float relativeHeight = SCREEN_HEIGHT / 1080.f;
	float boxHeight = 40.f * relativeHeight;

    Vector textPos = Vector2{
        startPosition.X + boxSize.X + 5,
        startPosition.Y + (int) (10.f * relativeHeight)
    };
}

// All draw functions live here. Abstract away from this as much as possible.
void drawInt(int number, CanvasWrapper canvas) {
    string output = format("Speed: {} mph", number);

    // White
    canvas.SetColor(255, 255, 255, 255);
    canvas.SetPosition(textPosition(canvas));
    canvas.DrawString(output);
}

// Checks whether we should render on this frame.
bool do_render() {
	CameraWrapper camera = gameWrapper->GetCamera();

	if (isInGoalReplay || camera.IsNull()) {
		return false;
	}

	PriWrapper target = PriWrapper(
            reinterpret_cast<std::uintptr_t>(gameWrapper->GetCamera().GetViewTarget().PRI));
	if (target.IsNull()) {
		return false;
	}

	CarWrapper car = target.GetCar();
    if (car.IsNull()) {
        return false;
    }

    return true;
}

void Speedometer::Render(CanvasWrapper canvas) {
    if (!do_render()) {
        return;
    }

	Vector velocity = car.GetVelocity();
	float speed = velocity.magnitude();

    int humanSpeed = toMph(speed);
    drawInt(canvas, humanSpeed);
}
