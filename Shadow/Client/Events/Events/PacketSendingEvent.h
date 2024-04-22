#pragma once

class PacketSendingEvent : public Event
{
public:
    PacketSendingEvent(Actor* _actor, Level* _level, GameMode* _gamemode)
    {
        actor = _actor;
        level = _level;
        gamemode = _gamemode;
    }

    EventType getType() const override { return EventType::PacketSending; }
    Actor* actor;
    Level* level;
    GameMode* gamemode;
};