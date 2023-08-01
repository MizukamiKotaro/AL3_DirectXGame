#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "BaseCharacter.h"

class Enemy : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection) override;

private:
	void InitializeRotGimmick();

	void UpdateRotGimmick();

private:
	enum Joints { kModelIndexBody, kModelIndexUpJoint };

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformUpJoint_;

	float parameter_ = 0.0f;
};
