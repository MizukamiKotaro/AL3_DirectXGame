#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <list>
#include "Collider.h"

class Player;
class GameScene;

class Enemy : public Collider
{
public:

	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(const Vector3& position, Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	void Fire();

	void OnCollision() override;

	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gamescene) { gameScene_ = gamescene; }

	Vector3 GetWorldPosition() override;

	float GetRadius() { return radius_; }

	bool IsDead() { return isDead_; }

private:

	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	void ApproachPhaseInitialize();

	void ApproachPhaseUpdate(const float& moveSpeed);

	//void LeavePhaseInitialize();

	void LeavePhaseUpdate(const float& moveSpeed);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	Phase phase_ = Phase::Approach;

	// 発射間隔
	static const int kFireInterval = 60;

	int32_t fireTimer = 0;

	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;

	float radius_ = 0;

	bool isDead_ = false;
};
