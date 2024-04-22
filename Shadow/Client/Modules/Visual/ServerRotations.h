#pragma once

class ServerRotations : public Module
{
public:
    ServerRotations(int keybind = Keys::NONE, bool enabled = false) :
        Module("ServerRotations", "Visual", "Client Sided rotations", keybind, enabled)
    {

    }

    void onEvent(RenderContextEvent* event) override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (!player) return;

        //auto headYaw = player->getHeadYaw().y;
        auto pitch = player->getMovementInterpolator()->Rotations.x;

        //Global::easedHeadYaw = Math::wrap(Global::easedHeadYaw, headYaw - 180.f, headYaw + 180.f);
        //Global::easedHeadYaw = Math::lerp(Global::easedHeadYaw, headYaw, 0.016f * 2.07);

        Global::easedBodyYaw = Math::wrap(Global::easedBodyYaw, Global::easedHeadYaw - 180.f, Global::easedHeadYaw + 180.f);
        Global::easedBodyYaw = Math::lerp(Global::easedBodyYaw, Global::easedHeadYaw, 0.016f * 1.04);

        Global::easedPitch = Math::lerp(Global::easedPitch, pitch, 0.016f * 2.07);

        //player->getActorHeadRotationComponent()->rotation.y = Global::easedHeadYaw;
        //player->setBodyRotation(Global::easedBodyYaw);
    }

    void onDisabled() override {
    }
};