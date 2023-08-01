#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Ground.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <memory>
#include "FollowCamera.h"
#include "Enemy.h"

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	uint32_t playerTextureHandle_ = 0;

	// Model* model_ = nullptr;
	// std::unique_ptr<Model> model_;

	ViewProjection viewProjection_;

	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;
	std::unique_ptr<Model> modelFighterWeapon_;

	std::unique_ptr<Player> player_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<Model> modelEnemyBody_;
	std::unique_ptr<Model> modelEnemyUpJoint_;

	std::unique_ptr<Enemy> enemy_;

	bool isDebugCameraActive_ = false;

	std::unique_ptr<DebugCamera> debugCamera_;

	std::unique_ptr<Skydome> skydome_;

	std::unique_ptr<Model> modelSkydome_;

	std::unique_ptr<Ground> ground_;

	std::unique_ptr<Model> modelGround_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
