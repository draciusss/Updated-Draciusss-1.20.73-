#pragma once

class Scaffold : public Module
{
public:
    Scaffold(int keybind = 82, bool enabled = false) :
        Module("Scaffold", "Player", "Scaffold like a nigger.", keybind, enabled)
    {
        addBool("Keep Y", "Kepps your y level when placing blocks", &yLock);
        addBool("Block Left", "Show left block", &blockleft);
        addEnum("Rotations", "The tower mode", { "Normal", "Down" }, &rotationMode);
        addEnum("Tower", "The tower mode", { "None", "Motion", "Timer" }, &towerMode);
        addSlider("Extend", "Distance your position to target block", &extend, 0, 10);
        addSlider("Tower Timer", "The timer tower speed", &towertimer, 20, 60);
        addBool("Telly", "Only builds when the Y level is the same (Keep Y must be enabled)", &telly);
        addSlider("gapTick", "the delay between placing with ticks.", &gapTickFloat, 1, 5);
    }

    float distance = 5;

    float targetY = 0.f;
    bool hasTowered = false;
    Vector3<float> targetBlock = {};

    int towerMode = 0;
    int rotationMode = 0;
    float towertimer = 17;
    bool yLock = true;
    bool telly = false;
    bool blockleft = true;

    float extend = 2;
    Vector3<float> calcPos = Vector3 <float>{ 0, 0, 0 };

    // aeolus shit
    float gapTickFloat = 1;////
    int gapTick = 1;
    int ticks = 0;
    bool TickGap = false;

    int HotbarSlotPrev = -1;

    bool canPlace(Vector3<float> pos) {
        return Global::getClientInstance()->getBlockSource()->getBlock(pos.floor().ToInt())->GetMaterialType() == MaterialType::Air;
        //return true;
    }

    void onEnabled() override {
        if (!Global::getClientInstance()->getLocalPlayer()) {
            return;
        }
        HotbarSlotPrev = Global::getClientInstance()->getLocalPlayer()->getSupplies()->hotbarSlot; // This for us to set our slot to the current one
    }

    bool findBlock() {
        PlayerInventory* playerInventory = Global::getClientInstance()->getLocalPlayer()->getSupplies();
        Inventory* inventory = playerInventory->inventory;
        auto previousSlot = playerInventory->hotbarSlot;

        for (int i = 0; i < 9; i++) {
            ItemStack* stack = inventory->getItem(i);
            if (stack->item != nullptr) {
                if (stack->isBlockType()) {
                    if (previousSlot != i)
                        playerInventory->hotbarSlot = i;
                    return true;
                }
            }
        }

        //return true; // false if we are using checks
        return false; // false if we are using checks
    }

    bool buildBlock(Vector3<float> pos, GameMode* gm) {
        static std::vector<Vector3<float>> neighbours = {
            Vector3<float>(0, -1, 0), Vector3<float>(0, 1, 0),
            Vector3<float>(0, 0, -1), Vector3<float>(0, 0, 1),
            Vector3<float>(-1, 0, 0), Vector3<float>(1, 0, 0),
        };

        for (int face = 0; face < neighbours.size(); face++) {
            Vector3<float> offset = Vector3<float>(pos.floor()).submissive(neighbours[face]);
            if (!canPlace(offset.ToFloat()) && findBlock()) {
                gm->buildBlock(offset.ToInt(), face, false);
                calcPos = offset.add(Vector3<float>(0.78f, 0.78f, 0.78f));

                return true;
            }
        }
        return false;
    }

    bool predictBlock(Vector3<float> pos, GameMode* gm) {
        static std::vector<Vector3<float>> blocks;
        if (blocks.empty()) {
            for (int y = -distance; y <= 0; y++) {
                for (int x = -distance; x <= distance; x++) {
                    for (int z = -distance; z <= distance; z++) {
                        blocks.push_back(Vector3<float>(x, y, z));
                    }
                }
            }
            std::sort(blocks.begin(), blocks.end(), [](Vector3<float> start, Vector3<float> end) {
                return sqrtf((start.x * start.x) + (start.y * start.y) + (start.z * start.z)) < sqrtf((end.x * end.x) + (end.y * end.y) + (end.z * end.z));
                });
        }

        for (const Vector3<float>& offset : blocks) {
            Vector3<float> currentBlock = Vector3<float>(pos.floor()).add(offset);
            if (buildBlock(currentBlock, gm)) {
                return true;
            }
        }
        return false;
    }

