#pragma once

class Sprint : public Module
{
public:
    Sprint(int keybind = Keys::NONE, bool enabled = false) :
        Module("Sprint", "Movement", "Automatically sprint without holding the key.", keybind, enabled)
    {
        addBool("OmniSprint", "Automatically sprint backwards too", &omni);
    }

    bool omni = false;

    void onEvent(RenderContextEvent* event) override
    {
        Player* player = Global::getClientInstance()->getLocalPlayer();
        ClientInstance* instance = Global::getClientInstance();

        // Check if player or valid key is null
        if (!player)
            return;

        bool isMoving = instance->getMinecraftGame()->getCanUseKeys();

        if (player->getStatusFlag(MOVING)) {
            if (omni) {
                player->setSprinting(true);
            }
            else if (!omni && Global::Keymap[GameInput::Forwards]) {
                player->setSprinting(true);
            }
        }

        if (isMoving)
        {
            StateVectorComponent* stateVec = player->getStateVector();

            if (omni) {
                player->setSprinting(true);
            }
            else if (!omni && Global::Keymap[GameInput::Forwards]) {
                player->setSprinting(true);
            }
        }
        else {
            player->setSprinting(false);
  
        }
    }
};