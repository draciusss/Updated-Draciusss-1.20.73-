#pragma once

class Regen : public Module
{
public:
    Regen(int keybind = Keys::NUM_0, bool enabled = false) :
        Module("Regen", "Misc", "Nukes blocks in a radius around you", keybind, enabled)
    {
        addSlider("Range", "How far around you is regened", &radiusregen, 1, 6);
        addSlider("Delay", "The delay between breaking", &delay, 1, 100);
        addBool("Render", "Render the redstone counter", &render);
        addBool("DigAround", "Mines around the redstone for blocks underground", &digAround);
        //addSlider("Delay", "The delay between destroying blocs", &delay, 1, 20);
    }

private:
    float radiusregen = 1.5;
    bool render = true;

    int StartTicks; //
    bool DestroyedBlock = false; // has
    bool DestroyBlock = false;

    bool digAround = true;

    int slot = 0;
    bool firstbreak = false;
    bool isCurrentlyMining = false;

    float delay = 63;
    bool haspickaxe = true;

    bool isFilledCover = false;
public:
    Vector3<int> GetBlockPosition()
    {
        auto localPlayer = Global::getClientInstance()->getLocalPlayer();
        //if (localPlayer == nullptr) return;

        Vector3<float> pos = localPlayer->getAABBShape()->PosLower;

        return { (int)floor(pos.x), (int)floor(pos.y), (int)floor(pos.z) };
    }

    static void ImScaleStart()
    {
        scale_start_index = ImGui::GetBackgroundDrawList()->VtxBuffer.Size;
    }

    static inline int scale_start_index;

    static ImVec2 ImScaleCenter()
    {
        ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX);

        const auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;
        for (int i = scale_start_index; i < buf.Size; i++)
        //  l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

