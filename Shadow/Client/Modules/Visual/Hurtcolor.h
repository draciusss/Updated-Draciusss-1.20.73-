
#pragma once

class Hurtcolor : public Module
{
public:
    Hurtcolor(int keybind = Keys::NONE, bool enabled = false) :
        Module("Hurtcolor", "Visual", "Changes the hurt color of entities", keybind, enabled)
    {

    }

    void onEvent(ImGuiRenderEvent* event) override {

    }
};
