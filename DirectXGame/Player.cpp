#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void Player::Initialize(Model* model, const Vector3& translate) {
	assert(model);
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = translate;

	// シングルインスタンスを取得
	input_ = Input::GetInstance();
}

void Player::Update() {

	// ゲームパッドの取得情報を得る関数
	XINPUT_STATE joyState;

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		const float speed = 3.0f;

		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f,(float)joyState.Gamepad.sThumbLY / SHRT_MAX
		};
		
		move = move.Normalize() * speed;

		move = move * Matrix4x4::MakeRotateXYZMatrix(viewProjection_->rotation_);

		worldTransform_.translation_ += move;

		if (move.x == 0 && move.z == 0) {
			//worldTransform_.rotation_.y = viewProjection_->rotation_.y;
		} else {
			worldTransform_.rotation_.y = atan2(move.x, move.z);
		}
	}

	worldTransform_.UpdateMatrix();
	
}

void Player::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}