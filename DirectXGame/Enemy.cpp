#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "calc.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "GameScene.h"

Enemy::~Enemy() {
	
}

void Enemy::Initialize(const Vector3& position, Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	ApproachPhaseInitialize();

	radius_ = 1.0f;
}

void Enemy::OnCollision() { isDead_ = true; }

void Enemy::ApproachPhaseInitialize() { 
	//発射タイマーの初期化
	fireTimer = kFireInterval;
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

void (Enemy::*Enemy::spPhaseUpdateFuncTable[])(const float& moveSpeed){
	&Enemy::ApproachPhaseUpdate,
	&Enemy::LeavePhaseUpdate
};

void Enemy::Fire() {
	assert(player_);

	const float kBulletSpeed = 1.0f;

	Vector3 playerPosition = player_->GetWorldPosition();
	Vector3 enemyPosition = GetWorldPosition();
	Vector3 direction = playerPosition - enemyPosition;
	direction = Calc::Normalize(direction);
	Vector3 velocity = direction * kBulletSpeed;

	velocity =
	    Matrix4x4::Transform(velocity, Matrix4x4::MakeRotateXYZMatrix(worldTransform_.rotation_));

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	gameScene_->AddEnemyBullet(newBullet);
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos = {};
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Enemy::Update() { 
	
	const float kMoveSpeed = 0.1f;

	/*switch (phase_) { 
	case Phase::Approach:
	default:
		ApproachPhaseUpdate(kMoveSpeed);
		break;
	case Phase::Leave:
		LeavePhaseUpdate(kMoveSpeed);
		break;
	}*/

	//ApproachPhaseUpdate(kMoveSpeed);

	(this->*spPhaseUpdateFuncTable[static_cast<size_t>(phase_)])(kMoveSpeed);

	fireTimer--;
	if (fireTimer == 0) {
		Fire();
		fireTimer = kFireInterval;
	}

	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