        return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
    }

    static void ImScaleEnd(float scaleX, float scaleY, ImVec2 center = ImScaleCenter())
    {
        auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;

        for (int i = scale_start_index; i < buf.Size; i++)
        {
            ImVec2 pos = buf[i].pos - center;
            pos.x *= scaleX;
            pos.y *= scaleY;
            buf[i].pos = pos + center;
        }
    }

    bool selectPickaxe() {
        PlayerInventory* supplies = Global::getClientInstance()->getLocalPlayer()->getSupplies();
        Inventory* inv = supplies->inventory;
        auto prevSlot = supplies->hotbarSlot;

        for (int n = 0; n < 36; n++) {
            ItemStack* stack = inv->getItem(n);
            if (stack->item != nullptr) {
                if (stack->getItem()->isPickaxe()) {
                    if (prevSlot != n) {
                        supplies->hotbarSlot = n;
                    }
                    return true;
                }
            }
        }
        return false;
    }

    bool hasPickaxe() {
        PlayerInventory* supplies = Global::getClientInstance()->getLocalPlayer()->getSupplies();
        Inventory* inv = supplies->inventory;
        auto prevSlot = supplies->hotbarSlot;

        for (int n = 0; n < 36; n++) {
            ItemStack* stack = inv->getItem(n);
            if (stack->item != nullptr) {
                if (stack->getItem()->isPickaxe()) {
                    return true;
                }
            }
        }
        return false;
    }

    void onEnabled() override {
        auto player = Global::getClientInstance()->getLocalPlayer();
        if (player == nullptr) return;

        firstbreak = false;
        isCurrentlyMining = false;
        StartTicks = 0;
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

    bool isCovered(Vector3<int> blockPos) {
        bool isFilled = true;
        static std::vector<Vector3<int>*> checklist;
        if (checklist.empty()) {
            checklist.push_back(new Vector3<int>(0, -1, 0));
            checklist.push_back(new Vector3<int>(0, 1, 0));

            checklist.push_back(new Vector3<int>(0, 0, -1));
            checklist.push_back(new Vector3<int>(0, 0, 1));

            checklist.push_back(new Vector3<int>(-1, 0, 0));
            checklist.push_back(new Vector3<int>(1, 0, 0));
        }

        for (auto current : checklist) {
            Vector3<int> calc = blockPos.add(*current);
            if (Global::getClientInstance()->getBlockSource()->getBlock(calc)->GetMaterialType() == MaterialType::Air && Global::getClientInstance()->getBlockSource()->getBlock(calc)->GetMaterialType() == MaterialType::Water && Global::getClientInstance()->getBlockSource()->getBlock(calc)->GetMaterialType() == MaterialType::Lava) {
                isFilled = false;
                break;
            }
        }
        return isFilled;
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

        if (Global::isAttacking)
            return;

        StartTicks++;

        PlayerInventory* supplies = player->getSupplies();

        haspickaxe = true;

        Vector3<int> playerBlockPos = GetBlockPosition();

        for (int x = -radiusregen; x <= radiusregen; x++) {
            for (int y = -radiusregen; y <= radiusregen; y++) {
                for (int z = -radiusregen; z <= radiusregen; z++) {
                    Vector3<int> blockPos = Vector3<int>(playerBlockPos.x + x, playerBlockPos.y + y, playerBlockPos.z + z);

                    BlockSource* source = Global::getClientInstance()->getBlockSource();

                    if (!source)
                        return;

                    Block* block = source->getBlock(blockPos);

                    if (block->GetBlockLegacy()->getBlockID() == 73 || block->GetBlockLegacy()->getBlockID() == 74)
                    {
                        isFilledCover = isCovered(blockPos);


                        if (isCurrentlyMining) {
                            DestroyBlock = true;
                        }
                        else {
                            DestroyBlock = true;
                            gamemode->startDestroyBlock(blockPos, 1, true);
                            isCurrentlyMining = true;
                        }
                    }
                    else continue;

                    Vector2<float> angle = CalcAngleRegen(player->getStateVector()->Position, blockPos.ToFloat());

                    if (DestroyBlock) {
                        /*slot = supplies->hotbarSlot;
                        static bool restored = false;
                        if (!selectPickaxe()) {
                            if (!restored)
                                restored = true;
                            else {
                                haspickaxe = false;
                            }
                            return;
                        }
                        restored = false*/
                        if (StartTicks > (delay * 2) + 2) {
                            player->getMovementInterpolator()->Rotations = angle;
                            gamemode->destroyBlock(blockPos, 1, true); // is safe there
                            DestroyedBlock = true;
                            StartTicks = 0;
                            gamemode->stopDestroyBlock(blockPos);
                            isCurrentlyMining = false;
                        }
                        //supplies->hotbarSlot = slot;
                    }

                    if (DestroyedBlock) {
                        //DestroyBlock = false;
                        DestroyedBlock = false;
                    }

                    if (!DestroyBlock) {
                        if (isCurrentlyMining) gamemode->stopDestroyBlock(blockPos);
                        isCurrentlyMining = false;
                        StartTicks = 0;
                    }

                    player->getMovementInterpolator()->Rotations = angle;
                }
            }
        }
    }

    // 193, 54, 52

    int cool = 0;
    void onEvent(ImGuiRenderEvent* event) {
        auto instance = Global::getClientInstance();
        auto localPlayer = instance->getLocalPlayer();

        if (!localPlayer || !localPlayer->isAlive()) return;

        std::string rendertext1 = std::to_string((int)StartTicks);
        ImRenderUtil::drawText(Vector2<float>(0, 0), &rendertext1, UIColor(255, 255, 255), 1, 1.f, true);

        float percentage = (StartTicks * 100) / delay;
        if (percentage > 100) percentage = 0;

        float inScale = percentage / 100; // The animations float
        if (inScale >= 1)
            inScale = 1;

        // Scale the Regen Counter
        ImScaleStart();

        cool++;
        if ((int)percentage == 0) {
            if (cool >= 90) cool = 0;
        }
        else {
            if (cool >= 25) cool = 0;
        }

        float renderx = instance->getGuiData()->mcResolution.x / 2;
        float rendery = (instance->getGuiData()->mcResolution.y / 2) + 90;
        Vector2 renderpos = Vector2(renderx, rendery);
        std::string rendertext = "Mining (" + std::to_string((int)percentage) + "/100)";
        if ((int)percentage == 0) {
            rendertext = "Queued";
        }
        if ((int)percentage != 0 && isFilledCover && Global::isAttacking) {
            rendertext = "Covered";
        }

        float textLen = ImRenderUtil::getTextWidth(&rendertext, 1) / 2;
        float textLenTrue = ImRenderUtil::getTextWidth(&rendertext, 1);
        renderpos.x -= textLen;

        Vector4<float> rect = Vector4<float>(renderpos.x - 19, renderpos.y - 5, (renderpos.x + textLenTrue) + 10, renderpos.y + 22);
        Vector4<float> rectQueued = Vector4<float>(rect.x - 19, rect.y, rect.z + 19, rect.w);
        Vector4<float> rectSmooth = Vector4<float>(rect.x, rect.y, (rect.z - 120) + (cool * 4.4), rect.w);
        if ((int)percentage == 0) {
            rectSmooth = Vector4<float>(rect.x - 19, rect.y, ((rect.z - 90) + cool) + 19, rect.w);
        }

        if (render) {
            if (haspickaxe) {
                if ((int)percentage == 0) {
                    ImRenderUtil::fillRectangle(rectQueued, UIColor(33, 33, 33), 1.f, 10.f);
                }
                else {
                    ImRenderUtil::fillRectangle(rect, UIColor(33, 33, 33), 1.f, 10.f);
                }
                if ((int)percentage != 0 && isFilledCover && Global::isAttacking) { // Covered
                    ImRenderUtil::fillRectangle(rectSmooth, UIColor(193, 54, 52), 1.f, 10.f);
                    ImRenderUtil::fillShadowRectangle(rectSmooth, UIColor(193, 54, 52), 1, 150, 0, 10);
                }
                else {
                    if ((int)percentage == 0) { // Queued
                        ImRenderUtil::fillRectangle(rectSmooth, UIColor(201, 62, 220), 1.f, 10.f);
                        ImRenderUtil::fillShadowRectangle(rectSmooth, UIColor(201, 62, 220), 1, 150, 0, 10);
                    }
                    else { // Mining
                        ImRenderUtil::fillRectangle(rectSmooth, UIColor(212, 173, 48), 1.f, 10.f);
                        ImRenderUtil::fillShadowRectangle(rectSmooth, UIColor(212, 173, 48), 1, 150, 0, 10);
                    }
                }
                ImRenderUtil::drawText(renderpos, &rendertext, UIColor(255, 255, 255), 1, 1.f, true);
            }
        }

        // Stop the zooming animation
      //ImScaleEnd(inScale, inScale, ImVec2(rect.getCenter().x, rect.getCenter().y));
    }

    void onDisabled() override {
        DestroyBlock = false;
    }

    std::string getModeName() override {
        return std::string(" ") + std::to_string((int)delay) + "%";
    }
};