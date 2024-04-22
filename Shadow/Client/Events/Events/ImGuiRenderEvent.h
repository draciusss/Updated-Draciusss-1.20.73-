#pragma once

class ImGuiRenderEvent : public Event
{
public:
    ImGuiRenderEvent() {}

    EventType getType() const override { return EventType::ImGuiRender; }
};
