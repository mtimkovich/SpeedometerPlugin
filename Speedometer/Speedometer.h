#pragma once
#pragma comment(lib, "PluginSDK.lib")
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include <chrono>

class Speedometer : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	bool isInGoalReplay = false;
	std::chrono::steady_clock::time_point timeStartedBuffer;
	std::shared_ptr<bool> useMetric;
	std::shared_ptr<float> prefPosition;

public:
	void onLoad() override;
	void onUnload() override;

	void Render(CanvasWrapper canvas);
};
