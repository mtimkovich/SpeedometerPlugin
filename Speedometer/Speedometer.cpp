#include "Speedometer.h"
#include "bakkesmod\wrappers\includes.h"
#include "bakkesmod\wrappers\arraywrapper.h"

using namespace std;

BAKKESMOD_PLUGIN(Speedometer, "Show car speed in freeplay", "1.3", PLUGINTYPE_FREEPLAY)

void Speedometer::onLoad()
{
	useMetric = make_shared<bool>(false);

    // keep this.
	cvarManager->registerCvar("Speedometer_Metric", "0", "Show speed in KPH or MPH").bindTo(useMetric);

	prefPosition = make_shared<float>(0.f);
	prefWidth = make_shared<float>(0.f);
	cvarManager->registerCvar("Speedometer_Position", "90", "Vertical position of speedometer", true, true, 0, true, 100).bindTo(prefPosition);
	cvarManager->registerCvar("Speedometer_Width", "60", "Width of speedometer", true, true, 0, true, 100).bindTo(prefWidth);

    // ! Look up docs for gamewrapper.
    // Interesting
	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.ReplayPlayback.BeginState", [&](std::string eventName){isInGoalReplay = true;});
	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.ReplayPlayback.EndState", [&](std::string eventName){isInGoalReplay = false;});

    // Draw
    // My game development knowledge coming in clutch.
	gameWrapper->RegisterDrawable(bind(&Speedometer::Render, this, std::placeholders::_1));
}
void Speedometer::onUnload(){}

int toMph(float gameSpeed) {
    return (int) (gameSpeed / 44.704);
}

int toKph(float gameSpeed) {
    return (int) (gameSpeed * 0.036);
}

// All draw functions live here. Abstract away from this as much as possible.
void drawInt(int number, CanvasWrapper canvas, int index) {
	float SCREEN_WIDTH = canvas.GetSize().X;
	float SCREEN_HEIGHT = canvas.GetSize().Y;
	float HALFWIDTH = SCREENWIDTH/2.f;

	float positionPerc = *prefPosition / 100.f;
	float widthPerc = *prefWidth / 100.f;

	float relativeHeight = SCREENHEIGHT / 1080.f;
	float boxHeight = 40.f * relativeHeight;

<<<<<<< HEAD

    Vector textPos = Vector2{
        startPosition.X + boxSize.X + 5,
        startPosition.Y + (int) (10.f * relativeHeight)
    };

    canvas.SetColor(255, 255, 255, 255);
    canvas.SetPosition(startPosition.minus(Vector2{60,(int)(-10.f * relativeHeight)}));
    canvas.DrawString("Speed");

    // TODO: We draw to one fixed point, and all the elements propogate from that space.
    canvas.SetPosition(textPos);
    canvas.DrawString(to_string(number) + " mph");
}

bool do_render() {
	CameraWrapper camera = gameWrapper->GetCamera();

	if (isInGoalReplay || camera.IsNull()) {
		return false;
=======
	Vector2 startPosition = {(int)(HALFWIDTH - HALFWIDTH * widthPerc), (int)(SCREENHEIGHT * positionPerc)};
	Vector2 boxSize = {(int)(SCREENWIDTH * widthPerc), boxHeight};

	if(*showSpeed)
	{
		Vector velocity = car.GetVelocity();
		float speed = velocity.magnitude();
		float speedPerc = speed/2300.f;
		float supersonicIn = 2200.f/2300.f;
		float supersonicOut = 2100.f/2300.f;

		if(speed >= 2200)
			wasSuperSonic = true;
		else
		{
			if(wasSuperSonic && (speed >= 2100 && speed < 2200))
				timeStartedBuffer = chrono::steady_clock::now();
			wasSuperSonic = false;
		}
		chrono::duration<double> bufferDuration = chrono::duration_cast<chrono::duration<double>>(chrono::steady_clock::now() - timeStartedBuffer);
		float bufferPerc = 1 - bufferDuration.count();
		if(bufferPerc < 0 || speed < 2100) bufferPerc = 0;

		//draw base color
		canvas.SetColor(255,255,255,(char)(100**prefOpacity));
		canvas.SetPosition(startPosition);
		canvas.FillBox(boxSize);

		//draw label
		canvas.SetColor(255,255,255,(char)(255**prefOpacity));
		canvas.SetPosition(startPosition.minus(Vector2{60,(int)(-10.f * relativeHeight)}));
		canvas.DrawString("Speed");
		if(*showValues)
		{
			canvas.SetPosition(Vector2{startPosition.X + boxSize.X + 5, startPosition.Y + (int)(10.f * relativeHeight)});
			if(*useMetric)
				canvas.DrawString(to_string((int)(speed* 0.036)) + " KPH");//KPH
			else
				canvas.DrawString(to_string((int)(speed/44.704)) + " MPH");//MPH
		}

		//draw speed amount
		if(car.GetbSuperSonic())
			canvas.SetColor(255,255,50,(char)(255**prefOpacity));
		else
			canvas.SetColor(255,50,50,(char)(255**prefOpacity));
		canvas.SetPosition(startPosition);
		canvas.FillBox(Vector2{(int)((float)boxSize.X * speedPerc), boxSize.Y});

		//draw outline
		canvas.SetColor(255,255,255,(char)(255**prefOpacity));
		canvas.SetPosition(startPosition.minus(Vector2{1,1}));
		canvas.DrawBox(boxSize.minus(Vector2{-2,-2}));

		//draw supersonic thresholds
		int XposIn = startPosition.X + (float)boxSize.X * supersonicIn;
		int XposOut = startPosition.X + (float)boxSize.X * supersonicOut;
		canvas.DrawLine(Vector2{XposIn, startPosition.Y}, Vector2{XposIn, startPosition.Y + (int)boxHeight}, 2.f);
		canvas.DrawLine(Vector2{XposOut, startPosition.Y}, Vector2{XposOut, startPosition.Y + (int)boxHeight}, 2.f);

		//draw supersonic buffer timer
		//base color
		Vector2 bufferTimerSize = {startPosition.X + boxSize.X - XposOut + 2, (int)(boxHeight/2)};
		Vector2 bufferTimerStart = Vector2{startPosition.X + boxSize.X - bufferTimerSize.X + 1, startPosition.Y - bufferTimerSize.Y + 1};
		canvas.SetColor(255,255,255,(char)(100**prefOpacity));
		canvas.SetPosition(bufferTimerStart);
		canvas.FillBox(bufferTimerSize);

		canvas.SetColor(255,50,50,(char)(255**prefOpacity));
		canvas.SetPosition(bufferTimerStart);
		canvas.FillBox(Vector2{(int)((float)bufferTimerSize.X * bufferPerc), bufferTimerSize.Y});

		canvas.SetColor(255,255,255,(char)(255**prefOpacity));
		canvas.SetPosition(bufferTimerStart);
		canvas.DrawBox(bufferTimerSize);
>>>>>>> parent of fa9b9a9... add Engine off line (#1)
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

// get access to the game canvas on that frame.
void Speedometer::Render(CanvasWrapper canvas) {
    if (!do_render()) {
        return;
    }

	Vector velocity = car.GetVelocity();
	float speed = velocity.magnitude();


    int humanSpeed = toMph(speed);
    drawInt(canvas, humanSpeed, textPos);
}
