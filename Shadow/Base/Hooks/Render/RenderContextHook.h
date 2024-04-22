#pragma once

// Includes
#include "../../SDK/Classes/Render/MinecraftUIRenderContext.h"

// Declare a void pointer(Empty object) called onRender which is the CallBack for renderDetourHook.
void* onRender; // Defined an orignal for RenderDetour.

// Declare a void pointer(Empty object) called onDrawImage which is the CallBack for onDrawImageDetour.
__int64* onDrawImage; // Defined an orignal for onDrawImageDetour.
// Declare a void pointer(Empty object) called onDrawNineSlice which is the CallBack for onDrawNineSliceHook.
__int64* onDrawNineSlice; // Defined an orignal for onDrawNineSliceHook.

// Declare a variable called layerCounter and initialize it to 0
int layerCounter = 0; // Defined an orignal for RenderDetour.

#pragma region onDrawImage

// MinecraftUIRenderContext, TexturePtr, ImagePos, ImageDimesnion, UvPos, UvSize
void onDrawImageDetour(MinecraftUIRenderContext* ctx, TexturePtr* path, Vector2<float>& ImagePos, Vector2<float>& a3, Vector2<float>& a4, Vector2<float>& a5) {
    if (strcmp(path->getFilePath().getText(), "textures/ui/title") == 0) {
        //return;
        // Declare a UIColor object called color and initialize it with the values 11, 11, 11, 255
        UIColor color = UIColor{ 11, 11, 11, 255 };

        // Declare a Vector4 object called rectPos and initialize it with the values { 25, 25 }, { 25, 25 }
        Vector4<float> rectPos({ 0, 0 }, { 1900, 1900 });

        // Call the fillRectangle function on ctx, passing in rectPos and color as arguments
        ctx->fillRectangle(rectPos, color, color.a);

        // Hide the minecraft title.
        return;
    }

    /*if (strcmp(path->getFilePath().getText(), "textures/ui/beacon_button_locked") == 0) {
        static float lerpedPos = ImagePos.x;
        lerpedPos = Math::animate(ImagePos.x, lerpedPos, ImRenderUtil::getDeltaTime() * 10.f);
        ImagePos.x = lerpedPos;

        RenderUtils::fillRectangle(Vector4<float>(ImagePos.x + 1, ImagePos.y + 1, ImagePos.x + a3.x - 1, ImagePos.y + a3.y - 1), UIColor(0, 0, 0), 0.5f);
        RenderUtils::fillRectangle(Vector4<float>(ImagePos.x + 1, ImagePos.y + 1, ImagePos.x + a3.x - 1, ImagePos.y + 1.5f), ColorUtils::Rainbow(5, 1, 1, 1), 1.f);
        return;
    }

    if (strcmp(path->getFilePath().getText(), "banners_no_border_dark_hover") == 0) {
        static float lerpedPos = ImagePos.x;
        lerpedPos = Math::animate(ImagePos.x, lerpedPos, ImRenderUtil::getDeltaTime() * 10.f);
        ImagePos.x = lerpedPos;

        RenderUtils::fillRectangle(Vector4<float>(ImagePos.x + 1, ImagePos.y + 1, ImagePos.x + a3.x - 1, ImagePos.y + a3.y - 1), UIColor(0, 0, 0), 0.5f);
        RenderUtils::fillRectangle(Vector4<float>(ImagePos.x + 1, ImagePos.y + 1, ImagePos.x + a3.x - 1, ImagePos.y + 1.5f), ColorUtils::Rainbow(5, 1, 1, 1), 1.f);
        return;
    }*/

    if (strcmp(path->getFilePath().getText(), "textures/ui/selected_hotbar_slot") == 0) {
        static float lerpedPos = ImagePos.x;
        lerpedPos = Math::animate(ImagePos.x, lerpedPos, ImRenderUtil::getDeltaTime() * 10.f);
        ImagePos.x = lerpedPos;

        RenderUtils::fillRectangle(Vector4<float>(ImagePos.x + 1, ImagePos.y + 1, ImagePos.x + a3.x - 1, ImagePos.y + a3.y - 1), UIColor(0, 0, 0), 0.5f);
        RenderUtils::fillRectangle(Vector4<float>(ImagePos.x + 1, ImagePos.y + 1, ImagePos.x + a3.x - 1, ImagePos.y + 1.5f), ColorUtils::Rainbow(5, 1, 1, 1), 1.f);
        return;
    }

    const char* prefix = "textures/ui/hotbar_";
    int prefixLen = strlen(prefix);
    if (strncmp(path->getFilePath().getText(), prefix, prefixLen) == 0) {
        RenderUtils::fillRectangle(Vector4<float>(ImagePos.x, ImagePos.y, ImagePos.x + a3.x, ImagePos.y + a3.y), UIColor(0, 0, 0), 0.5f);
        return;
    }

    // Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
    Utils::CallFunc<void*, MinecraftUIRenderContext*, TexturePtr*, Vector2<float>&, Vector2<float>&, Vector2<float>&, Vector2<float>&>(
        onDrawImage, ctx, path, ImagePos, a3, a4, a5);
}

