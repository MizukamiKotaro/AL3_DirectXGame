#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	playerTextureHandle_ = TextureManager::Load("nikoniko.png");

	// model_.reset(Model::Create());

	viewProjection_.Initialize();
	viewProjection_.translation_.y = 2.0f;
	viewProjection_.UpdateMatrix();

	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_Arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_Arm", true));
	std::vector<Model*> playerModels = {
	    modelFighterBody_.get(), modelFighterHead_.get(), 
		modelFighterL_arm_.get(),modelFighterR_arm_.get()};

	// 自キャラの生成
	player_ = std::make_unique<Player>();
	// 自キャラの初期化
	//Vector3 translate = {0.0f, 2.0f, 0.0f};
	player_->Initialize(playerModels);

	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());

	player_->SetViewProjection(&followCamera_->GetViewProjection());

	modelEnemyBody_.reset(Model::CreateFromOBJ("EnemyBody", true));
	modelEnemyUpJoint_.reset(Model::CreateFromOBJ("EnemyJoint1", true));
	std::vector<Model*> enemyModels = {modelEnemyBody_.get(), modelEnemyUpJoint_.get()};
	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize(enemyModels);

	// debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_.reset(new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight));

	modelSkydome_.reset(Model::CreateFromOBJ("skydome", true));
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get());

	modelGround_.reset(Model::CreateFromOBJ("ground", true));
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get());

#ifdef _DEBUG
	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&followCamera_->GetViewProjection());
#endif // _DEBUG
}

void GameScene::Update() {
	enemy_->Update();

	player_->Update();

	followCamera_->Update();

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
		//コンスト参照
		const ViewProjection &tmp = debugCamera_->GetViewProjection();
		viewProjection_.matView = tmp.matView;
		viewProjection_.matProjection = tmp.matProjection;
		viewProjection_.TransferMatrix();
	} 
	else { // 追従かめら
		const ViewProjection& tmp = followCamera_->GetViewProjection();
		viewProjection_.matView = tmp.matView;
		viewProjection_.matProjection = tmp.matProjection;
		viewProjection_.TransferMatrix();
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

	ground_->Draw(viewProjection_);

	player_->Draw(viewProjection_);

	enemy_->Draw(viewProjection_);

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
