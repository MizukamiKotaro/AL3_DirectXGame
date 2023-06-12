#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Vector3.h"
#include "Collider.h"

class PlayerBullet : public Collider {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velcity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() { return isDead_; }

	void OnCollision() override;

	Vector3 GetWorldPosition() override;

	float GetRadius() { return radius_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;

	Vector3 velocity_;

	static const int32_t kLifeTime = 60 * 5;

	int32_t deathTimer_ = kLifeTime;

	bool isDead_ = false;

	float radius_ = 0;
};