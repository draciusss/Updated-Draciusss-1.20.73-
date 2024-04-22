#pragma once

class FastUse : public Module
{
public:
    FastUse(int keybind = 7, bool enabled = true) :
        Module("FastUse", "Misc", "Uses bows and other stuff faster.", keybind, enabled)
    {
        addSlider("Delay", "The delay between usage.", &delay, 1, 10);
    }

    float delay = 1;
    int charge = 0;

    void onEvent(RenderContextEvent* event) override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        auto gamemode = player->getGamemode();

        if (!player || !gamemode) { return; }

        charge++;

        if (charge >= delay) {
            gamemode->releaseUsingItem();
            charge = 0;
        }
    }

    std::string getModeName() override {
        char str[10];
        sprintf_s(str, 10, "%.1f", delay);
        return " " + std::string(str);
    }
};