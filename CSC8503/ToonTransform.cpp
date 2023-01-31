#include "ToonTransform.h"

void NCL::CSC8503::ToonTransform::UpdateMatrix()
{
	modelMatrix =	NCL::Maths::Matrix4::Translation(position.x, position.y, position.z) *
					NCL::Maths::Matrix4(orientation.x, orientation.y, orientation.z, orientation.z) *
					NCL::Maths::Matrix4::Scale(scale.x, scale.y, scale.z);
}