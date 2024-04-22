#pragma once

// This is an abstract base class for function hooks to inherit.
class FuncHook {
public:
	// This function initializes the hooks.
	virtual bool Initialize();
};

// Hooks Include
#pragma region Hooks Include

// Actor
#include "Actor/ActorBaseTickHook.h"
#include "Actor/LerpMotionHook.h"

//Game
#include "Game/ContainerTickHook.h"
#include "Game/LoopbackPacketSenderHook.h"

//Input
#include "Input/KeymapHook.h"
#include "Input/MouseHook.h"

//Render
#include "Render/AnimationsHook.h"
//  #include "Render/HurtColorHook.h"
#include "Render/RenderContextHook.h"
#include "../DirectX/DirectXHook.h"

#pragma endregion

// This function initializes all registered function hooks
void InitializeHooks() {
	// This is an array of pointers to function hook objects
	static FuncHook* Hooks[] = { // This is to store out hooks.
		// Now in the FuncHook class here register the hooks.
		&RenderContextHook::Instance(),
		&DirectXHook::Instance(), // This is used to initialize dx11 & dx12 hooks
		&KeymapHook::Instance(),
		&MouseHook::Instance(),
		&ActorBaseTickHook::Instance(),
		&ContainerTickHook::Instance(),
		&LoopbackPacketSenderHook::Instance(),
		&AnimationsHook::Instance(),
	//	&HurtColorHook::Instance(),
		&LerpMotionHook::Instance(),
	};

	for (std::size_t i = 0; i < std::size(Hooks); ++i) { // This will allow us to loop through our hooks.
		if (!Hooks[i]->Initialize()) { // If a hooks didn't initialize.
			// Handle initialization failure (Which for now we'll ignore)
		}
	}
}