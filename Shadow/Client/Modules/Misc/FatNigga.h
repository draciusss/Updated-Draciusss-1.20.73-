#pragma once

class Fatnigga : public Module
{
public:
    Fatnigga(int keybind = Keys::NONE, bool enabled = false) :
        Module("Fatnigga", "Misc", "Nukes blocks in a radius around you", keybind, enabled)
    {
        addSlider("Range", "How far around you is regened", &radiusregen, 1, 6);
        addSlider("Delay", "The delay between breaking", &delay, 1, 200);
        addBool("Render", "Render the redstone counter", &render);
        //addSlider("Delay", "The delay between destroying blocs", &delay, 1, 20);
    }

private:
    float radiusregen = 1.5;
    bool render = true;

    int enabledticks;
    bool hasDestroyed = false;
    bool destroy = false;
    bool isregen = false;

    int tick = 0;
    int slot = 0;
    bool firstbreak = false;
    bool isCurrentlyMining = false;

    float delay = 120;
    bool haspickaxe = true;
public:
    Vector3<int> GetBlockPosition()
    {
        auto localPlayer = Global::getClientInstance()->getLocalPlayer();
        //if (localPlayer == nullptr) return;

        Vector3<float> pos = localPlayer->getAABBShape()->PosLower;

        return { (int)floor(pos.x), (int)floor(pos.y), (int)floor(pos.z) };
    }

    void onEnabled() override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (player == nullptr) return;

        firstbreak = false;
        isCurrentlyMining = false;
        isregen = true;
        enabledticks = 0;
    }

    const static Vector2<float> CalcAngleRegen(Vector3<float> ths, Vector3<float> dst)
    {
        Vector3 diff = dst.submissive(ths);

        diff.y = diff.y / diff.magnitude();
        Vector2<float> angles;
        angles.x = asinf(diff.y) * -(180.0f / 3.1415927f);
        angles.y = (float)-atan2f(diff.x, diff.z) * (180.0f / 3.1415927f);

        return angles;
    }

    void onEvent(RenderContextEvent* event) override
    {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (player == nullptr) {
            return;
        }

        GameMode* gamemode = player->getGamemode();

        if (!gamemode || !player)
            return;

        enabledticks++;

        PlayerInventory* supplies = player->getSupplies();

        isregen = true;
        haspickaxe = true;

        if (enabledticks > delay) {
            isregen = true;
        }
        else {
            isregen = false;
        }

        Vector3<int> playerBlockPos = GetBlockPosition();

        for (int x = -radiusregen; x <= radiusregen; x++) {
            for (int y = -radiusregen; y <= radiusregen; y++) {
                for (int z = -radiusregen; z <= radiusregen; z++) {
                    Vector3<int> blockPos = Vector3<int>(playerBlockPos.x + x, playerBlockPos.y + y, playerBlockPos.z + z);

                    BlockSource* source = Global::getClientInstance()->getBlockSource();

                    if (!source)
                        return;

                    Block* block = source->getBlock(blockPos);

                    if ((block->GetBlockLegacy()->getBlockID() != 73 ||
                        block->GetBlockLegacy()->getBlockID() != 74 ||
                        block->GetBlockLegacy()->getBlockID() != 56 ||
                        block->GetBlockLegacy()->getBlockID() != 129 ||
                        block->GetBlockLegacy()->getBlockID() != 265) &&
                        ((gamemode->getlastBreakProgress() * 100) >= 30)
                        )
                    {
                        destroy = true;
                        gamemode->destroyBlock(blockPos, 1, true); // is safe there
                        isCurrentlyMining = true;
                    }
                    else continue;


                    /*Vector2<float> angle = CalcAngleRegen(player->getStateVector()->Position, blockPos.ToFloat());

                    player->getMovementInterpolator()->Rotations = angle;*/
                }
            }
        }
    }

    void onDisabled() override {
        destroy = false;
        isregen = false;
    }
};