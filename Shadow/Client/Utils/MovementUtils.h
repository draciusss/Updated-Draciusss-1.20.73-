#pragma once

namespace GameInput
{
    int Forwards = 'Q';
    int Backwards = 'S';
    int Left = 'A';
    int Right = 'D';
    //int Jump = Keyboard::SPACEBAR;

    /*void SetKeybinds(const char* keybinds)
    {
        // WASD
        Forwards = keybinds[0];
        Left = keybinds[1];
        Backwards = keybinds[2];
        Right = keybinds[3];
    }*/
};

class MovementUtils
{
public:
    static Vector2<float> getMoveVec()
    {
        Vector2<float> moveVec = { 0.0f, 0.0f };

        if (Global::Keymap[GameInput::Forwards])
            moveVec.x += 1.0f;

        if (Global::Keymap[GameInput::Backwards])
            moveVec.x -= 1.0f;

        if (Global::Keymap[GameInput::Left])
            moveVec.y -= 1.0f;

        if (Global::Keymap[GameInput::Right])
            moveVec.y += 1.0f;

        return moveVec;
    }

    static void setSpeed(float speed) {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (!player)
            return;

        auto velocity = player->getStateVector();
        if (!velocity) {
            return;
        }

        float calcYaw = (player->getMovementInterpolator()->Rotations.y + 90) * (PI / 180);
        float c = cos(calcYaw);
        float s = sin(calcYaw);

        Vector2<float> moveVec = getMoveVec().normalize();
        moveVec = { moveVec.x * c - moveVec.y * s, moveVec.x * s + moveVec.y * c };

        Vector3<float> movement;
        movement.x = moveVec.x * speed;
        movement.y = velocity->Velocity.y;
        movement.z = moveVec.y * speed;

        StateVectorComponent* state = player->getStateVector();

        state->Velocity.z = movement.z;
        state->Velocity.x = movement.x;
    }

    static bool isMoving() {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (!player)
            return false;

        StateVectorComponent* velocity = player->getStateVector();
        if (!velocity && !player->getStateVector())
            return false;

        return velocity->Velocity.magnitudexz() >= 0.045f;
    }
};