#include "EnemyBullet.h"
#include <cassert>
#include "Matrix4x4.h"
#include "ImGuiManager.h"
#include <cmath>
#include <iostream>

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velcity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("heart.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.scale_ = {0.5f, 0.5f, 3.0f};

	worldTransform_.rotation_.y = std::atan2(velcity.x, velcity.z);

	float length = sqrtf(powf(velcity.x, 2) + powf(velcity.z, 2));

	worldTransform_.rotation_.x = std::atan2(-velcity.y, length);
	

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