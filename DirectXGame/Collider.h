#pragma once
#include "Vector3.h"

class Collider {
public:
	
	virtual void OnCollision();

	virtual Vector3 GetWorldPosition() = 0;

	float GetRadius() { return radius_; }

	float SetRadius(float radius) { radius_ = radius; }

private:
	//衝突半径
	//デフォルト値
	float radius_ = 1.0f;
};
