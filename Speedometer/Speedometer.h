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

    float Speedometer::getCarSpeed();
    void drawText(CanvasWrapper canvas, std::string text);
    void Render(CanvasWrapper canvas);

public:
    void onLoad() override;
    void onUnload() override;
};
