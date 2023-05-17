#include "calc.h"
#include <math.h>

Vector2 Calc::Normalize(const Vector2& vector) {
	float len = static_cast<float>(sqrt(powf(vector.x, 2) + powf(vector.y, 2)));

	Vector2 result = {};

	if (len != 0) {
		result = {vector.x / len, vector.y / len};
	}

	return result;
}
Vector3 Calc::Normalize(const Vector3& vector) {
	float len = static_cast<float> (sqrt(powf(vector.x, 2) + powf(vector.y, 2) + powf(vector.z, 2)));

	Vector3 result = {};

	if (len != 0) {
		result = {vector.x / len, vector.y / len, vector.z / len};
	}

	return result;
}
