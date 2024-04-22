#pragma once

// Declare a void pointer(Empty object) called onSendKey which is the CallBack for keymapDetourHook.
void* onSendKey; // Defined an orignal for Keymap.

void KeymapDetourHook(uint64_t key, bool held) // Checks what key has been pressed and if its currently being held.
{
	// Set the value of Global::keymap[key] equal to held.
	Global::Keymap[key] = held;

	bool cancelled = false;

	KeyboardEvent event{ &key, &held }; // KeymapEvent
	event.cancelled = &cancelled;
	CallBackEvent(&event); // Call Keymap event for modules to be writen on this hook.

	for (Module* mod : modules) { // Set the modules list.
		if (held == true) { // If a key is held. 
			if (mod->getKeybind() == key) { // If the modules key bind is the key that got pressed.
				if (mod->ingameOnly) // Check if the module is clickgui or no
				{
					if (Global::getClientInstance()->getMinecraftGame()->getCanUseKeys()) // If can use keys
					{
						mod->toggle(); // Disable or enable.
					}
				}
				else mod->toggle(); // Disable or enable.
			}
			
			/*if (mod->getKeybind() == key) { // If the modules key bind is the key that got pressed.
				// toggle module.
				mod->toggle(); // Disable or enable.
			}*/
		}
	}

	// Log the string "Key " concatenated with the value of key, a space, the string " ", and the value of held, followed by a newline character.
	// Debug code that you can remove.
	//log(Utils::combine("Key", key, " ", held, "\n").c_str());

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	if (!cancelled) {
		Utils::CallFunc<void*, __int32, bool>( // CallFunc to call the original.
			onSendKey, key, held
		);
	}
}

class KeymapHook : public FuncHook { // a Keymap class that inherits the FuncHook template we made
public:
	//
	bool Initialize() override // Now override the Initialize function so we can actually hook our stuff
	{
		// Using the utils class findSig function to locate the address of the signature
		// Basically we're using the unique machine code (IN HEX) to find that function again (so it "automatically updates")
		// Find the address of the function specified by the signature "48 83 EC ? ? ? C1 4C 8D 05" and store it in a void pointer called keymapAddr
		void* KeymapAddr = Utils::findSig("48 83 EC ? ? ? C1 4C 8D 05"); // I actually forgot how to get this but it doesn't change often so np.

		// Give it the address/ptr to the Keymap function.
		// Give it the detour ptr and then the original callback ptr.

		// Usage: (Sig/Address/Offset, Detour, Original Callback).
		// Don't forget to return its result.
		
		// Attempt to hook the function at the address stored in keymapAddr by replacing it with the address of keymapDetour and store the original function's address in onSendKey
		return Utils::HookFunction(KeymapAddr, (void*)&KeymapDetourHook, &onSendKey);

		return true;
	}

	static KeymapHook& Instance() { // a class setup function called Instance.
		static KeymapHook instance;
		return instance;
	}
};