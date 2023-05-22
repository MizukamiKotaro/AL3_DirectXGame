#pragma once
#include "Vector2.h"
#include "Vector3.h"

class Calc {
public:
	// 長さを出す
	static float MakeLength(const float& a, const float& b);
	static float MakeLength(const Vector2& v);
	static float MakeLength(const Vector3& v);
	static float MakeLength(const Vector2& v1, const Vector2& v2);
	static float MakeLength(const Vector3& v1, const Vector3& v2);

	// 正規化
	static Vector2 Normalize(const Vector2& v);
	static Vector3 Normalize(const Vector3& v);

	// 内積
	static float Dot(const Vector2& v1, const Vector2& v2);
	static float Dot(const Vector3& v1, const Vector3& v2);

	// 外積
	static float Outer(const Vector2& v1, const Vector2& v2);

	// クロス積
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
};
