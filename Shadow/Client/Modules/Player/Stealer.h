#pragma once

class Stealer : public Module
{
public:
    Stealer(int keybind = 7, bool enabled = true) :
        Module("Stealer", "Player", "Steal items out of chests", keybind, enabled)
    {
        addSlider("SPS", "How many items are moved a second", &SPS, 1, 60);
    }

    float SPS = 29;

    int item = 0;
    int maxItem = 56;

    bool inChest = false;

    void onEvent(ContainerTickEvent* event) override
    {
        if (TimeUtils::hasTimeElapsed("Stealer", 500, false))
            item = 0;

        ContainerScreenController* controller = event->Controller;

        if (!controller)
            return;

        if (SPS == 60)
        {
            for (int i = 0; i < 56; ++i)
            {
                controller->stealItems(Containers::Container, i);
            }

            return;
        }

        if (TimeUtils::hasTimeElapsed("Stealer", 1000 / SPS, true))
        {
            controller->stealItems(Containers::Container, item);

            if (item == maxItem)
            {
                item = 0;
            }

            item++;
        }
    }
};