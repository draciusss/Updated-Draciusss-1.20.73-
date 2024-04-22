#pragma once

// Declare a void pointer(Empty object) called onSendMouse which is the CallBack for MouseDetourHook.
void* onSendMouse; // Defined an orignal for Mouse.

void MouseDetourHook(void* _this, __int8 mouseButton, bool isDown, __int16 mouseX, __int16 mouseY, __int16 relX, __int16 relY, int a8)
{
	bool cancelled = false;

	if (mouseButton >= 1 && mouseButton <= 4) {
		MouseEvent event{ &mouseButton, &isDown }; // MouseEvent
		event.cancelled = &cancelled;
		CallBackEvent(&event); // Call Mouse event for modules to be writen on this hook.

		Utils::onMouseClick((int)mouseButton, isDown);

		if (mouseButton == 4) {
			MouseScrollEvent event{ isDown < 136 }; // MouseScrollEvent
			event.cancelled = &cancelled;
			CallBackEvent(&event); // Call MouseScroll event for modules to be writen on this hook.
		}
	}

	if (ImGui::GetCurrentContext() != nullptr) { // This is to hook imgui mouse pos and key stuff
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(mouseX, mouseY);

		switch (mouseButton)
		{
		case 1:
			io.MouseDown[0] = isDown;
			break;
		case 2:
			io.MouseDown[1] = isDown;
			break;
		case 3:
			io.MouseDown[2] = isDown;
			break;
		case 4:
			io.MouseWheel = isDown < 0 ? -0.5f : 0.5f; //For scrolling
			break;
		}

		if (io.WantCaptureMouse)
			return;
	}

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	if (!cancelled) {
		Utils::CallFunc<void*, void*, __int8, bool, __int16, __int16, __int16, __int16, char>( // CallFunc to call the original.
			onSendMouse, _this, mouseButton, isDown, mouseX, mouseY, relX, relY, a8
		);
	}
}

class MouseHook : public FuncHook { // a Mouse class that inherits the FuncHook template we made
public:
	//
	bool Initialize() override // Now override the Initialize function so we can actually hook our stuff
	{
		// Using the utils class findSig function to locate the address of the signature
		// Basically we're using the unique machine code (IN HEX) to find that function again (so it "automatically updates")
		// Find the address of the function specified by the signature "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24" and store it in a void pointer called MouseAddr
		void* MouseAddr = Utils::findSig("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24"); // I actually forgot how to get this but it doesn't change often so np.

		// Give it the address/ptr to the Mouse function.
		// Give it the detour ptr and then the original callback ptr.

		// Usage: (Sig/Address/Offset, Detour, Original Callback).
		// Don't forget to return its result.

		// Attempt to hook the function at the address stored in MouseAddr by replacing it with the address of MouseDetour and store the original function's address in onSendMouse
		return Utils::HookFunction(MouseAddr, (void*)&MouseDetourHook, &onSendMouse);

		return true;
	}

	static MouseHook& Instance() { // a class setup function called Instance.
		static MouseHook instance;
		return instance;
	}
};