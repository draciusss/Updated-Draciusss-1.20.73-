#pragma once
#include "ClickGUIManager.h"

class ShadowGUI {
public:
    struct CategoryPosition {
        float x = 0.f, y = 0.f;
        bool isDragging = false, isExtended = true, wasExtended = false;
        float yOffset = 0;
        float scrollEase = 0;
        Vector2<float> dragVelocity = Vector2<float>();
        Vector2<float> dragAcceleration = Vector2<float>();
    };

    const float catWidth = 175.f;
    const float catHeight = 25.f;

    const float catGap = 40;
    int lastDragged = -1;
    std::vector<CategoryPosition> catPositions;
    Module* lastMod = nullptr;

    void render(float animation, float inScale, int& scrollDirection, char* searchQuery, char* h) {
        ImRenderUtil::fillRectangle(Vector4<float>(0, 0, ImRenderUtil::getScreenSize().x, ImRenderUtil::getScreenSize().y), UIColor(0, 0, 0), animation * 0.1f);

        bool isEnabled = getModuleByName("clickgui")->isEnabled();
        std::string tooltip = "";

        float textSize = inScale;
        float textHeight = ImRenderUtil::getTextHeight(textSize);

        int screenWidth = (int)ImGui::GetIO().DisplaySize.x;
        int screenHeight = 10;

        float windowWidth = 220.0f;
        float windowHeight = 150.0f;
        float yOffset = 50.0f;
        float windowX = (screenWidth - windowWidth) * 0.5f;
        float windowY = screenHeight;

        std::string searchQueryLower = searchQuery;

        std::transform(searchQueryLower.begin(), searchQueryLower.end(), searchQueryLower.begin(), [](unsigned char c) {
            return std::tolower(c);
            });

        std::string SearchStr = "Search";
        std::string SearchQueryStr = strlen(searchQuery) == 0 ? "Search" : searchQueryLower;

        UIColor SearchQueryColor = strlen(searchQuery) == 0 ? UIColor(108, 108, 108) : UIColor(255, 255, 255);
                                                                                                                                           //150
        Vector4<float> rectangle = Vector4<float>(windowX, windowY, windowX + 220.0f, strlen(searchQuery) == 0 ? windowY + 68.0f : windowY + 68.f).scaleToCenter(inScale);
        Vector4<float> searchRectangle = Vector4<float>(windowX + 8, windowY + 30, windowX + 212, windowY + 60).scaleToCenter(inScale);

        ImRenderUtil::fillRectangle(rectangle, UIColor(0, 0, 0), 0.8, 9);
        float textLen = ImRenderUtil::getTextWidth(&SearchStr, 1.15) / 2;
        ImRenderUtil::drawText(Vector2<float>((rectangle.x + 110) - textLen, windowY + 5), &SearchStr, UIColor(255, 255, 255), 1.15 * inScale, 1, true);
        ImRenderUtil::fillRectangle(searchRectangle, UIColor(36, 36, 36), 0.6, 6);
        ImRenderUtil::drawText(Vector2<float>(windowX + 15, windowY + 33), &SearchQueryStr, SearchQueryColor, 1.15 * inScale, 1, true);

        if (catPositions.empty() && isEnabled)
        {
            float centerX = ImRenderUtil::getScreenSize().x / 2.f;
            float xPos = centerX - (categories.size() * (catWidth + catGap) / 2);
            for (std::string& category : categories)
            {
                CategoryPosition pos;
                pos.x = xPos;
                pos.y = catGap * 2;
                xPos += catWidth + catGap;
                catPositions.push_back(pos);
            }
        }

        if (!catPositions.empty()) {
            for (size_t i = 0; i < categories.size(); i++) {
                // Mod math stuff
                const float modWidth = catWidth;
                const float modHeight = catHeight;
                float moduleY = -catPositions[i].yOffset;

                // Get all the modules and populate our vector
                const auto& modsInCategory = ClickGUIManager::getModulesInCategory(categories[i], modules);

                // Calculate the catRect pos
                Vector4<float> catRect = Vector4<float>(catPositions[i].x, catPositions[i].y, catPositions[i].x + catWidth, catPositions[i].y + catHeight)
                    .scaleToPoint(Vector4<float>(ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2, ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2), inScale);

                /* Calculate the height of the catWindow including the settings */
                float settingsHeight = 0;

                for (const auto& mod : modsInCategory) {
                    if (mod->cAnim < 0.01f)
                        continue;

                    for (const auto& setting : mod->getSettings()) {
                        switch (setting->getType())
                        {
                        case SettingType::BOOL: {
                            settingsHeight = Math::lerp(settingsHeight, settingsHeight + modHeight, mod->cAnim);
                            break;
                        }
                        case SettingType::ENUM: {
                            std::vector<std::string> enumValues = setting->getEnumValues();
                            int numValues = static_cast<int>(enumValues.size());

                            settingsHeight = Math::lerp(settingsHeight, settingsHeight + modHeight, mod->cAnim);
                            if (setting->enumSlide > 0.01)
                            {
                                for (int j = 0; j < numValues; j++)
                                    settingsHeight = Math::lerp(settingsHeight, settingsHeight + modHeight, setting->enumSlide);
                            }
                            break;
                        }
                        case SettingType::SLIDER: {
                            settingsHeight = Math::lerp(settingsHeight, settingsHeight + modHeight, mod->cAnim);
                            break;
                        }
                        }
                    }
                }

                float catWindowHeight = catHeight + modHeight * modsInCategory.size() + settingsHeight;
                Vector4<float> catWindow = Vector4<float>(catPositions[i].x, catPositions[i].y, catPositions[i].x + catWidth, catPositions[i].y + moduleY + catWindowHeight)
                    .scaleToPoint(Vector4<float>(ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2, ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2), inScale);
                UIColor rgb = ColorUtils::RainbowDark(i * 20);

                // Can we scroll?
                if (ImRenderUtil::isMouseOver(catWindow) && catPositions[i].isExtended) {
                    if (scrollDirection > 0) {
                        catPositions[i].scrollEase += scrollDirection * catHeight;
                        if (catPositions[i].scrollEase > catWindowHeight - modHeight * 2)
                            catPositions[i].scrollEase = catWindowHeight - modHeight * 2;
                    }
                    else if (scrollDirection < 0) {
                        catPositions[i].scrollEase += scrollDirection * catHeight;
                        if (catPositions[i].scrollEase < 0)
                            catPositions[i].scrollEase = 0;
                    }
                    scrollDirection = 0;
                }

                // Lerp the category extending
                if (!catPositions[i].isExtended) {
                    catPositions[i].scrollEase = catWindowHeight - catHeight;
                    catPositions[i].wasExtended = false;
                }
                else if (!catPositions[i].wasExtended) {
                    catPositions[i].scrollEase = 0;
                    catPositions[i].wasExtended = true;
                }

                // Lerp the scrolling cuz smooth
                catPositions[i].yOffset = Math::animate(catPositions[i].scrollEase, catPositions[i].yOffset, ImRenderUtil::getDeltaTime() * 10.5);

                for (const auto& mod : modsInCategory) {
                    UIColor rgb = ColorUtils::RainbowDark(moduleY * 2);

                    // If the mod belongs to the category
                    if (mod->getCategory() == categories[i]) {
                        // Calculate the modRect pos
                        Vector4<float> modRect = Vector4<float>(catPositions[i].x, catPositions[i].y + catHeight + moduleY, catPositions[i].x + modWidth, catPositions[i].y + catHeight + moduleY + modHeight)
                            .scaleToPoint(Vector4<float>(ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2, ImRenderUtil::getScreenSize().x / 2, ImRenderUtil::getScreenSize().y / 2), inScale);

                        // Animate the setting animation percentage
                        float targetAnim = mod->showSettings ? 1.f : 0.f;
                        mod->cAnim = Math::animate(targetAnim, mod->cAnim, ImRenderUtil::getDeltaTime() * 10);
                        mod->cAnim = Math::clamp(mod->cAnim, 0.f, 1.f);

                        // Settings
                        if (mod->cAnim > 0.001) {
                            for (const auto& setting : mod->getSettings()) {
                                if (!setting->render) {
                                    continue;
                                }

                                UIColor rgb = ColorUtils::RainbowDark(moduleY * 2);

                                switch (setting->getType()) {
                                case SettingType::BOOL: {
                                    moduleY = Math::lerp(moduleY, moduleY + modHeight, mod->cAnim);

                                    Vector4<float> rect = Vector4<float>(modRect.x, catPositions[i].y + catHeight + moduleY, modRect.z, catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(Vector4<float>(modRect.x, ImRenderUtil::getScreenSize().y / 2, modRect.z, ImRenderUtil::getScreenSize().y / 2), inScale);

                                    if (rect.y > catRect.y + 0.5f) {
                                        std::string setName = setting->getName();
                                        ImRenderUtil::fillRectangle(rect, UIColor(30, 30, 30), animation);

                                        if (ImRenderUtil::isMouseOver(rect) && isEnabled) {
                                            tooltip = setting->getDescription();
                                            if (Utils::leftDown) {
                                                *(bool*)setting->getValue() = !*(bool*)setting->getValue();
                                                Utils::leftDown = false;
                                            }
                                        }

                                        setting->boolScale = Math::animate(*(bool*)setting->getValue() ? 1 : 1, setting->boolScale, ImRenderUtil::getDeltaTime() * 10);

                                        float scaledWidth = rect.getWidth() * setting->boolScale;
                                        float scaledHeight = rect.getHeight() * setting->boolScale;
                                        Vector2<float> center = Vector2<float>(rect.x + rect.getWidth() / 2.f, rect.y + rect.getHeight() / 2.f);
                                        Vector4<float> scaledRect = Vector4<float>(center.x - scaledWidth / 2.f, center.y - scaledHeight / 2.f, center.x + scaledWidth / 2.f, center.y + scaledHeight / 2.f);

                                        Vector4<float> smoothScaledRect = Vector4<float>(scaledRect.z - 22, scaledRect.y + 5, scaledRect.z - 7, scaledRect.w - 5).scaleToCenter(setting->boolScale);

                                        //Vector4<float> BallRect = *(bool*)setting->getValue() ? Vector4<float>(smoothScaledRect.z - 17, smoothScaledRect.y + 1, smoothScaledRect.z - 1, smoothScaledRect.w - 4) : Vector4<float>(smoothScaledRect.x + 17, smoothScaledRect.y + 1, smoothScaledRect.x + , smoothScaledRect.w - 4);
                                        //Vector2<float> BallRectCenter = Vector2<float>(BallRect.x + BallRect.getWidth() / 2.f, BallRect.y + BallRect.getHeight() / 2.f);

                                        ImRenderUtil::fillShadowRectangle(smoothScaledRect, UIColor(0, 0, 0), 1, 100, 0);
                                        ImRenderUtil::fillRectangle(smoothScaledRect, *(bool*)setting->getValue() ? rgb : UIColor(15, 15, 15), 1, 0);
                                        //ImRenderUtil::fillRectangle(BallRect, *(bool*)setting->getValue() ? UIColor(255, 255, 255) : UIColor(170, 170, 170), 1, 20);
                                        //ImRenderUtil::fillCircle(BallRectCenter, 20, UIColor(255, 255, 255), 1, 3);
                                        //ImRenderUtil::fillRectangleCustom(smoothScaledRect, rgb, animation * (setting->boolScale), Vector4<float>(20, 20, 20, 20));
                                        //ImRenderUtil::fillRectangle(scaledRect, rgb, animation * (setting->boolScale));

                                        float cSetRectCentreX = rect.x + ((rect.z - rect.x) - ImRenderUtil::getTextWidth(&setName, textSize)) / 2;
                                        float cSetRectCentreY = rect.y + ((rect.w - rect.y) - textHeight) / 2;
                                        ImRenderUtil::drawText(Vector2(rect.x + 5.f, cSetRectCentreY), &setName, *(bool*)setting->getValue() ? UIColor(255, 255, 255) : UIColor(180, 180, 180), textSize, animation, true);
                                    }
                                    break;
                                }
                                case SettingType::ENUM: {
                                    std::string setName = setting->getName();
                                    std::vector<std::string> enumValues = setting->getEnumValues();
                                    int* iterator = setting->getIterator();
                                    int numValues = static_cast<int>(enumValues.size());

                                    // Increment the yOffset
                                    moduleY = Math::lerp(moduleY, moduleY + modHeight, mod->cAnim);

                                    Vector4<float> rect = Vector4<float>(modRect.x, catPositions[i].y + catHeight + moduleY, modRect.z, catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(Vector4<float>(modRect.x, ImRenderUtil::getScreenSize().y / 2, modRect.z, ImRenderUtil::getScreenSize().y / 2), inScale);

                                    // Animate the setting animation percentage
                                    float targetAnim = setting->enumExtended && mod->showSettings ? 1.f : 0.f;
                                    setting->enumSlide = Math::animate(targetAnim, setting->enumSlide, ImRenderUtil::getDeltaTime() * 10);
                                    setting->enumSlide = Math::clamp(setting->enumSlide, 0.f, 1.f);

                                    if (setting->enumSlide > 0.001)
                                    {
                                        for (int j = 0; j < numValues; j++)
                                        {
                                            std::string enumValue = enumValues[j];

                                            // Calculate current position using easing function
                                            moduleY = Math::lerp(moduleY, moduleY + modHeight, setting->enumSlide);

                                            Vector4<float> rect2 = Vector4<float>(modRect.x, catPositions[i].y + catHeight + moduleY, modRect.z, catPositions[i].y + catHeight + moduleY + modHeight)
                                                .scaleToPoint(Vector4<float>(modRect.x, ImRenderUtil::getScreenSize().y / 2, modRect.z, ImRenderUtil::getScreenSize().y / 2), inScale);

                                            if (rect2.y > catRect.y + 0.5f)
                                            {
                                                float cSetRectCentreY = rect2.y + ((rect2.w - rect2.y) - textHeight) / 2;

                                                ImRenderUtil::fillRectangle(rect2, UIColor(20, 20, 20), animation);

                                                if (*iterator == j)
                                                    ImRenderUtil::fillRectangle(Vector4<float>(rect2.x, rect2.y, rect2.x + 1.5f, rect2.w), rgb, animation);

                                                if (ImRenderUtil::isMouseOver(rect2) && Utils::leftDown && isEnabled) {
                                                    *iterator = j;
                                                    Utils::leftDown = false;
                                                }

                                                ImRenderUtil::drawText(Vector2<float>(rect2.x + 5.f, cSetRectCentreY), &enumValue, UIColor(255, 255, 255), textSize, animation, true);
                                            }
                                        }
                                    }

                                    if (rect.y > catRect.y + 0.5f) {
                                        ImRenderUtil::fillRectangle(rect, UIColor(30, 30, 30), animation);

                                        if (ImRenderUtil::isMouseOver(rect) && isEnabled) {
                                            tooltip = setting->getDescription();
                                            if (Utils::leftDown) {
                                                *iterator = (*iterator + 1) % enumValues.size();
                                            }
                                            else if (Utils::rightDown && mod->showSettings) {
                                                setting->enumExtended = !setting->enumExtended;
                                            }
                                        }

                                        float cSetRectCentreY = rect.y + ((rect.w - rect.y) - textHeight) / 2;

                                        std::string enumValue = enumValues[*iterator];
                                        std::string settingString = setName + ": " + enumValue;

                                        ImRenderUtil::drawText(Vector2<float>(rect.x + 5.f, cSetRectCentreY), &settingString, UIColor(255, 255, 255), textSize, animation, true);
                                    }
                                    if (rect.y > catRect.y - modHeight) {
                                        ImRenderUtil::fillGradientOpaqueRectangle(Vector4<float>(rect.x, rect.w, rect.z, rect.w + 10.f * setting->enumSlide * animation), UIColor(0, 0, 0), UIColor(0, 0, 0), 0.F * animation, 0.55F * animation);
                                    }
                                    break;
                                }
                                case SettingType::SLIDER: {
                                    const float value = *(float*)setting->getValue();
                                    const float min = setting->getMin();
                                    const float max = setting->getMax();

                                    char str[10];
                                    sprintf_s(str, 10, "%.1f", value);
                                    std::string rVal = str;

                                    std::string setName = setting->getName();
                                    std::string valueName = rVal;

                                    moduleY = Math::lerp(moduleY, moduleY + modHeight, mod->cAnim);

                                    Vector4<float> rect = Vector4<float>(modRect.x, catPositions[i].y + catHeight + moduleY, modRect.z, catPositions[i].y + catHeight + moduleY + modHeight)
                                        .scaleToPoint(Vector4<float>(modRect.x, ImRenderUtil::getScreenSize().y / 2, modRect.z, ImRenderUtil::getScreenSize().y / 2), inScale);

                                    if (rect.y > catRect.y + 0.5f)
                                    {
                                        ImRenderUtil::fillRectangle(rect, UIColor(30, 30, 30), animation);

                                        const float sliderPos = (value - min) / (max - min) * (rect.z - rect.x);

                                        setting->sliderEase = Math::animate(sliderPos, setting->sliderEase, ImRenderUtil::getDeltaTime() * 10);
                                        setting->sliderEase = std::clamp(setting->sliderEase, 0.f, rect.getWidth());

                                        if (ImRenderUtil::isMouseOver(rect) && isEnabled) {
                                            tooltip = setting->getDescription();
                                            if (Utils::leftDown) {
                                                setting->isDragging = true;
                                            }
                                        }

                                        if (Utils::leftClick && setting->isDragging && isEnabled) {
                                            const float newValue = std::fmax(std::fmin((ImRenderUtil::getMousePos().x - rect.x) / (rect.z - rect.x) * (max - min) + min, max), min);
                                            *(float*)setting->getValue() = newValue;
                                        }
                                        else {
                                            setting->isDragging = false;
                                        }

                                        Vector4<float> newbarlol = Vector4<float>(rect.x + 5.f, rect.y + 17, rect.x + setting->sliderEase - 5.f, rect.w - 3);

                                        ImRenderUtil::fillRectangle(Vector4<float>(rect.x, rect.y, rect.x + setting->sliderEase, rect.w), rgb, animation);
                                        //ImRenderUtil::fillRectangleCustom(Vector4<float>(newbarlol.z - 6.f, newbarlol.y - 2, newbarlol.z + 6, newbarlol.w + 2), UIColor(255, 255, 255), animation, Vector4<float>(20, 20, 20, 20));
                                        auto ValueLen = ImRenderUtil::getTextWidth(&valueName, textSize);
                                        ImRenderUtil::drawText(Vector2<float>((rect.z - 5.f) - ValueLen, rect.y + 2.5), &valueName, UIColor(170, 170, 170), textSize, animation, true);
                                        ImRenderUtil::drawText(Vector2<float>(rect.x + 5.f, rect.y + 2.5), &setName, UIColor(255, 255, 255), textSize, animation, true);
                                    }
                                    break;
                                }
                                }
                            }
                        }

                        if (modRect.y > catRect.y + 0.5f)
                        {
                            // Draw the rect
                            if (mod->cScale <= 1) ImRenderUtil::fillRectangle(modRect, UIColor(40, 40, 40), animation);

                            std::string modName = mod->getName();

                            // Calculate the centre of the rect
                            Vector2<float> center = Vector2<float>(modRect.x + modRect.getWidth() / 2.f, modRect.y + modRect.getHeight() / 2.f);

                            mod->cScale = Math::animate(mod->isEnabled() ? 1 : 0, mod->cScale, ImRenderUtil::getDeltaTime() * 10);

                            // Calculate scaled size based on cScale
                            float scaledWidth = modRect.getWidth() * mod->cScale;
                            float scaledHeight = modRect.getHeight() * mod->cScale;

                            // Calculate new rectangle based on scaled size and center point
                            Vector4<float> scaledRect = Vector4<float>(center.x - scaledWidth / 2.f, center.y - scaledHeight / 2.f, center.x + scaledWidth / 2.f, center.y + scaledHeight / 2.f);

                            // Interpolate between original rectangle and scaled rectangle
                            if (mod->cScale > 0) {
                                ImRenderUtil::fillRectangle(scaledRect, rgb, animation* mod->cScale);
                                //ImRenderUtil::fillRectangle(Vector4(scaledRect.x, scaledRect.y, scaledRect.z, scaledRect.y + 1), UIColor(0, 0, 0), animation);
                            }

                            float cRectCentreX = modRect.x + ((modRect.z - modRect.x) - ImRenderUtil::getTextWidth(&modName, textSize)) / 2;
                            float cRectCentreY = modRect.y + ((modRect.w - modRect.y) - textHeight) / 2;

                            // cRectCentreX. vRectCentreY
                            // .lerp(Vector2<float>(modRect.x + 5, cRectCentreY), mod->cAnim) // if we want lerp to left on extend
                            Vector2<float> modPosLerped = Vector2<float>(modRect.x + 5, cRectCentreY);

                            // Draw the string
                            if (mod->isEnabled()) {
                                ImRenderUtil::drawText(modPosLerped, &modName, UIColor(255, 255, 255), textSize, animation, true);
                            }
                            else {
                                ImRenderUtil::drawText(modPosLerped, &modName, UIColor(170, 170, 170), textSize, animation, true);
                            }

                            std::string bindName = (mod == lastMod && ClickGUIManager::isBinding) ? "Binding..." : std::to_string(mod->getKeybind());
                            float bindNameLen = ImRenderUtil::getTextWidth(&bindName, textSize);
                            Vector4<float> bindRect = Vector4<float>((modRect.z - 10) - bindNameLen, modRect.y + 2.5, modRect.z - 2.5, modRect.w - 2.5);
                            Vector2<float> bindTextPos = Vector2<float>(bindRect.x + 3.5, cRectCentreY);
                            
                            if (mod->getKeybind() != 7) {
                                ImRenderUtil::fillRectangle(bindRect, UIColor(29, 29, 29), 0.9, 4);
                                ImRenderUtil::drawText(bindTextPos, &bindName, UIColor(255, 255, 255), textSize, animation, true);
                            }
                            else if (mod->getKeybind() == 7 && mod == lastMod && ClickGUIManager::isBinding) {
                                ImRenderUtil::fillRectangle(bindRect, UIColor(29, 29, 29), 0.9, 4);
                                ImRenderUtil::drawText(bindTextPos, &bindName, UIColor(255, 255, 255), textSize, animation, true);
                            }

                            GuiData* guidata = Global::getClientInstance()->getGuiData();
                            float renderx = guidata->mcResolution.x / 2;
                            float rendery = (guidata->mcResolution.y / 2) + 110;

                            Vector2<float> textPos = Vector2<float>(renderx, rendery + 70.f);

                            string text = "Hover a module and click the Middle button to bind";
                            float textLen = ImRenderUtil::getTextWidth(&text, 1.3) / 2;
                            textPos.x -= textLen;

                            ImRenderUtil::drawText(textPos, &text, UIColor(170, 170, 170), 1.3f, 1.f, true);

                            if (ImRenderUtil::isMouseOver(modRect) && catPositions[i].isExtended && isEnabled)
                            {
                                if (ImRenderUtil::isMouseOver(catWindow) && catPositions[i].isExtended) {
                                    tooltip = mod->getDescription();
                                }

                                if (Utils::leftDown)
                                {
                                    mod->toggle();
                                    Utils::leftDown = false;
                                    //particleMgr.addParticles(5, GuiInfo::MousePos.x, GuiInfo::MousePos.y, 80, 2.f);
                                }
                                else if (Utils::rightClick)
                                {
                                    mod->showSettings = !mod->showSettings;
                                    Utils::rightClick = false;
                                }
                                else if (Utils::middleClick) {
                                    lastMod = mod;
                                    ClickGUIManager::isBinding = true;
                                    Utils::middleClick = false;
                                }
                            }
                        }
                        if (modRect.y > catRect.y - modHeight) {
                            // Render a slight glow effect
                            ImRenderUtil::fillGradientOpaqueRectangle(Vector4<float>(modRect.x, modRect.w, modRect.z, modRect.w + 10.f * mod->cAnim * animation), UIColor(0, 0, 0), UIColor(0, 0, 0), 0.F * animation, 0.55F * animation);
                        }
                        moduleY += modHeight;
                    }
                }

                if (ClickGUIManager::isBinding) {
                    tooltip = Utils::combine("Currently binding ", lastMod->getName(), "...", " Press ESC to unbind.");
                    for (const auto& key : Global::Keymap) {
                        if (key.second && lastMod) {
                            lastMod->setKeybind(key.first == Keys::ESC ? 7 : key.first);
                            ClickGUIManager::isBinding = false;
                        }
                    }
                }

                std::string catName = categories[i];

                if (ImRenderUtil::isMouseOver(catRect) && Utils::rightDown)
                    catPositions[i].isExtended = !catPositions[i].isExtended;

                ImRenderUtil::fillRectangle(catRect, UIColor(20, 20, 20), animation);

                // Calculate the centre of the rect
                float cRectCentreX = catRect.x + ((catRect.z - catRect.x) - ImRenderUtil::getTextWidth(&catName, textSize * 1.15)) / 2;
                float cRectCentreY = catRect.y + ((catRect.w - catRect.y) - textHeight) / 2;

                // Draw the string
                ImRenderUtil::drawText(Vector2(cRectCentreX, cRectCentreY), &catName, UIColor(255, 255, 255), textSize * 1.15, animation, true);

                catPositions[i].x = std::clamp(catPositions[i].x, 0.f, ImRenderUtil::getScreenSize().x - catWidth);
                catPositions[i].y = std::clamp(catPositions[i].y, 0.f, ImRenderUtil::getScreenSize().y - catHeight);

#pragma region DraggingLogic
                static bool dragging = false;
                static Vector2<float> dragOffset;
                if (catPositions[i].isDragging)
                {
                    if (Utils::leftClick)
                    {
                        if (!dragging)
                        {
                            dragOffset = Vector2<float>(ImRenderUtil::getMousePos().x - catRect.x, ImRenderUtil::getMousePos().y - catRect.y);
                            dragging = true;
                        }
                        Vector2<float> newPosition = Vector2<float>(ImRenderUtil::getMousePos().x - dragOffset.x, ImRenderUtil::getMousePos().y - dragOffset.y);
                        newPosition.x = std::clamp(newPosition.toFloat().x, 0.f, ImRenderUtil::getScreenSize().x - catWidth);
                        newPosition.y = std::clamp(newPosition.toFloat().y, 0.f, ImRenderUtil::getScreenSize().y - catHeight);
                        catPositions[i].x = newPosition.x;
                        catPositions[i].y = newPosition.y;
                    }
                    else
                    {
                        catPositions[i].isDragging = false;
                        dragging = false;
                    }
                }
                else if (ImRenderUtil::isMouseOver(catRect) && Utils::leftDown && isEnabled)
                {
                    Utils::leftDown = false;
                    catPositions[i].isDragging = true;
                    dragOffset = Vector2<float>(ImRenderUtil::getMousePos().x - catRect.x, ImRenderUtil::getMousePos().y - catRect.y);
                }
#pragma endregion
            }

            if (!tooltip.empty()) {
                float textWidth = ImRenderUtil::getTextWidth(&tooltip, textSize * 0.8f);
                float textHeight = ImRenderUtil::getTextHeight(textSize * 0.8f);
                float padding = 2.5f;
                float offset = 8.f;

                Vector4<float> tooltipRect = Vector4<float>(
                    ImRenderUtil::getMousePos().x + offset - padding,
                    ImRenderUtil::getMousePos().y + textHeight / 2 - textHeight - padding,
                    ImRenderUtil::getMousePos().x + offset + textWidth + padding * 2,
                    ImRenderUtil::getMousePos().y + textHeight / 2 + padding
                ).scaleToPoint(Vector4<float>(
                    ImRenderUtil::getScreenSize().x / 2,
                    ImRenderUtil::getScreenSize().y / 2,
                    ImRenderUtil::getScreenSize().x / 2,
                    ImRenderUtil::getScreenSize().y / 2
                ), inScale);

                ImRenderUtil::fillRectangle(tooltipRect, UIColor(20, 20, 20), animation, 20.f);
                ImRenderUtil::drawText(Vector2(tooltipRect.x + padding, tooltipRect.y + padding), &tooltip, UIColor(255, 255, 255), textSize * 0.8f, animation, true);
            }

            if (isEnabled) {
                Utils::leftDown = false;
                Utils::rightDown = false;
                Utils::rightClick = false;
                Global::getClientInstance()->grabMouse();
                scrollDirection = 0;
            }
        }
    }
};
