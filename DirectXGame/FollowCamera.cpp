#include "FollowCamera.h"
#include "Matrix4x4.h"
#include "ImGuiManager.h"

void FollowCamera::Initialize() {

	// 天球の半径の2倍より少し大きいくらい
	viewProjection_.farZ = 1050.0f;
	// 出てないから今は初期値
	viewProjection_.Initialize();
}

void FollowCamera::Update() {

	// ゲームパッドの取得情報を得る関数
	XINPUT_STATE joyState;

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		const float rotSpeed = 0.01f;

		viewProjection_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * rotSpeed;
	}

	if (target_) {
		Vector3 offset = {0.0f, 2.0f, -30.0f};

		offset = offset * Matrix4x4::MakeRotateXYZMatrix(viewProjection_.rotation_);

		viewProjection_.translation_ = Vector3(target_->translation_) + offset;
	}

	viewProjection_.UpdateMatrix();

}