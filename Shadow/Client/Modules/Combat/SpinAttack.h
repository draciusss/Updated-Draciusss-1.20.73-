#pragma once

class SpinAttack : public Module
{
public:
    SpinAttack(int keybind = 67, bool enabled = false) :
        Module("SpinAttack", "Combat", "Aura but spinning.", keybind, enabled)
    {
        
    }

    void onEvent(RenderContextEvent* event) override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getLocalPlayer())
            return;

        Player* player = Global::getClientInstance()->getLocalPlayer();

        player->startSpinAttack();
    }

    void onDisabled() override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getLocalPlayer())
            return;

        Player* player = Global::getClientInstance()->getLocalPlayer();

        player->stopSpinAttack();
    }

    std::string getModeName() override {
        return "";
    }
};