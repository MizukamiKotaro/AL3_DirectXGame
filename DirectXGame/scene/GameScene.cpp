#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "WinApp.h"
#include "AxisIndicator.h"
#include "calc.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete enemy_;
}

void GameScene::CheckAllCollisions() { 
	Vector3 posA = {};
	Vector3 posB = {};
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region
	posA = player_->GetWorldPosition();
	for (EnemyBullet* bullet : enemyBullets) {
		posB = bullet->GetWorldPosition();

		float length = Calc::MakeLength(posA, posB);

		if (length <= player_->GetRadius() + bullet->GetRadius()) {
			//player_->OnCollision();
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region
	posA = enemy_->GetWorldPosition();
	for (PlayerBullet* bullet : playerBullets) {
		posB = bullet->GetWorldPosition();

		float length = Calc::MakeLength(posA, posB);

		if (length <= enemy_->GetRadius() + bullet->GetRadius()) {
			//enemy_->OnCollision();
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region
	for (PlayerBullet* playerBullet : playerBullets) {
		posA = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets) {
			posB = enemyBullet->GetWorldPosition();

			float length = Calc::MakeLength(posA, posB);

			if (length <= enemyBullet->GetRadius() + playerBullet->GetRadius()) {
				enemyBullet->OnCollision();
				playerBullet->OnCollision();
			}
		}
	}
	#pragma endregion
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	playerTextureHandle_ = TextureManager::Load("nikoniko.png");

	enemyTextureHandle_ = TextureManager::Load("eye.png");

	model_ = Model::Create();

	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, playerTextureHandle_);

	enemy_ = new Enemy();

	enemy_->Initialize(model_, enemyTextureHandle_);

	enemy_->SetPlayer(player_);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_R)) {
		GameScene::~GameScene();
		GameScene::Initialize();
	}
	player_->Update();
	enemy_->Update();
	CheckAllCollisions();
	debugCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_LSHIFT)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		} else {
			isDebugCameraActive_ = true;
		}
	}
#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		ViewProjection tmp = debugCamera_->GetViewProjection();
		viewProjection_.matView = tmp.matView;
		viewProjection_.matProjection = tmp.matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	enemy_->Draw(viewProjection_);
	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
