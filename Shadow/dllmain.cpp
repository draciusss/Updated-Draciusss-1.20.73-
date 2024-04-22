// dllmain.cpp : Defines the entry point for the DLL application.
bool isRunning = true; // isRunning bool for ejection/detachment
//Includes
#pragma region Includes & Macros

// C++ Includes
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <utility>
#include <chrono>
#include <unordered_map>
#include <filesystem>
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

// Module List
class Module;
std::vector<Module*> modules = std::vector<Module*>();
std::vector<std::string> categories;

// DirectX
#include <d2d1.h>
#include <d2d1_2.h>
#include <dwrite.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <dxgi1_4.h>
#include <d2d1_3.h>
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

// MinHook
#include "Libs/minhook/MinHook.h"

// ImGui & Kiero
#include "Libs/kiero/kiero.h"
#include "Libs/imgui/imgui.h"
#include "Libs/imgui/impl/imgui_impl_win32.h"
#include "Libs/imgui/impl/imgui_impl_dx11.h"
#include "Libs/imgui/impl/imgui_impl_dx12.h"
#include "Libs/imgui/stb_image.h"

// Maths
#include "Client/Utils/Math/Keys.h"
#include "Client/Utils/Math/Math.h"
#include "Client/Utils/Math/TextFormat.h"
#include "Client/Utils/Math/TextHolder.h"
#include "Client/Utils/Math/UIColor.h"
#include "Client/Utils/Math/Vector2.h"
#include "Client/Utils/Math/Vector3.h"
#include "Client/Utils/Math/Vector4.h"

// Utils
#include "Client/Utils/Utils.h"
#include "Client/Utils/EasingUtil.h"
#include "Client/Utils/TimeUtils.h"
#include "Client/Utils/ColorUtils.h"

// SDK
//#include "Base/SDK/Classes/Core/ClientInstance.h"
#include "Base/SDK/Classes/Packet/LoopbackPacketSender.h"
#include "Base/SDK/Classes/Packet/Packet.h"
#include "Base/SDK/Classes/Block/Block.h"
#include "Base/SDK/Classes/Block/BlockSource.h"
#include "Base/SDK/Classes/Block/BlockActor.h"
#include "Base/SDK/Classes/Container/Item.h"
#include "Base/SDK/Classes/Container/ItemStack.h"
#include "Base/SDK/Classes/Container/Inventory.h"
#include "Base/SDK/Classes/Container/PlayerInventory.h"
#include "Base/SDK/Classes/Container/ContainerScreenController.h"
#include "Base/SDK/Classes/Actor/ActorCollision.h"
#include "Base/SDK/Classes/Actor/Actor.h"
#include "Base/SDK/Classes/Actor/Level.h"
#include "Base/SDK/Classes/GameMode/GameMode.h"
#include "Base/SDK/Classes/Actor/Mob.h"
#include "Base/SDK/Classes/Actor/Player.h"
#include "Base/SDK/Classes/Core/ClientInstance.h"

// Render Utils
#include "Client/Utils/RenderUtils.h" // This must be under SDK to avoid errors

// Globals
#include "Base/Globals/Global.h"

// Movement
#include "Client/Utils/MovementUtils.h"

// ImRender Utils
#include "Client/Utils/ImRenderUtils.h"

// Event
#include "Client/Events/Event.h"

// Module
#include "Client/Modules/Module.h"

Module* getModuleByName(const std::string& name) {
    std::string nameLower(name);
    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

    auto it = std::find_if(modules.begin(), modules.end(), [&](Module* m) {
        std::string moduleNameLower = m->getName();
        std::transform(moduleNameLower.begin(), moduleNameLower.end(), moduleNameLower.begin(), ::tolower);
        moduleNameLower.erase(std::remove(moduleNameLower.begin(), moduleNameLower.end(), ' '), moduleNameLower.end());
        return nameLower == moduleNameLower;
        }
    );
    return it != modules.end() ? *it : nullptr;
}

// Hooks
#include "Base/Hooks/FuncHook.h"
//#include "Base/DirectX/DirectXHook.h"

// ModuleAssistant
#include "Client/Modules/ModuleAssistant.h"

#pragma endregion

void InitializeClient() {
    Utils::InitializeClientFolders(); // This is used to initialize clients folders.
    InitializeHooks(); // This is used to initialize hooks to the proccess.
    InitializeMods(); // This is used to initialize modules & sort categories.
    //DirectXHook::Initialize(); // This is used to initialize dx11 & dx12 hooks
}

DWORD APIENTRY ejectThread(HMODULE lpParam)
{
    while (isRunning) { // When the client is running.
        // When Control and L pressed.
        if ((Global::Keymap[VK_CONTROL] && Global::Keymap['L'])) {
            // set bool isRunning to false. for ejection.
            isRunning = false;  // Uninject
        }
        Sleep(0); // I use this to avoid little decreasing in fps.
    }

    Sleep(50); // Sleep for a little bit.
    kiero::shutdown();
    MH_DisableHook(MH_ALL_HOOKS); // Disable all Hooks.
    MH_RemoveHook(MH_ALL_HOOKS); // Remove all Hooks.
    UninitializeMods(); // Disable all modules.
    FreeLibraryAndExitThread(lpParam, 1); // Uninject the client completly.
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // Prevent the DLL from being unloaded
        DisableThreadLibraryCalls(hModule);

        // Create a new thread to initialize the client on.
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)InitializeClient, hModule, 0, nullptr);

        // Create a new thread for ejection.
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ejectThread, hModule, 0, 0);
    }
    if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        // Here is where to write code for when a dll detach.
        // We have that writen on ejectThread so no worries.
    }
    return TRUE;
}

