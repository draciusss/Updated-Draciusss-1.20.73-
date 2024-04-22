#pragma once

class FastStop : public Module
{
public:
    FastStop(int keybind = Keys::NONE, bool enabled = false) :
        Module("FastStop", "Movement", "Quickly stops the player's movement", keybind, enabled)
    {
        addSlider("Speed", "The speed of which to stop", &stopAmount, 2, 6);
    }

    float stopAmount = 6;

    void onEvent(KeyboardEvent* event) override
    {
        int key = *event->Key;

        Player* player = Global::getClientInstance()->getLocalPlayer();
        ClientInstance* instance = Global::getClientInstance();

        // Check if player or valid key is null
        if (!player || key != GameInput::Forwards && key != GameInput::Left && key != GameInput::Backwards && key != GameInput::Right)
            return;

        Global::Keymap[key] = *event->Held; // Update key state

        bool w = Global::Keymap[GameInput::Forwards];
        bool a = Global::Keymap[GameInput::Left];
        bool s = Global::Keymap[GameInput::Backwards];
        bool d = Global::Keymap[GameInput::Right];

        bool isMoving = instance->getMinecraftGame()->getCanUseKeys() && (w || a || s || d);

        if (!isMoving)
        {
            StateVectorComponent* stateVec = player->getStateVector();

            // If state vector component is missing then no further action needed
            if (!stateVec)
                return;

            if (stopAmount != 6)
            {
                // Adjust velocity based on stop amount
                stateVec->Velocity.x /= stopAmount;
                stateVec->Velocity.z /= stopAmount;
            }
            else
            {
                // Set velocity to zero
                stateVec->Velocity.x = 0;
                stateVec->Velocity.z = 0;
            }
        }
    }
};