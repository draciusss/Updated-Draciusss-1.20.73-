#pragma once

class Phase : public Module
{
public:
    Phase(int keybind = 7, bool enabled = false) :
        Module("Phase", "Player", "Allow phasing through blocks", keybind, enabled)
    {

    }

    void onEvent(RenderContextEvent* event) override
    {
        Player* player = Global::getClientInstance()->getLocalPlayer();
        AABBShapeComponent* aabb = player->getAABBShape();

        if (!player) { return; }

        aabb->PosUpper.y = aabb->PosLower.y;
    }

    void onDisabled() override
    {
        Player* player = Global::getClientInstance()->getLocalPlayer();
        AABBShapeComponent* aabb = player->getAABBShape();

        if (!player) { return; }

        aabb->PosUpper.y += 1.8f;

        //player->SetPos(aabb->r_Pos_Lower);
    }
};