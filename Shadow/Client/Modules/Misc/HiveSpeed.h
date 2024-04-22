#pragma once

class HiveSpeed : public Module
{
public:
    HiveSpeed(int keybind = 86, bool enabled = false) :
        Module("HiveSpeed", "Misc", "Increase your game tick rate.", keybind, enabled)
    {
        addEnum("Mode", "The mode of the speed", { "Hive Obv" }, &mode);
        addSlider("Timer", "Sex Speed", &timerSpeed, 0, 24);
        addBool("Delay Hits Per Second", "Delay Your Hits", &desync);
        addSlider("aeolus shit", "Desync Your speed", &desyncdelayMS, 0, 300);
    }

    int mode = 0;
    float timerSpeed = 21.5;
    bool desync = false;
    float desyncdelayMS = 0; ///possible ka range 

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
