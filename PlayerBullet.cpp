#include "PlayerBullet.h"
#include <cassert>
#include "Matrix4x4.h"
#include "ImGuiManager.h"

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
}

void PlayerBullet::Update() {
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}