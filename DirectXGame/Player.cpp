#include "Player.h"
#include <cassert>

void Player::Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm) {
	assert(modelBody);
	assert(modelHead);
	assert(modelL_arm);
	assert(modelR_arm);

	modelBody_ = modelBody;
	modelHead_ = modelHead;
	modelL_arm_ = modelL_arm;
	modelR_arm_ = modelR_arm;

	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformBody_.translation_.y = 6.0f;
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_.y = 3.0f;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = {1.0f, 2.5f, 0.0f};
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = {-1.0f, 2.5f, 0.0f};
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	InitializeFloatingGimmick();

}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {

	const uint16_t cycle = 100;

	const float pi = 3.14f;

	const float step = 2.0f * pi / cycle;

	floatingParameter_ += step;

	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * pi);
	//浮遊の振幅
	const float amplitude = 0.5f;

	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;

	const float angle = pi / 6;

	worldTransformL_arm_.rotation_.z = std::sin(floatingParameter_) * angle;
	worldTransformR_arm_.rotation_.z = std::sin(floatingParameter_) * angle;
}

void Player::Update() {

	UpdateFloatingGimmick();

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

		worldTransformBase_.translation_ += move;

		if (move.x == 0 && move.z == 0) {
			//worldTransform_.rotation_.y = viewProjection_->rotation_.y;
		} else {
			worldTransformBase_.rotation_.y = atan2(move.x, move.z);
		}
	}

	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection) {
	modelBody_->Draw(worldTransformBody_, viewProjection);
	modelHead_->Draw(worldTransformHead_, viewProjection);
	modelL_arm_->Draw(worldTransformL_arm_, viewProjection);
	modelR_arm_->Draw(worldTransformR_arm_, viewProjection);
}