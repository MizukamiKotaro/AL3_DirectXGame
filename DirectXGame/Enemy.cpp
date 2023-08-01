#include "Enemy.h"
#include <cassert>

void Enemy::Initialize(const std::vector<Model*>& models) {

	BaseCharacter::Initialize(models);

	worldTransform_.Initialize();
	worldTransform_.translation_.z = 5.0f;

	worldTransformBody_.Initialize();
	worldTransformBody_.translation_.y = 2.0f;
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformUpJoint_.Initialize();
	worldTransformUpJoint_.translation_.y = 3.0f;
	worldTransformUpJoint_.parent_ = &worldTransformBody_;

	InitializeRotGimmick();
}

void Enemy::InitializeRotGimmick() { parameter_ = 0.0f; }

void Enemy::UpdateRotGimmick() {

	const uint16_t cycle = 100;

	const float pi = 3.14f;

	const float step = 2.0f * pi / cycle;

	parameter_ += step;

	parameter_ = std::fmod(parameter_, 2.0f * pi);
	// 浮遊の振幅
	const float amplitude = 0.1f;

	worldTransformUpJoint_.translation_.y += std::sin(parameter_) * amplitude;

	worldTransformUpJoint_.rotation_.y = std::sin(parameter_);
}

void Enemy::Update() {

	BaseCharacter::Update();

	UpdateRotGimmick();

	const float speed = 0.5f;

	const float pi = 3.14f;

	const float angle = pi / 60.0f;

	worldTransform_.rotation_.y += angle;

	worldTransform_.rotation_.y = std::fmod(worldTransform_.rotation_.y, 2.0f * pi);

	Vector3 move = {0.0f, 0.0f, -1.0f};
	
	move *= speed;

	move = move * Matrix4x4::MakeRotateXYZMatrix(worldTransform_.rotation_);

	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformUpJoint_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexUpJoint]->Draw(worldTransformUpJoint_, viewProjection);
}