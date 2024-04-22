#pragma once

// TimerClass known as Minecraft class
class TimerClass
{
private:
	char pad_0x0000[0xD8]; //0x0000
public:
	float* mainTimer; //0x00D8 
	float* renderTimer; //0x00D0

	void setMainTimerSpeed(float tps) {
		*this->mainTimer = tps;
	};

	void setRenderTimerSpeed(float tps) { // set Render's TicksPerSecound
		*this->renderTimer = tps;
	}
};