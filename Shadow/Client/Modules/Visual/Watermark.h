#pragma once

class Watermark : public Module
{
public:
    Watermark(int keybind = Keys::NONE, bool enabled = true) :
        Module("Watermark", "Visual", "Displays the client's watermark", keybind, enabled)
    {
        addEnum("Style", "The style of the watermark", { "Aeolus", "Gradient" }, &Style);
        addSlider("Opacity", "The opacity of the rectangle", &opacity, 0, 1);
        addBool("Shadow", "Render shadows for the rectangle", &shadow);
        addBool("Fill Shadow", "Fill the rectangle with shadow", &shadowfilled);
        addSlider("Shadow strenght", "The strenght of the shadow", &shadowstrenght, 0, 200);
    }

    float opacity = 0.2;
    bool shadow = true;
    bool shadowfilled = true;
    float shadowstrenght = 150;
    int Style = 0;

    void onEvent(ImGuiRenderEvent* event) override {
        if (Style == 0) {
            std::string name = "DRACO ON TOP FUCK YOU TOZIC GATEKEEPING BITCH";
            
            Vector2<float> textPos = Vector2<float>(15.f, 15.f);

            float verlen = ImRenderUtil::getTextWidth(&name, 1.16) + 22;
            float height = ImRenderUtil::getTextHeight(1.16) + 18;

            Vector4<float> boxRect = Vector4<float>(textPos.x - 7.5, textPos.y - 3.5, verlen, height);

            ImRenderUtil::fillRectangle(boxRect, UIColor(0, 0, 0), opacity);
            ImRenderUtil::fillShadowRectangle(boxRect, UIColor(0, 0, 0), 1, shadowstrenght, shadowfilled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground);

            ImRenderUtil::drawText(textPos, &name, UIColor(255, 255, 255), 1.16, 1, true);
        }
    }

    void onDisabled() override {
        this->toggle();
    }
};