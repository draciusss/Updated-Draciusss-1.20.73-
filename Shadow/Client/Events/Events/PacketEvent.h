#pragma once

class PacketEvent : public Event
{
public:
    PacketEvent(LoopbackPacketSender* sender, Packet* packet)
    {
        Sender = sender;
        Packet = packet;
    }

    EventType getType() const override { return EventType::PacketSending; }
    LoopbackPacketSender* Sender;
    Packet* Packet;
};