#pragma once

class ActorDefinitionIdentifier
{
public:
	void* namespacedKey; //0x0020 // NamespacedKey
	std::string identifier; //0x0040
	std::string initEvent; //0x0060
	void* canonicalName; //0x0080 // HashedString
}; //Size: 0x0160

//static_assert(sizeof(ActorDefinitionIdentifier) == 0xB0);