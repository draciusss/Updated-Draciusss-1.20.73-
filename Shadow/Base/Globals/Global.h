#pragma once

// You might use this if you want to group all of your global variables,
// functions, etc. under a common name to make them easier to find and organize
namespace Global {
	static inline std::map<uint64_t, bool> Keymap = {}; // Keymap for key checks.
	Actor* EntityList;

	// Rotations
	static inline float easedHeadYaw = 0.f;
	static inline float easedBodyYaw = 0.f;
	static inline float easedPitch = 0.f;

	// In Class functions, variables, etc. (to avoid "is not a type name" error)
	class Core {
	public:
		static inline ClientInstance* ClientInstance = nullptr; // ClientInstance

		static inline bool ShouldAnimate = false;

	};

	// Change or Set ClientInstance
	static void setClientInstance(ClientInstance* instance) { Core::ClientInstance = instance; }

	// Get ClientInstance's class
	ClientInstance* getClientInstance() { return Core::ClientInstance; }

	// IsAtacking
	static inline bool isAttacking = false;

	namespace RenderInfo {
		static inline HWND Window = nullptr;
		static inline ID3D11ShaderResourceView* Texture = NULL;
	}
}
