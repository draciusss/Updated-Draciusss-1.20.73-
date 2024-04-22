#pragma once

class ExperiencePoints : public Module
{
public:
    ExperiencePoints(int keybind = 7, bool enabled = false) :
        Module("ExperiencePoints", "Player", "Increase your game experience points.", keybind, enabled)
    {
        addSlider("Amount", "Set the amount within the range of 1 to 5000", &amount, 1, 5000);
    }

    float amount = 300;

    void onEvent(RenderContextEvent* event) override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (!player) { return; }

        player->addLevels(amount);
    }

    std::string getModeName() override {
        char str[10];
        sprintf_s(str, 10, "%.0f", amount);
        return " " + std::string(str);
    }
};