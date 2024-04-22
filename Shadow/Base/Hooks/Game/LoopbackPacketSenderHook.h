#pragma once

// Declare a void pointer(Empty object) called onLoopbackPacketSender which is the CallBack for LoopbackPacketSenderDetour.
void* onLoopbackPacketSender; // Defined an orignal for LoopbackPacketSender.

void LoopbackPacketSenderDetour(LoopbackPacketSender* _this, Packet* packet) {
    bool cancelled = false;

    PacketEvent event{ _this, packet }; // PacketEvent
    event.cancelled = &cancelled;
    CallBackEvent(&event); // Call Packet event for modules to be writen on this hook.

    if (!cancelled) {
        // Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
        Utils::CallFunc<void*, LoopbackPacketSender*, Packet*>( // CallFunc to call the original.
            onLoopbackPacketSender, _this, packet
        );
    }
}

class LoopbackPacketSenderHook : public FuncHook { // a LoopbackPacketSender class that inherits the FuncHook template we made
public:
	bool Initialize() override // Now override the Initialize function so we can actually hook our stuff
	{
		// Using the utils class findSig function to locate the address of the signature

		// LoopbackPacketSender::send(void)
		void* loopbackPacketSenderAddr = Utils::findSig("48 8d 05 ? ? ? ? 48 8b 5c 24 ? 48 89 06 33 c0");
		auto Offset = *(int*)((uintptr_t)loopbackPacketSenderAddr + 3);
		auto LoopbackVTable = reinterpret_cast<uintptr_t**>((uintptr_t)loopbackPacketSenderAddr + Offset + 7);

		// Give it the address/ptr to the LoopbackPacketSender function.
		// Give it the detour ptr and then the original callback ptr.

		// Usage: (Sig/Address/Offset/VTable, Detour, Original Callback).
		// Don't forget to return its result.

		// Attempt to hook the function at the address stored in LoopbackVTable by replacing it with the address of LoopbackPacketSenderDetour and store the original function's address in onLoopbackPacketSender
		return Utils::HookFunction(LoopbackVTable[1], (void*)&LoopbackPacketSenderDetour, &onLoopbackPacketSender);

		return true;
	}

	static LoopbackPacketSenderHook& Instance() { // a class setup function called Instance.
		static LoopbackPacketSenderHook instance;
		return instance;
	}
};