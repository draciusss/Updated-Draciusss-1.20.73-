#pragma once

// Declare a void pointer(Empty object) called onItemUseDuration which is the CallBack for ItemUseDurationDetour.
void* onItemUseDuration; // Defined an orignal for ItemUseDuration.
// Declare a void pointer(Empty object) called onCurrentSwingDuration which is the CallBack for getCurrentSwingDurationDetour.
void* onCurrentSwingDuration; // Defined an orignal for CurrentSwingDuration.

int getCurrentSwingDurationDetour(Actor* _this)
{
	return 30;

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	Utils::CallFunc<void*, Actor*>( // CallFunc to call the original.
		onCurrentSwingDuration, _this
	);
}

bool ItemUseDurationDetour(Mob* _this)
{
	return true;

	// Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
	Utils::CallFunc<void*, Mob*>( // CallFunc to call the original.
		onItemUseDuration, _this
	);
}

class AnimationsHook : public FuncHook { // a Animations class that inherits the FuncHook template we made
public:
	//
	bool Initialize() override // Now override the Initialize function so we can actually hook our stuff
	{
		// Mob::getCurrentSwingDuration(void)
		void* getCurrentSwingDurationAddr = Utils::findSig("48 89 5C 24 ? 57 48 83 EC 20 48 8B 15");

		// Player::getItemUseDuration(void)
		void* itemUseDurationAddr = Utils::findSig("8b 81 ? ? ? ? c3 cc cc cc cc cc cc cc cc cc 48 89 6c 24");

		// Attempt to hook the function at the address stored in itemUseDurationAddr by replacing it with the address of ItemUseDurationDetour and store the original function's address in onItemUseDuration
		if (!Utils::HookFunction(getCurrentSwingDurationAddr, (void*)&getCurrentSwingDurationDetour, &onCurrentSwingDuration)) { return false; }

		// Attempt to hook the function at the address stored in itemUseDurationAddr by replacing it with the address of ItemUseDurationDetour and store the original function's address in onItemUseDuration
		if (!Utils::HookFunction(itemUseDurationAddr, (void*)&ItemUseDurationDetour, &onItemUseDuration)) { return false; }

		return true;
	}

	static AnimationsHook& Instance() { // a class setup function called Instance.
		static AnimationsHook instance;
		return instance;
	}
};