#pragma once

class ClickGUIManager
{
public:
    static std::vector<Module*> getModulesInCategory(const std::string& category, const std::vector<Module*>& modules) {
        std::vector<Module*> mods;
        std::copy_if(modules.begin(), modules.end(), std::back_inserter(mods), [category](Module* mod) { return mod->getCategory() == category; });
        return mods;
    }

    static inline bool isBinding = false;

    static float getEaseAnim(EasingUtil ease, int mode) {
        switch (mode) {
        case 0: return ease.easeOutExpo(); break;
        case 1: return getModuleByName("clickgui")->isEnabled() ? ease.easeOutElastic() : ease.easeOutBack(); break;
        }
    }
};
