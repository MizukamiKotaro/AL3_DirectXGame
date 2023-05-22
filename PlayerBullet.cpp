#include "PlayerBullet.h"
#include <cassert>
#include "Matrix4x4.h"
#include "ImGuiManager.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velcity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	velocity_ = velcity;
}

void PlayerBullet::OnCollision() { 
	isDead_ = true;
}

void PlayerBullet::Update() {
	worldTransform_.translation_ += velocity_;

	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}