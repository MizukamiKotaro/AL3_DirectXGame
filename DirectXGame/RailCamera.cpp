#include "RailCamera.h"
#include "Matrix4x4.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(const ViewProjection& viewProjection) {

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = viewProjection.translation_;
	worldTransform_.rotation_ = viewProjection.rotation_;

	//天球の半径の2倍より少し大きいくらい
	//viewProjection_.farZ = 300.0f;
	//出てないから今は初期値
	viewProjection_.Initialize();
}

void RailCamera::Update() {

	float velocity = 0.1f;
	float rotSpeed = 0.01f;
	worldTransform_.translation_.z -= velocity;
	worldTransform_.rotation_.z += rotSpeed;

	worldTransform_.matWorld_ = Matrix4x4::MakeAffinMatrix(worldTransform_.scale_,worldTransform_.rotation_,worldTransform_.translation_);

	viewProjection_.matView = Matrix4x4::Inverse(worldTransform_.matWorld_);

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("Translation", &worldTransform_.translation_.x, -50.0f, 50.0f);
	ImGui::SliderFloat3("Rotation", &worldTransform_.rotation_.x, -50.0f, 50.0f);
	ImGui::End();
}


