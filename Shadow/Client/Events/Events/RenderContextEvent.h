#pragma once

class RenderContextEvent : public Event
{
public:
    RenderContextEvent() {}

    EventType getType() const override { return EventType::RenderContext; }
};