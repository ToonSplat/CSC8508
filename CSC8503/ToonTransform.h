#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "Matrix4.h"
#include "Vector3.h"
#include "Quaternion.h"

using namespace NCL::Maths;

namespace NCL 
{
	namespace CSC8503 
	{
		class ToonTransform
		{
		public:
			ToonTransform() { scale = reactphysics3d::Vector3(1, 1, 1); }
			~ToonTransform() {};

			reactphysics3d::Vector3 GetPosition() const { return r3DTransform.getPosition(); }
			reactphysics3d::Quaternion GetOrientation() const { return r3DTransform.getOrientation(); }
			reactphysics3d::Vector3 GetScale() const { return scale; }
			reactphysics3d::Transform& GetR3DTransform() { return r3DTransform; }
			Matrix4 GetMatrix() const { return modelMatrix; }

			ToonTransform& SetPosition(const reactphysics3d::Vector3& newPosition)
			{ 
				r3DTransform.setPosition(newPosition);
				UpdateMatrix();
				return *this;
			}

			ToonTransform& SetPosition(const Vector3& newPosition)
			{
				reactphysics3d::Vector3 newPos(newPosition.x, newPosition.y, newPosition.z);
				r3DTransform.setPosition(newPos);
				UpdateMatrix();
				return *this;
			}

			ToonTransform& SetOrientation(const reactphysics3d::Quaternion& newOrientation)
			{ 
				r3DTransform.setOrientation(newOrientation);
				UpdateMatrix();
				return *this;
			}

			ToonTransform& SetOrientation(const Quaternion& newOrientation)
			{
				reactphysics3d::Quaternion newQuat(newOrientation.x, newOrientation.y, newOrientation.z, newOrientation.w);
				r3DTransform.setOrientation(newQuat);
				UpdateMatrix();
				return *this;
			}
			
			ToonTransform& SetScale(const reactphysics3d::Vector3& newScale)
			{ 
				scale = newScale;
				UpdateMatrix();
				return *this;
			}

			ToonTransform& SetScale(const Vector3& newScale)
			{
				scale.x = newScale.x;
				scale.y = newScale.y;
				scale.z = newScale.z;
				UpdateMatrix();
				return *this;
			}

		protected:
			void UpdateMatrix();

			reactphysics3d::Transform r3DTransform;
			reactphysics3d::Vector3 scale;
			Matrix4 modelMatrix;
		};
	}
}