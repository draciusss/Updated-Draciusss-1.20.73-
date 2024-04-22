#pragma once

class Speed : public Module
{
public:
    Speed(int keybind = Keys::X, bool enabled = false) :
        Module("Speed", "Motion", "Bhop like the flash.", keybind, enabled)
    {
        addEnum("Mode", "The mode of the speed", { "Bunnyhop", "Friction" }, &mode);
        addSlider("Speed", "How fast you will go", &speed, 1, 10);
        // desync
        addSlider("Height", "How fast you will go", &height, 0, 7);
        addSlider("Friction", "How fast you will slowdown off ground", &friction, 0, 10);
        addBool("Timer boost", "Increase the game tick rate", &timerBoost);
        addSlider("Timer", "Increase the game timer", &timerSpeed, 0, 60);
        addBool("Fast Fall", "Fast falls into the ground", &fastfall);
        addSlider("Fall Speed", "The amount speed to fall (only when fast fall enabled)", &fastfallspeed, 2, 50);
    }

    float speed = 3.5f;
    float height = 4.2f;
    float friction = 9.3f;

    bool timerBoost = false;
    float timerSpeed = 20;

    bool fastfall = false;
    float fastfallspeed = 20;

    int mode = 0;

    void onEvent(RenderContextEvent* event) override {
        if (!Global::getClientInstance() || !Global::getClientInstance()->getLocalPlayer() || !Global::getClientInstance()->getLocalPlayer()->getStateVector()|| !Global::getClientInstance()->getTimerClass())
            return;

        Player* player = Global::getClientInstance()->getLocalPlayer();
        StateVectorComponent* state = player->getStateVector();

        player->setSprinting(true);

        if (timerBoost) {
            Global::getClientInstance()->getTimerClass()->setMainTimerSpeed(timerSpeed);
        }
        else {
            Global::getClientInstance()->getTimerClass()->setMainTimerSpeed(20);
        }

        auto onGround = player->isOnGround();

        if (fastfall) {
            if (onGround && state->Velocity.y < 0 && !(state->Velocity.y < -1))
            {
                state->Velocity.y = -(speed / 20);
            }
        }

        bool keyPressed = Global::getClientInstance()->getMinecraftGame()->getCanUseKeys() && (Global::Keymap[GameInput::Forwards] || Global::Keymap[GameInput::Left] || Global::Keymap[GameInput::Backwards] || Global::Keymap[GameInput::Right]);

        float yaw = (float)player->getMovementInterpolator()->Rotations.y;

        if (mode == 0) { // Bunnyhop
            // Are we holding movement keys?
            if (keyPressed)
            {
                // Should we jump?
                if ((height / 10) > 0 && onGround)
                {
                    player->jumpFromGround();
                    state->Velocity.y = height / 10;
                }
                MovementUtils::setSpeed(speed / 10);
            }
        }

        if (mode == 1) { // Friction
            static float currentSpeed = speed / 10;
            // Are we holding movement keys?
            if (keyPressed)
            {
                // Should we jump?
                if ((height / 10) > 0 && onGround)
                {
                    player->jumpFromGround();
                    state->Velocity.y = height / 10;

                    // Set our speed onGround
                    currentSpeed = speed / 10;
                }
                MovementUtils::setSpeed(currentSpeed);
            }

            // Apply air friction
            float frictionFactor = friction / 10; // Adjust this value to control the rate of speed reduction
            currentSpeed *= (1 - frictionFactor * 0.016f);
        }
    }

    void onDisabled() override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getTimerClass())
            return;

        Global::getClientInstance()->getTimerClass()->setMainTimerSpeed(20);
    }
};