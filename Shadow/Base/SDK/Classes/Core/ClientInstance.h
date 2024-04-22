#pragma once

#include "GLMatrix.h"
#include "MinecraftGame.h"
#include "TimerClass.h"
#include "GuiData.h"
#include "LevelRender.h"
#include "../Network/RakNetConnector.h"

// Define 
#define LocalPlayerIndex 29 // 1.20.71
#define ReleaseMouseIndex 331 // 1.20.71
#define GrabMouseIndex 332 // 1.20.71

class ClientInstance {
public:
	// Use CallVFunc to call the VTables by there index.

	Player* getLocalPlayer() {
		return (Player*)Utils::CallVFunc<LocalPlayerIndex, Player*>(this); // 1.20.71
	}

	BlockSource* getBlockSource() {
		return Utils::CallVFunc<28, BlockSource*>(this); // 1.20.71
	}

	void releaseMouse() { // grabMouse - 1
		return Utils::CallVFunc<ReleaseMouseIndex, void>(this); // 1.20.71
	}

	void grabMouse() {
		return Utils::CallVFunc<GrabMouseIndex, void>(this); // 1.20.71
	}

	Vector2<float> getFov() { // Check for fov in clientinstance
		Vector2<float> fov;
		fov.x = *(float*)(reinterpret_cast<uintptr_t>(this) + 0x6F0); // 1.20.71
		fov.y = *(float*)(reinterpret_cast<uintptr_t>(this) + 0x704); // 1.20.71
		return fov;
	}

	GLMatrix* getGLMatrix() {
		return (GLMatrix*)((uintptr_t)(this) + 0x330); // 1.20.71
	}

	inline bool WorldToScreen(Vector3<float> pos, Vector2<float>& screen)
	{ // pos = pos 2 w2s, screen = output screen coords
		if (!getGuiData()) {
			return false;
		}
		Vector2<float> displaySize = getGuiData()->getmcResolution();
		class LevelRender* lr = getLevelRender();
		Vector3<float> origin = lr->getOrigin();
		Vector2<float> fov = getFov();

		pos.x -= origin.x;
		pos.y -= origin.y;
		pos.z -= origin.z;

		auto glmatrix = getGLMatrix();
		std::shared_ptr<GLMatrix> matrix = std::shared_ptr<GLMatrix>(glmatrix->correct());

		float x = matrix->transformx(pos);
		float y = matrix->transformy(pos);
		float z = matrix->transformz(pos);

		if (z > 0) return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}
public:
	BUILD_ACCESS(class MinecraftGame*, MinecraftGame, 0xC8); // 1.20.71
	BUILD_ACCESS(class TimerClass*, TimerClass, 0xD0); // 1.20.71
	BUILD_ACCESS(class GuiData*, GuiData, 0x558); // 1.20.71
	BUILD_ACCESS(class LevelRender*, LevelRender, 0xE0);
};