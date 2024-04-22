#pragma once

class Animations : public Module
{
public:
    Animations(int keybind = 7, bool enabled = true) :
        Module("Animations", "Visual", "Animations like its flux", keybind, enabled)
    {
        addBool("Exhibition", "Use taphit", &exhibition);
    }

    bool exhibition = false;

    void onEvent(RenderContextEvent* event) override {
        if (!Global::getClientInstance())
            return;
    }

    void onDisabled() override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getTimerClass())
            return;
    }

    std::string getModeName() override {
        if (exhibition) { return " " + std::string("Exhibition"); } // If Exhibition animations is on
        else { return " " + std::string("Flux"); } // else if its 1.7 animations
    }
};