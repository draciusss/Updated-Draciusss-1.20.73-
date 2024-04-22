#pragma once

class InvMove : public Module
{
public:
    InvMove(int keybind = Keys::NONE, bool enabled = false) :
        Module("InvMove", "Player", "Allow you to move while in Inventory's GUI", keybind, enabled)
    {
        addSlider("Speed", "The speed which the player moves in", &speed, 0.3, 0.5);
        addSlider("Height", "The height which the player jumps in", &height, 0.3, 0.5);
    }

    float speed = 0.432f;
    float height = 0.42f;

    void onEvent(RenderContextEvent* event) override
    {
        if (!Global::getClientInstance()->getLocalPlayer())
            return;

        Player* player = Global::getClientInstance()->getLocalPlayer();

        MovementInterpolatorComponent* movement = player->getMovementInterpolator();
        StateVectorComponent* stateVec = player->getStateVector();

        if (!movement || !stateVec)
            return;

        bool w = Global::Keymap[GameInput::Forwards];
        bool a = Global::Keymap[GameInput::Left];
        bool s = Global::Keymap[GameInput::Backwards];
        bool d = Global::Keymap[GameInput::Right];

        bool pressed = !Global::getClientInstance()->getMinecraftGame()->getCanUseKeys() && (w || a || s || d);
        auto calcYaw = movement->Rotations.y;

        updateYaw(w, a, s, d, calcYaw);

        auto calcYawF = calcYaw * (3.1415927f / 180);
        if (pressed)
        {
            if (player->isOnGround() && Global::Keymap[Keys::SPACEBAR])
                stateVec->Velocity.y = height;

            stateVec->Velocity.x = cos(calcYawF) * (speed / 2);
            stateVec->Velocity.z = sin(calcYawF) * (speed / 2);
        }
    }

    void updateYaw(bool w, bool a, bool s, bool d, float& calcYaw)
    {
        if (w)
        {
            if (!a && !d)
                calcYaw += 90.f;
            else if (a)
                calcYaw += 45.f;
            else if (d)
                calcYaw += 135.f;
        }
        else if (s)
        {
            if (!a && !d)
                calcYaw -= 90.f;
            else if (a)
                calcYaw -= 45.f;
            else if (d)
                calcYaw -= 135.f;
        }
        else if (!w && !s)
        {
            if (!a && d)
                calcYaw += 180.f;
        }
        if (w && s) {
            calcYaw -= 90.f;
        }
    }
};