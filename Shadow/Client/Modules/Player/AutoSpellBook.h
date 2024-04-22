#pragma once

class AutoSpellbook : public Module
{
public:
    AutoSpellbook(int keybind = Keys::NONE, bool enabled = false) :
        Module("AutoSpellbook", "Player", "Automatically uses spellbook", keybind, enabled)
    {

    }

    void onEvent(RenderContextEvent* event) {
        // when low on health we don't have this yet
        auto localPlayer = Global::getClientInstance()->getLocalPlayer();

        if (!localPlayer || !Global::getClientInstance()->getMinecraftGame()->getCanUseKeys()) return;

        PlayerInventory* playerInventory = localPlayer->getSupplies();
        Inventory* inventory = playerInventory->inventory;
        auto previousSlot = playerInventory->hotbarSlot;

        for (int n = 0; n < 36; n++) {
            ItemStack* stack = inventory->getItem(n);
            if (stack->item != nullptr) {
                std::string ItemName = stack->getItem()->name;
                if (ItemName.find("enchanted_book") != std::string::npos) {
                    if (previousSlot != n) {
                        playerInventory->hotbarSlot = n;
                    }
                    else {
                        localPlayer->getGamemode()->useItem(*stack);
                        playerInventory->hotbarSlot = previousSlot;
                    }
                }
            }
        }
    }
};