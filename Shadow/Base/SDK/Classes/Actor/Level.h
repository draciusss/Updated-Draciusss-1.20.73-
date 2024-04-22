#pragma once

class Level { // Level VTable
public:
	// Level::getRuntimeActorList
	// Use CallVFunc to call the VTables by there index.
	std::vector<Actor*> getRuntimeActorList() { // Using a CallVFunc for getRuntimeActorList because There is no need for the entire VTable class
		std::vector<Actor*> listOut;
		Utils::CallVFunc<289, decltype(&listOut)>(this, &listOut); // 1.20.71
		return listOut;
	}

	Actor* getRuntimeActorList3() {
		return Utils::CallVFunc<289, Actor*>(this); // 1.20.71
	}
};