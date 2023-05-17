#include "Enemy.h"
#include <cassert>
#include "ImGuiManager.h"
#include "calc.h"
#include "Player.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();

	worldTransform_.translation_ = {5.0f, 0.0f, 25.0f};

	ApproachPhaseInitialize();
}

void Enemy::ApproachPhaseInitialize() { 
	//発射タイマーの初期化
	fireTimer = kFireInterval;
}

void Enemy::ApproachPhaseUpdate(const float& moveSpeed) {
	Vector3 velocity = {0, 0, -moveSpeed};

	worldTransform_.translation_ += velocity;

	/*if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}*/
}

void Enemy::LeavePhaseUpdate(const float& moveSpeed) {
	Vector3 velocity = {-moveSpeed/2, moveSpeed/2, -moveSpeed};

	worldTransform_.translation_ += velocity;

}

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

	bullets_.push_back(newBullet);
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos = worldTransform_.translation_;
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

	ApproachPhaseUpdate(kMoveSpeed);

	fireTimer--;
	if (fireTimer == 0) {
		Fire();
		fireTimer = kFireInterval;
	}

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
