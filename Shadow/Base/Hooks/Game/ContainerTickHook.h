#pragma once

void* onContainerTick;

void* ContainerTickDetour(ContainerScreenController* _this) {

    ContainerTickEvent event{ _this }; // ContainerTickEvent // ChestScreenController::tick
    event.cancelled = nullptr;
    CallBackEvent(&event); // Call ContainerTick event for modules to be writen on this hook.

    return Utils::CallFunc<void*, ContainerScreenController*>(onContainerTick, _this);
}

class ContainerTickHook : public FuncHook
{
public:
    bool Initialize() override // Now override the Initialize function so we can actually hook our stuff
    {
        // Using the utils class findSig function to locate the address of the signature

        // ContainerScreenController::tick(ContainerScreenController *this)
        void* containerTickAddr = Utils::findSig("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC 70 0F 29 74 24 ? 48 8B F1 48 8B 89 ? ? ? ? E8 ? ? ? ? 48 8B CE E8 ? ? ? ? 8B E8 48 8B 16 48 8B CE 48 8B 82 ? ? ? ? FF 15 ? ? ? ? 84 C0 74 31 48 8B 8E ? ? ? ? 48 85 C9 74 12 0F B6 81 ? ? ? ? C6 81 ? ? ? ? ? 84 C0 75 09");

        // Give it the address/ptr to the LoopbackPacketSender function.
        // Give it the detour ptr and then the original callback ptr.

        // Usage: (Sig/Address/Offset/VTable, Detour, Original Callback).
        // Don't forget to return its result.

        // Attempt to hook the function at the address stored in LoopbackVTable by replacing it with the address of LoopbackPacketSenderDetour and store the original function's address in onLoopbackPacketSender
        return Utils::HookFunction(containerTickAddr, (void*)&ContainerTickDetour, &onContainerTick);

        return true;
    }

    static ContainerTickHook& Instance()
    {
        static ContainerTickHook instance;
        return instance;
    }
};