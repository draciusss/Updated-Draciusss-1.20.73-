#pragma once

class GuiData {
public:
	BUILD_ACCESS(Vector2<float>, mcResolution, 0x0030); //ScreenSize()
	BUILD_ACCESS(Vector2<float>, Resolution, 0x0040); //ScreenSizeScaled()
	BUILD_ACCESS(float, Scale, 0x004C); // getScale()


	BUILD_ACCESS(float, screenResRounded, 0x0038);
	BUILD_ACCESS(float, sliderAmount, 0x004C);
	BUILD_ACCESS(float, scalingMultiplier, 0x0050);

	BUILD_ACCESS(Vector2<short>, MousePos, 0x006A); //MousePos
};