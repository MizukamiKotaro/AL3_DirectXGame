#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model) { 
	assert(model);
	worldTransform_.Initialize();
	model_ = model;
	worldTransform_.translation_ = {};
}

void Skydome::Update() {

}

void Skydome::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection);
}
