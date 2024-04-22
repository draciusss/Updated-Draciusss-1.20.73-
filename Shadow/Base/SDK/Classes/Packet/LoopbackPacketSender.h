#pragma once

class Packet;
// LoopbackPacketSender VTable
class LoopbackPacketSender { // 1.20.71
public:
	BUILD_ACCESS(class NetworkSystem*, NetworkSystem, 0x20);
public:
	virtual void Destructor(); // ~LoopbackPacketSender() // LoopbackPacketSender Destructor
	virtual void send(Packet* packet);
	virtual void sendToServer(Packet* packet);
	virtual void sendToClient(class UserEntityIdentifierComponent const*, void* Packet);
	virtual void sendToClient(class NetworkIdentifier const&, void* Packet, unsigned char);
	virtual void sendToClients(std::vector<class NetworkIdentifierWithSubId, std::allocator<class NetworkIdentifierWithSubId>> const&, void* Packet);
	virtual void sendBroadcast(void* Packet);
	virtual void sendBroadcast(class NetworkIdentifier const&, unsigned char, void* Packet);
	virtual void flush(class NetworkIdentifier const&);
};

class NetworkSystem {
public:
	BUILD_ACCESS(class RemoteConnectorComposite*, RemoteConnectorComposite, 0x80);
};

class RemoteConnectorComposite {
public:
	BUILD_ACCESS(class RaknetConnector*, RakNetConnector, 0x60);
};