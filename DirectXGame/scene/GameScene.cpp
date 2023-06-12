#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "WinApp.h"
#include "AxisIndicator.h"
#include "calc.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	//delete enemy_;
	delete skydome_;
	delete modelSkydome_;
	delete railCamera_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	for (PlayerBullet* bullet : playerBullets_) {
		delete bullet;
	}
}

void GameScene::CheckAllCollisions() { 
	//const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//const std::list<EnemyBullet*>& enemyBullets_ = enemy_->GetBullets();

	#pragma region
	
	for (EnemyBullet* bullet : enemyBullets_) {
		CheakCollisionPair(player_, bullet);
	}
	#pragma endregion

	#pragma region
	for (PlayerBullet* bullet : playerBullets_) {
		for (Enemy* enemy : enemies_) {
			CheakCollisionPair(enemy, bullet);
		}
	}
	#pragma endregion

	#pragma region
	for (PlayerBullet* playerBullet : playerBullets_) {
		for (EnemyBullet* enemyBullet : enemyBullets_) {
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) { enemyBullets_.push_back(enemyBullet); }
void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) { playerBullets_.push_back(playerBullet); }

void GameScene::LoadEnemyPopData() { 
	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {

	//待機処理
	if (IsWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			//待機完了
			IsWait_ = false;
		}
		return;
	}

	//1列分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		//1列分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行は飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生させる
			EnemyGeneration(Vector3(x, y, z));

		} 
		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			IsWait_ = true;
			waitTimer_ = waitTime;

			break;
		}
	}


}

void GameScene::EnemyGeneration(const Vector3& position) {
	Enemy* enemy_ = new Enemy();
	enemy_->Initialize(position, model_, enemyTextureHandle_);
	enemy_->SetPlayer(player_);
	enemy_->SetGameScene(this);
	enemies_.push_back(enemy_);
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	playerTextureHandle_ = TextureManager::Load("nikoniko.png");

	enemyTextureHandle_ = TextureManager::Load("eye.png");

	IsWait_ = false;
	waitTimer_ = 0;
	LoadEnemyPopData();

	model_ = Model::Create();

	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	Vector3 playerPosition(0, 0, 20);
	player_->Initialize(model_, playerTextureHandle_, playerPosition);
	player_->SetGameScene(this);

	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_);
	
	railCamera_ = new RailCamera();
	railCamera_->Initialize(viewProjection_);

	player_->SetParent(railCamera_->GetWorldTransform());

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_R)) {
		GameScene::~GameScene();
		GameScene::Initialize();
	}
	railCamera_->Update();
	// ViewProjection hoge = railCamera_->GetViewProjection();

	player_->Update(railCamera_->GetWorldTransform());
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Update();
	}
	UpdateEnemyPopCommands();
	//enemy_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	CheckAllCollisions();
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	playerBullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


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
		ViewProjection tmp = railCamera_->GetViewProjection();
		viewProjection_.matView = tmp.matView;
		viewProjection_.matProjection = tmp.matProjection;
		viewProjection_.TransferMatrix();
		//viewProjection_.UpdateMatrix();
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
	
	//enemy_->Draw(viewProjection_);
	for (Enemy* enemy : enemies_) {
		enemy->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	player_->Draw(viewProjection_);
	for (PlayerBullet* bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}

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
