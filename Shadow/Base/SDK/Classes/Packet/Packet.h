#pragma once

class Packet {
public:
	virtual void packetConstructor(void) {};
public:
	virtual int getId(void) { return 0x0; };
	virtual class TextHolder getTypeName(void) { return (class TextHolder)nullptr; };
	virtual void write(class BinaryStream&) {};
	virtual void read(class ReadOnlyBinaryStream&) {};
	virtual void readExtended(class ReadOnlyBinaryStream&) {};
	virtual void disallowBatching(void) {};
public:
	uintptr_t** VTable;
private:
	char pad[0x24];
public:
	template<typename P>
	void setVTable(void* sig) { // sets the VTable/Sig for Packets.
		if (!sig)
			return;

		int offset = *(int*)((uintptr_t)sig + 3);
		this->VTable = (uintptr_t**)((uintptr_t)sig + offset + 7);
	}

	template <class T>
	bool instanceOf() { // checks If the packet is being sent.
		T packet;
		return (packet.VTable == this->VTable);
	}

	// You can use those If we didn't have vftables for Packet.
	/*int getId() { return Utils::CallVFunc<1, int>(this); } // get ID from Packet's VTables.
	std::string getName(void) { return Utils::CallVFunc<2, TextHolder*>(this)->getText(); } // get Name from Packet's VTables.*/
};

class PlayerAuthInputPacket : public Packet
{
public:
	PlayerAuthInputPacket() { init(); };
	PlayerAuthInputPacket(Vector3<float> pos, float pitch, float yaw, float bodyyaw)
	{
		init();
		this->pitch = pitch;
		this->yawUnused = yaw;
		this->pos = pos;
		this->yaw = bodyyaw;
	}

	float pitch;
	float yaw;
	Vector3<float> pos;
	float yawUnused;
	Vector3<float> posDelta;
	Vector2<float> moveVector2;
	Vector2<float> moveVector;
	Vector3<float> lookDirectionVR;

	void init()
	{
		static void* sig = nullptr;
		if (!sig) sig = Utils::findSig("48 8D 05 ? ? ? ? 48 89 45 A0 0F 57 C0 0F 29 45 D0 0F 57 C9 0F 29 4D E0 0F 29 45 F0 0F 29 4D 00 0F 29 45 10"); // 1.20.71
		// 48 8D 05 ? ? ? ? 48 89 45 A0
		// 48 8D 05 ? ? ? ? 48 89 45 A0 0F 57 C0 0F 29 45 D0 0F 57 C9 0F 29 4D E0 0F 29 45 F0 0F 29 4D 00 0F 29 45 10 
		this->setVTable<PlayerAuthInputPacket>(reinterpret_cast<uintptr_t*>(sig));
	}
};