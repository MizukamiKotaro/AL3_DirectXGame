#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = Matrix4x4::MakeAffinMatrix(scale_, rotation_, translation_);

	if (parent_) {
		matWorld_ = Matrix4x4::Multiply(matWorld_, parent_->matWorld_);
	}

	TransferMatrix();
}