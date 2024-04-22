#pragma once

class AirSwim : public Module
{
public:
    AirSwim(int keybind = 7, bool enabled = false) :
        Module("AirSwim", "Motion", "Swin in the air.", keybind, enabled)
    {

    }

    void onEvent(RenderContextEvent* event) override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (!player) { return; }

        player->doWaterSplashEffect();
        player->startSwimming();
    }

    void onDisabled() override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (!player) { return; }

        player->stopSwimming();
    }

    std::string getModeName() override {
        return "";
    }
};