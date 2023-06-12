#include "EnemyBullet.h"
#include <cassert>
#include "Matrix4x4.h"
#include "ImGuiManager.h"
#include <cmath>
#include <iostream>
#include "calc.h"

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velcity) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("heart.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::atan2(velcity.x, velcity.z);

	float length = Calc::MakeLength(velcity.x, velcity.z);

	worldTransform_.rotation_.x = std::atan2(-velcity.y, length);
	
	velocity_ = velcity;

	radius_ = 1.0f;
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void EnemyBullet::OnCollision() { 
	isDead_ = true;
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