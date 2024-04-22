#pragma once

struct GLMatrix {
public:
	float matrix[16];
	float matrix_nest[4][4];

	GLMatrix* correct() {
		GLMatrix* newMatPtr = new GLMatrix;

		for (int i = 0; i < 4; i++) {
			newMatPtr->matrix[i * 4 + 0] = matrix[0 + i];
			newMatPtr->matrix[i * 4 + 1] = matrix[4 + i];
			newMatPtr->matrix[i * 4 + 2] = matrix[8 + i];
			newMatPtr->matrix[i * 4 + 3] = matrix[12 + i];
		}
		return newMatPtr;
	};

	__forceinline float transformx(const Vector3<float>& p) const {
		return p.x * matrix[0] + p.y * matrix[4] + p.z * matrix[8] + matrix[12];
	}

	__forceinline float transformy(const Vector3<float>& p) const {
		return p.x * matrix[1] + p.y * matrix[5] + p.z * matrix[9] + matrix[13];
	}

	__forceinline float transformz(const Vector3<float>& p) const {
		return p.x * matrix[2] + p.y * matrix[6] + p.z * matrix[10] + matrix[14];
	}
};