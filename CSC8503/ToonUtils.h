#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <Vector3.h>

namespace ToonUtils
{
	static reactphysics3d::Vector3 ConvertToRP3DVector3(const NCL::Maths::Vector3& v)
	{
		return reactphysics3d::Vector3(v.x, v.y, v.z);
	}

	static NCL::Maths::Vector3 ConvertToNCLVector3(const reactphysics3d::Vector3& v)
	{
		return NCL::Maths::Vector3(v.x, v.y, v.z);
	}
}