#pragma once
#include <algorithm>

class ArrayList : public Module
{
public:
    ArrayList(int keybind = 7, bool enabled = false) :
        Module("Arraylist", "Visual", "Display list of modules that are enabled.", keybind, enabled)
    {
        addSlider("Size", "The size of the arraylist", &textSize, 5, 25);
        addSlider("Padding", "The padding of the arraylist", &padding, 5, 15);
        addBool("Shadow", "Render shadows for the arraylist's rectagnle", &shadow);
        addBool("Fill Shadow", "Fill the rectangle with shadow", &shadowfilled);
        addSlider("Shadow strenght", "The strenght of the shadow", &shadowstrenght, 0, 200);
    }

    static inline float textSize = 10.5;
    static inline float padding = 9.5;
    bool shadow = true;
    bool shadowfilled = true;
    float shadowstrenght = 50;

    void onEvent(ImGuiRenderEvent* event) override {
        if (!ImGui::GetBackgroundDrawList() && Global::getClientInstance() == nullptr) return;

        // Create a copy of the modules list
        static size_t previousSize;
        static std::vector<Module*> sortedModules;
        static std::vector<Module*> currentModules;

        for (auto mod : modules) {
            // Hide modules that aren't enabled or visible
            if (!mod->isEnabled() && mod->animPercentage <= 0.01 || !mod->isVisible()) continue;
            sortedModules.push_back(mod);
        }

        // Sort the modules if the vector size has changed (this is so scuffed lmao)
        if (sortedModules.size() != previousSize) {
            sortModules(sortedModules);
            currentModules.clear();
            currentModules = sortedModules;
            previousSize = sortedModules.size();
        }

        // Define the textSize and module index
        const float textHeight = ImRenderUtil::getTextHeight((textSize / 10)) * (padding / 10);
        const float padding = 0.5f * (textSize / 10);
        float textPosY = -1 + 10.20;

        float easeAnim = 0;

        std::vector<std::tuple<Vector4<float>, std::string, std::string, Vector2<float>, float, float>> shadowDraw;
        for (Module* mod : currentModules) {
            float targetAnim = mod->isEnabled() ? 1.f : 0.f;
            mod->animPercentage = Math::animate(targetAnim, mod->animPercentage, ImRenderUtil::getDeltaTime() * 15);
            mod->animPercentage = Math::clamp(mod->animPercentage, 0.f, 1.f);

            easeAnim = mod->animPercentage;

            std::string ModuleName = mod->getName();
            std::string ModeName = mod->getModeName();
            float modWidth = ImRenderUtil::getTextWidth(&ModuleName, (textSize / 10));
            float modeWidth = !ModeName.empty() ? ImRenderUtil::getTextWidth(&ModeName, (textSize / 10)) : 0.f;
            float modulePosX = (ImGui::GetIO().DisplaySize.x - modWidth - modeWidth - padding) - 10.20;
            mod->pos.x = Math::lerp(ImGui::GetIO().DisplaySize.x + 5.f, modulePosX, easeAnim);
            mod->pos.y = textPosY;

            Vector4<float> boxRect(mod->pos.x - padding - 2.f, textPosY, ImRenderUtil::getScreenSize().x + 5.5f, textPosY + textHeight);

            ImRenderUtil::fillRectangle(boxRect, UIColor(0, 0, 0), easeAnim * 0.5);

            shadowDraw.push_back(std::make_tuple(boxRect, ModuleName, ModeName, Vector2<float>(mod->pos.x, mod->pos.y), modWidth, easeAnim));

            textPosY = Math::lerp(textPosY, textPosY + textHeight, easeAnim); // Set a space between arraylist modules.
        }

        for (const auto& pos : shadowDraw) {
            Vector4<float> boxPos = std::get<0>(pos);
            std::string ModuleName = std::get<1>(pos);
            std::string ModeName = std::get<2>(pos);
            Vector2<float> ModulePos = std::get<3>(pos);
            float ModuleWidth = std::get<4>(pos);
            float anim = std::get<5>(pos);

            if (shadow) {
                ImRenderUtil::fillShadowRectangle(boxPos, UIColor(0, 0, 0), anim * 0.5, shadowstrenght, shadowfilled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground);
            }

            ImRenderUtil::drawText(ModulePos, &ModuleName, UIColor(255, 255, 255), (textSize / 10), anim, true);
            ImRenderUtil::drawText(Vector2<float>(ModulePos.x + ModuleWidth, ModulePos.y), &ModeName, UIColor(170, 170, 170), (textSize / 10), anim, true);
        }

        sortedModules.clear(); // Clear sortedModules
    }
private:
    void sortModules(std::vector<Module*>& sortedModules) { // Sort the modules by length.
        std::sort(sortedModules.begin(), sortedModules.end(), [](Module* a, Module* b) {
            std::string nameA = a->getName() + a->getModeName(); // First module.
            std::string nameB = b->getName() + b->getModeName(); // Last module.
            float textSizeA = ImRenderUtil::getTextWidth(&nameA, (textSize / 10)); // First module's Length.
            float textSizeB = ImRenderUtil::getTextWidth(&nameB, (textSize / 10)); // Last module's Lenght.
            return textSizeA > textSizeB; // Sort by the most lenght to lower.
        });
    }

};