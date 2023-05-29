#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"

class RailCamera {
public:
	void Initialize(const ViewProjection& viewProjection);

	void Update();

	ViewProjection GetViewProjection() { return viewProjection_; }

	const WorldTransform* GetWorldTransform() { return &worldTransform_; }

private:
	WorldTransform worldTransform_;

	ViewProjection viewProjection_;
};
