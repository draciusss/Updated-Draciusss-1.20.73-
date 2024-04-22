#pragma once

class Ambience : public Module
{
public:
    Ambience(int keybind = Keys::NONE, bool enabled = false) :
        Module("Ambience", "Visual", "Changes the time and weather (clientside)", keybind, enabled)
    {
        addBool("Rain", "Make the weather raining", &rain);
        addBool("Time", "Enabling this allows you to change the time of the day", &rain);
        addSlider("RainLevel", "Change the level of the rain", &Rainlevel, 0.1, 5);
        addSlider("Time", "Change the time of the day", &Time, 0, 30000);
    }

    bool rain = false;
    bool time = true;
    float Rainlevel = 0.4;
    float Time = 15000;

    void onEvent(RenderContextEvent* event) override
    {
        Player* player = Global::getClientInstance()->getLocalPlayer();
        ClientInstance* instance = Global::getClientInstance();

        // Check if player or valid key is null
        if (!player || !instance->getBlockSource())
            return;

        if (rain) {
            instance->getBlockSource()->getDimension()->getWeather()->rainLevel = Rainlevel;
        }
        else {
            instance->getBlockSource()->getDimension()->getWeather()->rainLevel = 0;
        }
    }
};