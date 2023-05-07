#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	matWorld_ = Matrix4x4::MakeAffinMatrix(scale_, rotation_, translation_);

	TransferMatrix();
}