#include "ToonTransform.h"

void NCL::CSC8503::ToonTransform::UpdateMatrix()
{
	modelMatrix =	Matrix4::Translation(GetPosition().x, GetPosition().y, GetPosition().z) *
					/*Matrix4(GetOrientation().x, GetOrientation().y, GetOrientation().z, GetOrientation().w) * */
					Matrix4::Scale(scale.x, scale.y, scale.z);
}