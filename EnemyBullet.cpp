#include "EnemyBullet.h"
#include <cassert>
#include "Matrix4x4.h"
#include "ImGuiManager.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velcity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("heart.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velcity;
}

void EnemyBullet::Update() {
	worldTransform_.translation_ += velocity_;

	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}