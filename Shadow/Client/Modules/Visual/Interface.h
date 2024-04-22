#pragma once

class Interface : public Module
{
public:
    Interface(int keybind = Keys::NONE, bool enabled = true) :
        Module("Interface", "Visual", "Interface", keybind, enabled)
    {
        addBool("ProductSans", "ProductSans font", &ProductSansFont);
    }

    bool ProductSansFont = true;

    ImGuiIO& io = ImGui::GetIO();
    ImFont* sansFont = io.Fonts->Fonts[0];
    ImFont* mojanglesFont = io.Fonts->Fonts[1];

    void onEvent(ImGuiRenderEvent* event) override {
        if (ProductSansFont) {
            if (io.FontDefault != sansFont) io.FontDefault = sansFont;
        }
        else {
            if (io.FontDefault != mojanglesFont) io.FontDefault = mojanglesFont;
        }
    }

    void onDisabled() override {
        this->toggle();
    }
};