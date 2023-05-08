#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = {0, 5.0f, 25.0f};
}

void Enemy::ApproachPhaseUpdate(const float& moveSpeed) {
	Vector3 velocity = {0, 0, -moveSpeed};

	worldTransform_.translation_ += velocity;

	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeavePhaseUpdate(const float& moveSpeed) {
	Vector3 velocity = {-moveSpeed/2, moveSpeed/2, -moveSpeed};

	worldTransform_.translation_ += velocity;

}

void Enemy::Update() { 
	
	const float kMoveSpeed = 0.2f;

	switch (phase_) { 
	case Phase::Approach:
	default:
		ApproachPhaseUpdate(kMoveSpeed);
		break;
	case Phase::Leave:
		LeavePhaseUpdate(kMoveSpeed);
		break;
	}

	float inputFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	ImGui::Begin("Enemy");
	ImGui::InputFloat3("EnemyPos", inputFloat3);
	if (phase_ == Phase::Approach) {
		ImGui::Text("Phase : Approach");
	} else {
		ImGui::Text("Phase : Leave");
	}
	ImGui::End();

	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
