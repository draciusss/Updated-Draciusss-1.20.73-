#pragma once

class NoSwing : public Module
{
public:
    NoSwing(int keybind = Keys::NONE, bool enabled = false) :
        Module("NoSwing", "Player", "Stops arm swing animation (clientside)", keybind, enabled)
    {

    }
};