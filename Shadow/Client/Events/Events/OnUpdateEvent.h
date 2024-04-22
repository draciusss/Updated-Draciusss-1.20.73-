#pragma once

class OnUpdateEvent : public Event
{
public:
    OnUpdateEvent(Actor* _actor, Level* _level, GameMode* _gamemode)
    {
        actor = _actor;
        level = _level;
        gamemode = _gamemode;
    }

    EventType getType() const override { return EventType::UpdateTick; }
    Actor* actor;
    Level* level;
    GameMode* gamemode;
};