#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "PlayerBullet.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <list>
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void AddEnemyBullet(EnemyBullet* enemyBullet);

	void AddPlayerBullet(PlayerBullet* playerBullet);

private:
	void CheckAllCollisions();

	void CheakCollisionPair(Collider* colliderA, Collider* colliderB);

	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();

	void EnemyGeneration(const Vector3& position);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t playerTextureHandle_ = 0;

	uint32_t enemyTextureHandle_ = 0;

	Model* model_ = nullptr;

	ViewProjection viewProjection_;

	Player* player_ = nullptr;
	std::list<PlayerBullet*> playerBullets_;

	bool isDebugCameraActive_ = false;

	DebugCamera* debugCamera_ = nullptr;

	std::list<Enemy*> enemies_;
	std::list<EnemyBullet*> enemyBullets_;
	//敵発生コマンド
	std::stringstream enemyPopCommands;

	bool IsWait_ = false;
	int32_t waitTimer_ = 0;

	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

	RailCamera* railCamera_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
