#pragma once

class Math
{
public:
	inline static float animate(float endPoint, float current, float speed) { // Animate the position
		if (speed < 0.0) speed = 0.0; // If 0 is less than speed then set speed to 0.
		else if (speed > 1.0) speed = 1.0; // If Speed is faster than 1 then set speed to 1.
		
		float dif = std::fmax(endPoint, current) - std::fmin(endPoint, current); // Animate between max and min.
		float factor = dif * speed;
		return current + (endPoint > current ? factor : -factor); // Animates the distance
	}

	inline static float lerp(float a, float b, float c) { // Lerp the position
		return a + c * (b - a); // Make the lerp smooth
	}

	__forceinline static float wrap(float value, float min, float max) {
		return fmod(fmod(value - min, max - min) + (max - min), max - min) + min;
	}

	static inline float clamp(float value, float min_val, float max_val) { // Clamp the position
		return value < min_val ? min_val : (value > max_val ? max_val : value);
	}
};