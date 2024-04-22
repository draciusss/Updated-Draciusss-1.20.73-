#pragma once

#include "Settings.h"

class Module
{
public:
    Module(const std::string& name, const std::string& category, const std::string& description, __int32 keybind = 0x7, bool enabled = false) :
        name(name),
        category(category),
        description(description),
        enabled(enabled),
        keybind(keybind),
        visible(true)
    {
        //addBool("IngameOnly", "Can use keybind in menus", &ingameOnly);
        addBool("Visable in Arraylist", "Draw in the arraylist", &visible);
    }

    // Remove the module from arraylist.
    void RemoveFromArraylist() {
        visible = false;
    }

    // Get the name of the module.
    std::string getName() const {
        return name;
    }

    // Get the description of the module.
    std::string getDescription() const {
        return description;
    }

    // Get the category of the module.
    std::string getCategory() const {
        return category;
    }

    bool getCallWhenDisabled() {
        return callWhenDisabled;
    }

    // Get the keybind of the module.
    int getKeybind() {
        return keybind;
    }

    // Get the enabled state of the module.
    bool isEnabled() const {
        return enabled;
    }

    // Get the visible state of the module.
    bool isVisible() const {
        return visible;
    }

    // Get the settings of the module.
    const std::vector<Setting*> getSettings() const {
        return settings;
    }

    // Set the keybind of the module
    void setKeybind(__int32 _Keybind) {
        keybind = _Keybind;
    }

    // Set the enabled state of the module
    void setEnabled(bool _Enabled) {
        enabled = _Enabled;
    }

    // Set the visible state of the module
    void setVisible(bool _Visible) {
        visible = _Visible;
    }

    // Toggle the enabled state of the module (Includes events)
    void toggle() {
        enabled = !enabled;
        enabled ? onEnabled() : onDisabled();
    }

    // Creates a float slider.
    void addSlider(const std::string name, const std::string description, float* value, float min, float max) {
        settings.emplace_back(new Setting(name, description, value, min, max));
    }

    // Creates a bool.
    void addBool(const std::string name, const std::string description, bool* state) {
        settings.emplace_back(new Setting(name, description, state));
    }

    // Creates a enum.
    void addEnum(const std::string name, const std::string description, std::vector<std::string> enums, int* iterator) {
        settings.emplace_back(new Setting(name, description, enums, iterator));
    }

    virtual std::string getModeName() {
        std::ostringstream oss;
        bool firstEnum = false;
        for (auto set : settings)
        {
            if (set->getType() == SettingType::ENUM && set->getEnumValues().size() > 0 && set->getEnumValues()[0].size() > 0)
            {
                if (!firstEnum)
                {
                    oss << " " << set->getEnumValues()[*set->getIterator()];
                    firstEnum = true;
                }
            }
        }
        return oss.str();
    }

    virtual void onEnabled() {};
    virtual void onDisabled() {};

    // frame events
    virtual void onEvent(class ActorBaseTickEvent* event) {}; // On ActorBaseTick event.
    virtual void onEvent(class ContainerTickEvent* event) {}; // On ContainerTick event.
    virtual void onEvent(class ImGuiRenderEvent* event) {}; // On ImGuiRender event.
    virtual void onEvent(class KeyboardEvent* event) {}; // On Keyboard event.
    virtual void onEvent(class MouseEvent* event) {}; // On Mouse event.
    virtual void onEvent(class MouseScrollEvent* event) {}; // On MouseScroll event.
    virtual void onEvent(class PacketEvent* event) {}; // On Packet event.
    virtual void onEvent(class RenderContextEvent* event) {}; // On RenderContext event.

    float animPercentage = 0.f;
    Vector2<float> location = Vector2<float>();
    Vector2<float> size = Vector2<float>();
    Vector2<float> pos = Vector2<float>();
    bool showSettings = false;
    float cAnim = 0;
    float cFade = 0;
    float cScale = 0;

    bool ingameOnly = true;
    bool callWhenDisabled = false;
protected:
    std::vector<Setting*> settings;
    std::string name;
    std::string description;
    std::string category;
    __int32 keybind;
    bool enabled;
    bool visible;
};