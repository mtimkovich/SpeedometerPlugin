#pragma once
#pragma comment(lib, "PluginSDK.lib")
#include "bakkesmod/plugin/bakkesmodplugin.h"

class Speedometer : public BakkesMod::Plugin::BakkesModPlugin
{
private:
    bool isInGoalReplay = false;
    float runningAverage = 0;
    int samples = 0;

    const std::string countdownEvent = "Function GameEvent_TA.Countdown.BeginState";

    std::shared_ptr<int> xPos;
    std::shared_ptr<int> yPos;

    float getCarSpeed();
    void updateAverage(float);
    void resetAverage(std::string);
    void drawSpeed(CanvasWrapper, std::string, float, int);
    void Render(CanvasWrapper);

public:
    void onLoad() override;
    void onUnload() override;
};
