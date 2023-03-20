#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <Vector3.h>
#include <random>

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

	static NCL::Maths::Quaternion ConvertToNCLQuaternion(const reactphysics3d::Quaternion& q)
	{
		return NCL::Maths::Quaternion(q.x, q.y, q.z, q.w);
	}

	static reactphysics3d::Quaternion ConvertToRP3DQuaternion(const NCL::Maths::Quaternion& q)
	{
		return reactphysics3d::Quaternion(q.x, q.y, q.z, q.w);
	}

	static float RandF(const float& min, const float& max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(min, max);

		return (float)dis(gen);
	}
}