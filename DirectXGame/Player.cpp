#include "Player.h"
#include <cassert>
#include "Matrix4x4.h"
#include "ImGuiManager.h"

Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle, const Vector3& playerPosition) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_ = playerPosition;

	//シングルインスタンスを取得
	input_ = Input::GetInstance();

	radius_ = 1.0f;
}

void Player::Rotate() { 
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack(const WorldTransform* railCameraTransform) {
	if (input_->TriggerKey(DIK_SPACE)) {

		Vector3 position = Matrix4x4::Transform(
		    worldTransform_.translation_,
		    Matrix4x4::MakeAffinMatrix(
		        railCameraTransform->scale_, railCameraTransform->rotation_,
		        railCameraTransform->translation_));

		const float kBulletSpeed = 0.5f;
		Vector3 velocity(0.0f, 0.0f, kBulletSpeed);
		velocity = Matrix4x4::Transform(
		    velocity, Matrix4x4::MakeRotateXYZMatrix(railCameraTransform->rotation_));

		velocity = Matrix4x4::Transform(velocity, Matrix4x4::MakeRotateXYZMatrix(worldTransform_.rotation_));

		

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, position,velocity);

		bullets_.push_back(newBullet);
	}
}

void Player::OnCollision() {

}

Vector3 Player::GetWorldPosition() {
	Matrix4x4 worldMatrix = Matrix4x4::MakeAffinMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	Vector3 worldPos = {};
	worldPos.x = worldMatrix.m[3][0];
	worldPos.y = worldMatrix.m[3][1];
	worldPos.z = worldMatrix.m[3][2];
	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::Update(const WorldTransform* railCameraTransform) {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix(); 

	Rotate();

	Vector3 move = {0, 0, 0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	worldTransform_.translation_ += move;

	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 20.0f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.UpdateMatrix();

	bullets_.remove_if([](PlayerBullet* bullet) { 
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Attack(railCameraTransform);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//float inputFloat3[3] = {
	//    worldTransform_.translation_.x, worldTransform_.translation_.y,
	//    worldTransform_.translation_.z};

	//ImGui::Begin("PlayerPosition");
	////ImGui::InputFloat3("PlayerPosition", inputFloat3);
	//ImGui::SliderFloat3("PlayerPosition", inputFloat3, -35.0f, 35.0f);
	//ImGui::End();

	//worldTransform_.translation_.x = inputFloat3[0];
	//worldTransform_.translation_.y = inputFloat3[1];
	//worldTransform_.translation_.z = inputFloat3[2];
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	
}
