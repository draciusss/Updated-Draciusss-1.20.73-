#pragma once

class Weather {
public:
	BUILD_ACCESS(float, lightingLevel, 0x44);
	BUILD_ACCESS(float, rainLevel, 0x38);
};