    void onEvent(RenderContextEvent* event) override {
        if (!Global::getClientInstance())
            return;

        if (!Global::getClientInstance()->getTimerClass())
            return;

        Global::getClientInstance()->getTimerClass()->setRenderTimerSpeed(0);

        auto player = Global::getClientInstance()->getLocalPlayer();
        auto gm = player->getGamemode();

        if (!player || !gm || !player->getStateVector()) { return; }

        gapTick = gapTickFloat;////

        if (TickGap)
            ticks = 0;
        else
            ticks++;

        if (!TickGap)
        {
            if (ticks % gapTick != 0)
                return;
        }

        Vector3<float> velocity = player->getStateVector()->Velocity.Normalize();

        if (player->isOnGround() || player->getStateVector()->Position.y < targetY + 0.5f || (!MovementUtils::isMoving() && GetAsyncKeyState(VK_SPACE)) || (!yLock && player->getStateVector()->Position.y > targetY)) {
            targetY = player->getStateVector()->Position.floor().y - 1.5f;
        }

        targetBlock = player->getStateVector()->Position;
        targetBlock.x += velocity.x * extend;
        targetBlock.y = targetY;
        targetBlock.z += velocity.z * extend;

        if (canPlace(targetBlock)) {
            if (telly && yLock) {
                if ((player->getStateVector()->Position.floor().y - 1.5f) == targetY) {
                    predictBlock(targetBlock, gm);
                }
            }
            else {
                predictBlock(targetBlock, gm);
            }
        }
    }

    void onEvent(ImGuiRenderEvent* event) override {
        if (blockleft && findBlock()) {
            Player* player = Global::getClientInstance()->getLocalPlayer();
            GuiData* guidata = Global::getClientInstance()->getGuiData();
            float renderx = guidata->mcResolution.x / 2;
            float rendery = (guidata->mcResolution.y / 2) + 110;
            Vector4<float> plusRect = Vector4<float>(renderx, rendery + 6, 55 + renderx, rendery + 16);
            Vector2<float> textPos = Vector2<float>(renderx, rendery - 30.f);

            auto supplies = player->getSupplies();
            auto inv = supplies->inventory;

            int totalCount = 0;

            for (int i = 0; i < 8; i++) {
                ItemStack* stack = inv->getItem(i);
                if (stack->item != nullptr) {
                    if (stack->isBlockType()) {
                        totalCount += (int)(*(uint8_t*)(((uintptr_t)stack) + 0x22));
                    }
                }
            }

            string text = "Blocks Left: " + to_string(totalCount);
            float textLen = ImRenderUtil::getTextWidth(&text, 1.2) / 2;
            textPos.x -= textLen;
            plusRect.x -= textLen;
            Vector4<float> rectpos = Vector4<float>(textPos.x - 5, textPos.y - 5, textPos.x + textLen + 17, textPos.y + 30);

            ImRenderUtil::fillRectangle(rectpos, UIColor(0, 0, 0), 0.6f, 0.f);
            ImRenderUtil::drawText(textPos, &text, UIColor(255, 255, 255), 1.1f, 1.f, true);
        }
    }

    void onDisabled() override {
        if (!Global::getClientInstance() || !Global::getClientInstance()->getLocalPlayer() || !Global::getClientInstance()->getTimerClass())
            return;

        Global::getClientInstance()->getLocalPlayer()->getSupplies()->hotbarSlot = HotbarSlotPrev;

        Global::getClientInstance()->getTimerClass()->setRenderTimerSpeed(20);
    }
};