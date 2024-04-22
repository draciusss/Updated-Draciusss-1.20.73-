#pragma once

class ActorCollision {
public:
	void setOnGround(bool onGround) {
		static auto setOnGround = ("48 83 EC 28 84 D2 74 09");

		Utils::CallFunc<void*, ActorCollision*, bool>((void*)setOnGround, this, onGround);
	}
};