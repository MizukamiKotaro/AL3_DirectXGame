#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"

class FollowCamera {
public:
	void Initialize();

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	void SetTarget(const WorldTransform* target) { target_ = target; }



private:
	const WorldTransform* target_ = nullptr;

	ViewProjection viewProjection_;

	Input* input_ = nullptr;
};
