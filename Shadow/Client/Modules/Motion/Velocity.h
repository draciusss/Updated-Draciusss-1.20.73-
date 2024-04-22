#pragma once

class Velocity : public Module
{
public:
    Velocity(int keybind = 7, bool enabled = true) :
        Module("Velocity", "Motion", "Increase your game tick rate.", keybind, enabled)
    {

    }

    std::string getModeName() override {
        return " " + std::string("FUllNIGGERMODE");
    }
};