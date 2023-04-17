#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"ImGuiManager.h"
#include"PrimitiveDrawer.h"
#include"AxisIndicator.h"

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//スプライトの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	//3Dモデルの生成
	model_ = Model::Create();
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	//サウンドデータの読み込み
	soundDateHandle_ = audio_->LoadWave("fanfare.wav");
	//音声生成
	voiceHandle_ = audio_->PlayWave(soundDateHandle_, true);

	// デバッグカメラの生成 (画面横幅,画面縦幅)
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	//球を表現しようとしてできなかった
	/*for (int i = 0; i < 65; i++) {
		float theta = float(float(1) / 32 * 3.14 * i);
		circlePosition[0][i].x = radius * cosf(theta);
		circlePosition[0][i].y = 0;
		circlePosition[0][i].z = radius * sinf(theta);
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 65; j++) {
			float theta = 1 / 4 * 3.14;
			circlePosition[i + 1][j].x =
			    circlePosition[i][j].x * cosf(theta) - circlePosition[i][j].y * sinf(theta);
			circlePosition[i + 1][j].y =
			    circlePosition[i][j].x * sinf(theta) + circlePosition[i][j].y * cosf(theta);
			circlePosition[i + 1][j].z = circlePosition[i][j].z;
		}
	}*/
}

void GameScene::Update() {
	
	Vector2 position = sprite_->GetPosition();

	position.x += 2.0f;
	position.y += 1.0f;

	sprite_->SetPosition(position);

	if (input_->TriggerKey(DIK_SPACE)) {
		//音声停止
		audio_->StopWave(voiceHandle_);
	}

	//ウィンドウの作成
	ImGui::Begin("Debug1");
	//float3入力ボックス
	ImGui::InputFloat3("InputFloat3", inputFloat3);
	//float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	ImGui::End();

	ImGui::Begin("Debug2");
	// デバッグテキストの表示
	ImGui::Text("Dark Angle Yohane %d.%d.%d", 2050, 12, 31);
	ImGui::End();

	//デモウィンドウの表示を有効化
	ImGui::ShowDemoWindow();

	//デバッグカメラの更新
	debugCamera_->Update();
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
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	//ラインを描画する
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	//球を表示しようとしたができなかった。
	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 64; j++) {
			PrimitiveDrawer::GetInstance()->DrawLine3d(
			    {circlePosition[i][j].x, circlePosition[i][j].y, circlePosition[i][j].z},
			    {circlePosition[i][j + 1].x, circlePosition[i][j + 1].y,
			     circlePosition[i][j + 1].z},
			    {0.0f, 1.0f, 0.0f, 1.0f});
		}
	}*/

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
