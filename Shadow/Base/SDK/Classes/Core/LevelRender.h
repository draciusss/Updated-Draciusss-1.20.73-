#pragma once
class MaterialPtr;
class LevelRendererPlayer
{
public:
	BUILD_ACCESS(Vector3<float>, cameraPos, 0x5FC); // 1.20.71
};

class LevelRender {
public:
	LevelRendererPlayer* getLevelRendererPlayer()
	{
		return *reinterpret_cast<LevelRendererPlayer**>((uintptr_t)(this) + 0x308); // 1.20.71
	};

	Vector3<float> getOrigin() {
		return getLevelRendererPlayer()->cameraPos;
	};
};