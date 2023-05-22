#include "calc.h"
#include <math.h>

float Calc::MakeLength(const float& a, const float& b) {
	float result = static_cast<float>(sqrt(powf(a, 2) + powf(b, 2)));
	return result;
}
float Calc::MakeLength(const Vector2& v) {
	float result = static_cast<float>(sqrt(powf(v.x, 2) + powf(v.y, 2)));
	return result;
}
float Calc::MakeLength(const Vector3& v) {
	float result = static_cast<float>(sqrt(powf(v.x, 2) + powf(v.y, 2) + powf(v.z, 2)));
	return result;
}
float Calc::MakeLength(const Vector2& v1, const Vector2& v2) {
	float result = static_cast<float>(sqrt(powf(v1.x - v2.x, 2) + powf(v1.y - v2.y, 2)));
	return result;
}
float Calc::MakeLength(const Vector3& v1, const Vector3& v2) {
	float result = static_cast<float>(
	    sqrt(powf(v1.x - v2.x, 2) + powf(v1.y - v2.y, 2) + powf(v1.z - v2.z, 2)));
	return result;
}

Vector2 Calc::Normalize(const Vector2& v) {
	float len = MakeLength(v);

	Vector2 result = {};

	if (len != 0) {
		result = {v.x / len, v.y / len};
	}

	return result;
}

Vector3 Calc::Normalize(const Vector3& v) {
	float len = MakeLength(v);

	Vector3 result = {};

	if (len != 0) {
		result = {v.x / len, v.y / len, v.z / len};
	}

	return result;
}

float Calc::Dot(const Vector2& v1, const Vector2& v2) {
	float dot = v1.x * v2.x + v1.y * v2.y;
	return dot;
}

float Calc::Dot(const Vector3& v1, const Vector3& v2) {
	float dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return dot;
}

float Calc::Outer(const Vector2& v1, const Vector2& v2) {
	float outer = v1.x * v2.y - v1.y * v2.x;
	return outer;
}

Vector3 Calc::Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result = {
	    v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
	return result;
}
