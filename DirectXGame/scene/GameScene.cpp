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
	delete skydome_;
	delete modelSkydome_;
}

void GameScene::CheckAllCollisions() { 
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region
	
	for (EnemyBullet* bullet : enemyBullets) {
		CheakCollisionPair(player_, bullet);
	}
	#pragma endregion

	#pragma region
	for (PlayerBullet* bullet : playerBullets) {
		CheakCollisionPair(enemy_, bullet);
	}
	#pragma endregion

	#pragma region
	for (PlayerBullet* playerBullet : playerBullets) {
		for (EnemyBullet* enemyBullet : enemyBullets) {
			CheakCollisionPair(playerBullet, enemyBullet);
		}
	}
	#pragma endregion
}

void GameScene::CheakCollisionPair(Collider* colliderA, Collider* colliderB) {
	Vector3 posA = colliderA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	float length = Calc::MakeLength(posA, posB);
	if (length <= colliderA->GetRadius() + colliderB->GetRadius()) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
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

	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_);
	

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

	skydome_->Update();
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
	
	skydome_->Draw(viewProjection_);
	
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
