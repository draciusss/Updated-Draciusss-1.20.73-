#pragma once

template <typename T>
struct Vector4 {
	// union allows for the same memory location to be accessed with different names
	union {
		struct {
			T x, y, z, w;
		};
		T arr[4];
	};


	// constructor that initializes x, y, z, and w to provided values, or 0 if not provided
	Vector4(T x = 0, T y = 0, T z = 0, T w = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	};

	// constructor that initializes the vector based on a position and size (x and y are position, z and w are size)
	Vector4(Vector2<float> position, Vector2<float> size) {
		this->x = position.x;
		this->y = position.x + size.x;
		this->z = position.y;
		this->w = position.y + size.y;
	};

	__forceinline Vector4<T> scale(float amount) {
		return Vector4<T>(this->x + amount, this->y + amount, this->z - amount, this->w - amount);
	}

	__forceinline float getWidth() {
		return abs(this->z - this->x);
	}

	__forceinline float getHeight() {
		return abs(this->w - this->y);
	}

	__forceinline Vector4<T> scaleToCenter(float scaleFactor) {
		float centerX = (this->x + this->z) / 2.0f;
		float centerY = (this->y + this->w) / 2.0f;

		float width = this->getWidth();
		float height = this->getHeight();

		float scaledWidth = width * scaleFactor;
		float scaledHeight = height * scaleFactor;

		float newX = centerX - scaledWidth / 2.0f;
		float newY = centerY - scaledHeight / 2.0f;
		float newZ = newX + scaledWidth;
		float newW = newY + scaledHeight;

		return Vector4<T>(newX, newY, newZ, newW);
	}

	__forceinline Vector4<T> scaleToPoint(const Vector4<T>& point, float amount) {
		return Vector4<T>(point.x + (this->x - point.x) * amount, point.y + (this->y - point.y) * amount,
			point.z + (this->z - point.z) * amount, point.w + (this->w - point.w) * amount);
	}

	// overload == operator to allow for comparison between two Vector4 objects
	bool operator == (Vector4 v) { return v.x == x && v.y == y && v.z == z && v.w == w; };

	// overload != operator to allow for comparison between two Vector4 objects
	bool operator != (Vector4 v) { return v.x != x || v.y != y || v.z != z || v.w != w; };
};