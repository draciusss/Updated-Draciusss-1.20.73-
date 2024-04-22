#pragma once

class MidClickPearl : public Module
{
public:
    MidClickPearl(int keybind = Keys::NONE, bool enabled = false) :
        Module("MidClickPearl", "Player", "Automatically ender pearls when you press the middle click button", keybind, enabled)
    {

    }

    void onEvent(RenderContextEvent* event) {
        auto localPlayer = Global::getClientInstance()->getLocalPlayer();

        if (!localPlayer || !Global::getClientInstance()->getMinecraftGame()->getCanUseKeys()) return;

        if (Utils::middleClick) {
            PlayerInventory* playerInventory = localPlayer->getSupplies();
            Inventory* inventory = playerInventory->inventory;
            auto previousSlot = playerInventory->hotbarSlot;

            for (int n = 0; n < 36; n++) {
                ItemStack* stack = inventory->getItem(n);
                if (stack->item != nullptr) {
                    std::string ItemName = stack->getItem()->name;
                    if (ItemName.find("ender_pearl") != std::string::npos) {
                        if (previousSlot != n) {
                            playerInventory->hotbarSlot = n;
                        }
                        else {
                            localPlayer->getGamemode()->useItem(*stack);
                            playerInventory->hotbarSlot = previousSlot;
                        }
                    }
                    else {
                        // Send a message
                        // gray[whiteShadowgray]gray No pearl found!
                    }
                }
            }
        }
    }
};