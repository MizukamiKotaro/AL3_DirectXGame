#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = {0, 5.0f, 25.0f};
}

void Enemy::Update() { 
	
	const float kMoveSoeed = 0.2f;
	Vector3 velocity = {0, 0, -kMoveSoeed};

	worldTransform_.translation_ += velocity;

	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
