#include "Player.h"
#include <cassert>
#include "Matrix4x4.h"
#include "ImGuiManager.h"
#include "GameScene.h"
#include "calc.h"

Player::~Player() { 
	
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

	worldTransform3DReticle_.Initialize();
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

		Vector3 position = Matrix4x4::Transform(worldTransform_.translation_,railCameraTransform->matWorld_);

		const float kBulletSpeed = 0.5f;
		Vector3 velocity;
		
		velocity = worldTransform3DReticle_.translation_ - worldTransform_.translation_;
		velocity = Calc::Normalize(velocity) * kBulletSpeed;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, position,velocity);

		gameScene_->AddPlayerBullet(newBullet);
	}
}

void Player::OnCollision() {

}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos = {};
	worldPos.x =worldTransform_.matWorld_.m[3][0];
	worldPos.y =worldTransform_.matWorld_.m[3][1];
	worldPos.z =worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::Update(const WorldTransform* railCameraTransform) {
	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへのオフセット（z+向き）
	Vector3 offset = {0, 0, 1.0f};
	//自機のワールド行列の回転を反映
	offset = Matrix4x4::Multiply(offset, worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = Calc::Normalize(offset) * kDistancePlayerTo3DReticle;
	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ =
	    Matrix4x4::Transform(offset, railCameraTransform->matWorld_);

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();

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

	

	Attack(railCameraTransform);

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
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);	

	
}
