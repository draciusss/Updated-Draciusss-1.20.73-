#pragma once

#pragma region Module imports

// Combat
#include "Combat/Killaura.h"
#include "Combat/SpinAttack.h"

// Misc
#include "Misc/Disabler.h"
#include "Misc/FastUse.h"
#include "Misc/KickDecryptor.h"
#include "Misc/Regen.h"
#include "Misc/HiveTimer.h"
#include "Misc/HiveSpeed.h"
#include "Misc/Nuker.h"
#include "Misc/FatNigga.h"

// Motion
#include "Motion/AirSwim.h"
#include "Motion/FastStop.h"
#include "Motion/Fly.h"
#include "Motion/Speed.h"
#include "Motion/Sprint.h"
#include "Motion/TestFly.h"
#include "Motion/Velocity.h"

// Player
#include "Player/AutoSpellbook.h"
#include "Player/Stealer.h"
#include "Player/ExperiencePoints.h"
#include "Player/InvMove.h"
#include "Player/MidClickPearl.h"
#include "Player/NoRotate.h"
#include "Player/NoSwing.h"
#include "Player/Phase.h"
#include "Player/Scaffold.h"

// Visual
#include "Visual/Ambience.h"
#include "Visual/Animations.h"
#include "Visual/ArrayList.h"
#include "Visual/ClickGUI/ClickGUI.h"
#include "Visual/ESP.h"
#include "Visual/Hurtcolor.h"
#include "Visual/Interface.h"
#include "Visual/LevelInfo.h"
#include "Visual/RedstoneESP.h"
#include "Visual/ServerRotations.h"
#include "Visual/Trails.h"
#include "Visual/Watermark.h"

#pragma endregion

// Sort the categories in the modules
void SortCategories() {
	for (Module* mod : modules) {
		if (std::find(categories.begin(), categories.end(), mod->getCategory()) == categories.end())
			categories.push_back(mod->getCategory());
	}
}

// Initialize and push all modules here.
void InitializeModules() {
	// Combat
	modules.push_back(new Module("AntiBot", "Combat", "Stops you from targetting bots", 7, true)); // NONE
	modules.push_back(new Killaura(Keys::TAB, false)); // C
	modules.push_back(new SpinAttack(Keys::NONE, false)); // P

	// Misc
	modules.push_back(new Disabler(Keys::Z, true)); // NONE
	modules.push_back(new FastUse(Keys::NONE, false)); // NONE
	modules.push_back(new KickDecryptor(Keys::NONE, true)); // NONE
	modules.push_back(new Regen(Keys::X, false)); // NONE
	modules.push_back(new HiveTimer(Keys::NONE, false)); // V
	modules.push_back(new HiveSpeed(Keys::NONE, false)); // V
	modules.push_back(new Module("Test Module", "Misc", "Testing", 7, false)); // NONE
	modules.push_back(new Nuker(Keys::NONE, false)); //NONE
	modules.push_back(new Fatnigga(Keys::NONE, false)); //NONE

	// Movement
	modules.push_back(new AirSwim(Keys::NONE, false)); // NONE
	modules.push_back(new FastStop(Keys::NONE, false)); // F
	modules.push_back(new Fly(Keys::NONE, false)); // F
	modules.push_back(new Sprint(Keys::NONE, true)); // Z
	modules.push_back(new Speed(Keys::F, true)); // Z
	modules.push_back(new TestFly(Keys::NONE, false)); // Z
	modules.push_back(new Velocity(Keys::Z, true)); // NONE

	// Player
	modules.push_back(new AutoSpellbook(Keys::NONE, false)); // NONE
	modules.push_back(new Stealer(Keys::Z, true)); // NONE
	modules.push_back(new ExperiencePoints(Keys::NONE, false)); // NONE
	modules.push_back(new InvMove(Keys::NONE, true)); // NONE
	modules.push_back(new MidClickPearl(Keys::NONE, false)); // NONE
	modules.push_back(new NoRotate(Keys::NONE, false)); // NONE
	modules.push_back(new NoSwing(Keys::NONE, false)); // NONE
	modules.push_back(new Phase(Keys::NONE, false)); // NONE
	modules.push_back(new Scaffold(Keys::R, false)); // R
	
	// Renders
	modules.push_back(new Ambience(Keys::NONE, false)); // NONE
	modules.push_back(new Animations(Keys::NONE, true)); // NONE
	modules.push_back(new ArrayList(Keys::NONE, true)); // NONE
	modules.push_back(new ClickGUI(Keys::INSERT, false)); // NONE
	modules.push_back(new ESP(Keys::NONE, false)); // NONE M = 7
	modules.push_back(new Hurtcolor(Keys::NONE, false)); // NONE M = 7
	modules.push_back(new Interface(Keys::NONE, true)); // NONE M = 7
	modules.push_back(new LevelInfo(Keys::NONE, false)); // NONE M = 7
	modules.push_back(new RedstoneESP(Keys::NONE, false)); // NONE
	modules.push_back(new ServerRotations(Keys::Z, true)); // NONE
	modules.push_back(new Trails(Keys::NONE, true)); // NONE
	modules.push_back(new Watermark(Keys::NONE, true)); // NONE
}

void InitializeMods() {
	InitializeModules(); // Initialize modules.
	SortCategories(); // create categories
}

// Uninitialize modules.
void UninitializeMods() {
	for (auto mod : modules) {
		if (mod->isEnabled()) // If a module is enabled.
			mod->toggle(); // Disable the module.

		delete mod;
	}
}