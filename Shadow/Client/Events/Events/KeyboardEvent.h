#pragma once

class KeyboardEvent : public Event
{
public:
    KeyboardEvent(uint64_t* key, bool* held) {
        Key = key;
        Held = held;
    }

    EventType getType() const override { return EventType::Keyboard; }
    uint64_t* Key;
    bool* Held;
};