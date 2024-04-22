#pragma once

class RedstoneESP : public Module
{
public:
    RedstoneESP(int keybind = Keys::NONE, bool enabled = false) :
        Module("RedstoneESP", "Visual", "Esp for redstone ores", keybind, enabled)
    {
        addBool("Lit Redstone", "include lit redstone", &litredstone);
        addSlider("Range", "How far around you is esp rendered", &range, 1, 100);
    }

    bool litredstone = true;
    float range = 20;

    bool render = false;
    Vector3<int> blockPos;

    Vector3<int> getBlockPosition()
    {
        auto localPlayer = Global::getClientInstance()->getLocalPlayer();
        //if (localPlayer == nullptr) return;

        Vector3<float> pos = localPlayer->getAABBShape()->PosLower;

        return { (int)floor(pos.x), (int)floor(pos.y), (int)floor(pos.z) };
    }

    void onEvent(RenderContextEvent* event) override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (player == nullptr) {
            return;
        }

        Vector3<int> playerBlockPos = getBlockPosition();

        for (int x = -range; x <= range; x++) {
            for (int y = -range; y <= range; y++) {
                for (int z = -range; z <= range; z++) {
                    blockPos = Vector3<int>(playerBlockPos.x + x, playerBlockPos.y + y, playerBlockPos.z + z);

                    BlockSource* source = Global::getClientInstance()->getBlockSource();

                    if (!source)
                        return;

                    Block* block = source->getBlock(blockPos);

                    if (block->GetBlockLegacy()->getBlockID() == 73 || (litredstone && block->GetBlockLegacy()->getBlockID() == 74)) {
                        render = true;
                        blockPos = Vector3<int>(playerBlockPos.x + x, playerBlockPos.y + y, playerBlockPos.z + z);
                        //ImRenderUtil::drawBox(blockPos.ToFloat(), blockPos.ToFloat(), UIColor(199, 199, 199), UIColor(0, 0, 0), 4, true, true);
                    }
                }
            }
        }
    }

    void onEvent(ImGuiRenderEvent* event) override {
        if (render) {
            ImRenderUtil::drawBox(blockPos.ToFloat(), blockPos.ToFloat(), UIColor(199, 199, 199), UIColor(0, 0, 0), 4, true, true);
        }
    }

    void onDisabled() override {
        this->toggle();
    }
};