#pragma endregion
#pragma region onDrawNineSlice

// MinecraftUIRenderContext, TexturePtr, nineSliceInfo
void onDrawNineSliceDetour(MinecraftUIRenderContext* _this, TexturePtr* texturePtr, NinesliceInfo* nineSliceInfo) {
    float* x = reinterpret_cast<float*>((__int64)nineSliceInfo);
    float* y = reinterpret_cast<float*>((__int64)nineSliceInfo + 0x4);
    float* z = reinterpret_cast<float*>((__int64)nineSliceInfo + 0x60); // funny cuh offset
    float* w = reinterpret_cast<float*>((__int64)nineSliceInfo + 0x64);
    Vector4<float> position(*x, *y, *z, *w);

    if (strcmp(texturePtr->getFilePath().getText(), "textures/ui/hotbar_start_cap") == 0 || (strcmp(texturePtr->getFilePath().getText(), "textures/ui/hotbar_end_cap") == 0)) {
        return;
    }

    //path->getFilePath().getText()
    if (strcmp(texturePtr->getFilePath().getText(), "banners_no_border_dark_hover") == 0 || strcmp(texturePtr->getFilePath().getText(), "textures/ui/beacon_button_locked") == 0) {
        static float lerpedPos = *x;
        lerpedPos = Math::animate(*x, lerpedPos, ImRenderUtil::getDeltaTime() * 10.f);
        *x = lerpedPos;

        RenderUtils::fillRectangle(Vector4<float>(*x + 1, *y + 1, *x + *z - 1, *y + *w - 1), UIColor(0, 0, 0), 0.5f);
        RenderUtils::fillRectangle(Vector4<float>(*x + 1, *y + 1, *x + *z - 1, *y + 1.5f), ColorUtils::Rainbow(5, 1, 1, 1), 1.f);
        return;
    }

    // Inside our functiion we're calling the original code that was there/the original function we hooked so the games behavior doesn't change.
    Utils::CallFunc<void*, MinecraftUIRenderContext*, TexturePtr*, NinesliceInfo*>(
        onDrawNineSlice, _this, texturePtr, nineSliceInfo);
}

#pragma endregion

