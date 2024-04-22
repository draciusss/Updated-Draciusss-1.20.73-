#pragma once
#include "ShadowGUI.h"
#include "RiseGUI.h"

class ClickGUI : public Module
{
public:
    float animation = 0;
    int animationMode = 0; // Ease enum
    int styleMode = 0; // Ease enum
    float animationSpeed = 10.5f; // Ease speed
    int scrollDirection = 0;

    char searchQuery[256];
    char h[2] = { 0 };

    ClickGUI(int keybind = Keys::INSERT, bool enabled = false) : Module("ClickGUI", "Visual", "Display all modules", keybind, enabled)
    {
        addEnum("Style", "The style of ClickGUI", { "Shadow", "Rise"}, &styleMode);
        addEnum("Animation", "The way it zooms in", { "Expo", "Elastic" }, &animationMode);
        addSlider("EaseSpeed", "The speed it zooms in", &animationSpeed, 5, 15);

        ingameOnly = false;
        callWhenDisabled = true;
    }

    void onEvent(ImGuiRenderEvent* e) override {
        static EasingUtil inEase;
        static ShadowGUI shadowGUI;
        static RiseGUI riseGUI;
        this->isEnabled() ? inEase.incrementPercentage(ImRenderUtil::getDeltaTime() * animationSpeed / 10)
            : inEase.decrementPercentage(ImRenderUtil::getDeltaTime() * 2 * animationSpeed / 10);
        float inScale = ClickGUIManager::getEaseAnim(inEase, animationMode);
        if (inEase.isPercentageMax()) inScale = 1;
        animation = Math::lerp(0, 1, inEase.easeOutExpo());

        if (animation < 0.0001f) {
            return;
        }

        if (styleMode == 0) {
            shadowGUI.render(animation, inScale, scrollDirection, searchQuery, h);
        }
        else if (styleMode == 1) {
            riseGUI.render(animation, inScale, scrollDirection);
        }
    }


    void onEnabled() override
    {
        for (auto key : Global::Keymap)
        {
            key.second == true ? Utils::CallFunc<void*, __int32, bool>(onSendKey, key.first, false) : 0;
        }
    }

    void onDisabled() override
    {
        Global::getClientInstance()->releaseMouse();

        searchQuery[0] = '\0';
    }

    void onEvent(MouseEvent* event) override
    {
        if (!enabled) return;
        *event->cancelled = true;
    }

    void onEvent(KeyboardEvent* event) override
    {
        if (!enabled) return;
        __int32 key = *event->Key;
        bool held = *event->Held;

        if (key == VK_ESCAPE && !ClickGUIManager::isBinding && held) {
            this->setEnabled(false);
            Global::getClientInstance()->releaseMouse();
        }

        if (key == VK_BACK && !held) {
            int len = (int)strlen(searchQuery);
            if (len > 0) {
                searchQuery[len - 1] = '\0';
            }
        }
        else if (key >= 65 && key <= 90 && !held) {
            *event->cancelled = true;
            h[0] = (char)key;
            if (h[0] != '\0') {
                strcat_s(searchQuery, h);
            }
        }

        //*event->cancelled = true;
    }

    void onEvent(MouseScrollEvent* event) override
    {
        if (!enabled) return;
        bool direction = event->MouseDirection;

        if (!direction) scrollDirection++; else scrollDirection--;
    }

private:
    // Add any additional properties or methods specific to your module here
};