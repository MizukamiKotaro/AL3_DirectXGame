#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"
#include "Vector3.h"
#include "PlayerBullet.h"
#include <list>
#include "Collider.h"

class GameScene;

class Player : public Collider {
public:
	
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model,uint32_t textureHandle, const Vector3& playerPosition);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const WorldTransform* railCameraTransform);

	// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection& viewProjection);

	void Rotate();

	void Attack(const WorldTransform* railCameraTransform);

	Vector3 GetWorldPosition() override;

	void OnCollision() override;

	//const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

	float GetRadius() { return radius_; }

	void SetParent(const WorldTransform* parent);

	void SetGameScene(GameScene* gamescene) { gameScene_ = gamescene; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;

	GameScene* gameScene_ = nullptr;

	//当たり判定用
	float radius_ = 0;

};
