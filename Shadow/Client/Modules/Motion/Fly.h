#pragma once

class Fly : public Module
{
public:
    Fly(int keybind = 70, bool enabled = false) :
        Module("Fly", "Motion", "Fly like a superhero.", keybind, enabled)
    {

    }

    void onEvent(RenderContextEvent* event) override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getTimerClass())
            return;

        if (!Global::getClientInstance()->getLocalPlayer() || !Global::getClientInstance()->getLocalPlayer()->getStateVector())
            return;

        Player* player = Global::getClientInstance()->getLocalPlayer();
        StateVectorComponent* state = player->getStateVector();

        state->Velocity.y = 0;

        float yaw = (float)player->getMovementInterpolator()->Rotations.y;
        float speed = 0.65f;
        if (Global::Keymap['Q']) {
            state->Velocity.z = sin((yaw + 90) * 0.01745329251f) * speed; // 0.01745329251f
            state->Velocity.x = cos((yaw + 90) * 0.01745329251f) * speed;
        }
        if (Global::Keymap['A']) {
            state->Velocity.z = sin((yaw + 0) * 0.01745329251f) * speed;
            state->Velocity.x = cos((yaw + 0) * 0.01745329251f) * speed;
        }
        if (Global::Keymap['S']) {
            state->Velocity.z = sin((yaw - 90) * 0.01745329251f) * speed;
            state->Velocity.x = cos((yaw - 90) * 0.01745329251f) * speed;
        }
        if (Global::Keymap['D']) {
            state->Velocity.z = sin((yaw + 180) * 0.01745329251f) * speed;
            state->Velocity.x = cos((yaw + 180) * 0.01745329251f) * speed;
        }

        if (Global::Keymap[VK_SPACE]) {
            player->getStateVector()->Velocity.y += 0.3;
        }
        else if (Global::Keymap[VK_SHIFT]) {
            player->getStateVector()->Velocity.y -= 0.3;
        }
    }

    void onDisabled() override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getTimerClass())
            return;

        Global::getClientInstance()->getTimerClass()->setMainTimerSpeed(20);
    }

    std::string getModeName() override {
        return "";
    }
};