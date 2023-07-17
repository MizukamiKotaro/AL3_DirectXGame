#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Skydome {
public: 

	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
};
