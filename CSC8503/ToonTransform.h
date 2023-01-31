#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "Matrix4.h"

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

			reactphysics3d::Vector3 GetPosition() const { return position; }
			reactphysics3d::Quaternion GetOrientation() const { return orientation; }
			reactphysics3d::Vector3 GetScale() const { return scale; }
			NCL::Maths::Matrix4 GetMatrix() const { return modelMatrix; }

			ToonTransform& SetPosition(const reactphysics3d::Vector3& newPosition)
			{ 
				position = newPosition; 
				UpdateMatrix();
				return *this;
			}

			ToonTransform& SetPosition(const Vector3& newPosition)
			{
				position.x = newPosition.x;
				position.y = newPosition.y;
				position.z = newPosition.z;
				UpdateMatrix();
				return *this;
			}

			ToonTransform& SetOrientation(const reactphysics3d::Quaternion& newOrientation)
			{ 
				orientation = newOrientation;
				UpdateMatrix();
				return *this;
			}

			ToonTransform& SetOrientation(const Quaternion& newOrientation)
			{
				orientation.x = newOrientation.x;
				orientation.y = newOrientation.y;
				orientation.z = newOrientation.z;
				orientation.w = newOrientation.w;
				UpdateMatrix();
				return *this;
			}
			
			ToonTransform& SetScale(const reactphysics3d::Vector3& newScale)
			{ 
				position = newScale;
				UpdateMatrix();
				return *this;
			}

			ToonTransform& SetScale(const Vector3& newScale)
			{
				position.x = newScale.x;
				position.y = newScale.y;
				position.z = newScale.z;
				UpdateMatrix();
				return *this;
			}

		protected:
			void UpdateMatrix();

			reactphysics3d::Vector3 position;
			reactphysics3d::Quaternion orientation;
			reactphysics3d::Vector3 scale;
			NCL::Maths::Matrix4 modelMatrix;
		};
	}
}