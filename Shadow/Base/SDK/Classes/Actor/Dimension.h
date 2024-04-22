#pragma once

class Dimension { // is named mcDimension since it conflicts with the Dimension class in Engine.hpp
public:
	BUILD_ACCESS(class Weather*, Weather, 0x1B0);
};