#pragma once

class NoRotate : public Module
{
public:
    NoRotate(int keybind = Keys::NONE, bool enabled = false) :
        Module("NoRotate", "Player", "Prevents servers from setting your bodyyaw rotation", keybind, enabled)
    {

    }
};