#pragma once

class HiveTimer : public Module
{
public:
    HiveTimer(int keybind = 86, bool enabled = false) :
        Module("HiveTimer", "Misc", "Increase your game tick rate.", keybind, enabled)
    {
        addSlider("Timer", "Sex + Hive", &timerSpeed, 0, 24);
    }

    float timerSpeed = 12;

    void onEvent(RenderContextEvent* event) override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getTimerClass())
            return;

        Global::getClientInstance()->getTimerClass()->setMainTimerSpeed(timerSpeed);
    }

    void onDisabled() override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getTimerClass())
            return;

        Global::getClientInstance()->getTimerClass()->setMainTimerSpeed(20);
    }

    std::string getModeName() override {
        char str[10];
        sprintf_s(str, 10, "%.1f", timerSpeed);
        return " " + std::string(str);
    }
};