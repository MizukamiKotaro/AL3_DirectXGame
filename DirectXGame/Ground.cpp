#include "Ground.h"
#include <cassert>

void Ground::Initialize(Model* model) {
	assert(model);
	worldTransform_.Initialize();
	model_ = model;
	worldTransform_.translation_ = {};
}

void Ground::Update() {}

void Ground::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