void renderDetourHook(void* __this, MinecraftUIRenderContext* ctx) { // ScreenContext, MinecraftUIRenderContext
    // Call the function at the address stored in onRender, passing in __this and ctx as arguments
    Utils::CallFunc<void*, void*, MinecraftUIRenderContext*>(
        onRender, __this, ctx
    );

    // Set ClientInstance so it would work
    Global::setClientInstance(ctx->ClientInstance);

    // Initialize RenderContext for renderutils
    RenderUtils::Initialize(ctx); // set CTX

    //Vector3<float> cPos = Global::getClientInstance()->getLocalPlayer()->getStateVector()->Position;
    //std::string playerPos = std::to_string(cPos.x) + " " + std::to_string(cPos.y) + " " + std::to_string(cPos.z);
    //log(playerPos.c_str());

    //Global::getClientInstance()->getTimerClass()->setRenderTimerSpeed(0);

    // -2 + 1
    //Global::getClientInstance()->getLocalPlayer()->getGamemode()->buildBlock(Vector3<int>(cPos.x, cPos.y - 3, cPos.z ), 0, true);

    RenderContextEvent event{}; // RenderContextEvent
    event.cancelled = nullptr;
    CallBackEvent(&event); // Call RenderContext event for modules to be writen on this hook.

    // MinecraftUIRenderContext VTable
    auto vtable = *(uintptr_t**)ctx;
    // Make a bool for tryDrawImage
    static bool tryHookDrawImage = false;
    static bool tryHookDrawNineSlice = false;

    if (!tryHookDrawImage) {
        // Hook drawImage from MinecraftUIRenderContext's VTable
        tryHookDrawImage = Utils::HookFunction((void*)vtable[7], (void*)&onDrawImageDetour, &onDrawImage);
    }

    if (!tryHookDrawNineSlice) {
        // Hook drawImage from MinecraftUIRenderContext's VTable
        tryHookDrawNineSlice = Utils::HookFunction((void*)vtable[8], (void*)&onDrawNineSliceDetour, &onDrawNineSlice);
    }

    // Increment layerCounter
    layerCounter++;

    // If layerCounter is equal to 3
    if (layerCounter == 3) {
        // Set layerCounter to 0
        layerCounter = 0;

        // Declare a UIColor object called color and initialize it with the values 255, 255, 255, 255
        UIColor color = UIColor{ 255, 255, 255, 50 };

        // Declare a Vector4 object called rectPos and initialize it with the values { 25, 25 }, { 25, 25 }
        Vector4<float> rectPos({ 25, 25 }, { 50, 50 });

        // Call the fillRectangle function on ctx, passing in rectPos and color as arguments
        ctx->fillRectangle(rectPos, color, color.a);

        if (Global::getClientInstance() != nullptr) {
            ctx->fillRectangle(Vector4<float>(100, 100, 50, 50), UIColor(0, 255, 0), 255);
        } 
        if (Global::getClientInstance() == nullptr) {
            ctx->fillRectangle(Vector4<float>(150, 100, 50, 50), UIColor(255, 0, 0), 255);
        }
    }
}

class RenderContextHook : public FuncHook {
public:
    bool Initialize() override
    {
        // 2873 is the size of the machine code (Excluding variables) at the time im making this

        // Find the address of the function specified by the signature {SIG} and store it in a void pointer called renderAddr
        // Using the utils class findSig function to locate the address of the signature
        void* renderAddr = Utils::findSig("48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B ? 48 89 54 24 ? 4C");
        // 48 8b c4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8d a8 ? ? ? ? 48 81 ec ? ? ? ? 0f 29 70 ? 0f 29 78 ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 85 ? ? ? ? 4c 8b fa 48 89 54 24 ? 4c 8b e9 48 89 4c 24

        // Give it the address/ptr to the renderDetour function.
        // Give it the detour ptr and then the original callback ptr.

        // Usage: (Sig/Address/Offset, Detour, Original Callback).
        // Don't forget to return its result.
        
        // Attempt to hook the function at the address stored in renderAddr by replacing it with the address of renderDetour and store the original function's address in onRender
        return Utils::HookFunction(renderAddr, (void*)&renderDetourHook, &onRender);

        return true;
    }

    static RenderContextHook& Instance() { // a class setup function called Instance.
        static RenderContextHook instance;
        return instance;
    }
};