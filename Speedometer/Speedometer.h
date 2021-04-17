#pragma once
#pragma comment(lib, "PluginSDK.lib")
#include "bakkesmod/plugin/bakkesmodplugin.h"

class Speedometer : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	bool isInGoalReplay = false;
	std::shared_ptr<bool> useMetric;
	std::shared_ptr<int> xPos;
	std::shared_ptr<int> yPos;

public:
	void onLoad() override;
	void onUnload() override;

    Vector2 textPosition(CanvasWrapper canvas);
    void drawInt(CanvasWrapper canvas, int number);
	void Render(CanvasWrapper canvas);
};
