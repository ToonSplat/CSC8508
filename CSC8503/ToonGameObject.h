#pragma once
#include "ToonTransform.h"
#include "ToonRenderObject.h"
#include <reactphysics3d/reactphysics3d.h>

namespace NCL::CSC8503
{
	class ToonGameObject
	{
	public:
		ToonGameObject(reactphysics3d::PhysicsWorld& RP3D_World);
		~ToonGameObject();

		virtual void Update(float dt) { std::cout << "Base class update"; };

		const std::string& GetName() const { return name; }

		bool IsActive() const { return isActive; }
		void SetActive(const bool& status) { isActive = status; }

		ToonTransform& GetTransform() { return transform; }

		ToonRenderObject* GetRenderObject() const { return renderObject; }
		void SetRenderObject(ToonRenderObject* newRenderObject) { renderObject = newRenderObject; newRenderObject->SetGameObject(this); }

		reactphysics3d::RigidBody* GetRigidbody() const { return rigidBody; }
		void SetRigidbody(reactphysics3d::RigidBody* RP3D_Rigidbody) { rigidBody = RP3D_Rigidbody; }
		void AddRigidbody();
		
		reactphysics3d::Collider* GetCollider() const { return collider; };
		void SetCollider(reactphysics3d::CollisionShape* RP3D_CollisionShape);

		reactphysics3d::SphereShape* GetCollisionShapeSphere() const { return collisionShapeSphere; };
		void SetCollisionShape(reactphysics3d::SphereShape* RP3D_CollisionShape) { collisionShapeSphere = RP3D_CollisionShape; }

		reactphysics3d::BoxShape* GetCollisionShapeBox() const { return collisionShapeBox; };
		void SetCollisionShape(reactphysics3d::BoxShape* RP3D_CollisionShape) { collisionShapeBox = RP3D_CollisionShape; }

		void SetPosition(const reactphysics3d::Vector3& newPos);
		void SetPosition(const Vector3& newPos);
		void SetPosition(const float& x, const float& y, const float& z);

		void SetOrientation(const reactphysics3d::Vector3& newRotEulerAngles);
		void SetOrientation(const Vector3& newRotEulerAngles);
		void SetOrientation(const reactphysics3d::Quaternion& newRot);
		void SetOrientation(const Quaternion& newRot);

		void SetScale(const Vector3& newScale) { transform.SetScale(newScale); }
		void SetScale(const reactphysics3d::Vector3& newScale) { transform.SetScale(newScale); }

		void SetWorldID(int newID) { worldID = newID; }
		int	GetWorldID() const { return worldID; }

	protected:
		bool isActive;
		int worldID;
		std::string	name;

		ToonTransform transform;
		ToonRenderObject* renderObject;
		reactphysics3d::RigidBody* rigidBody;
		reactphysics3d::BoxShape* collisionShapeBox;
		reactphysics3d::SphereShape* collisionShapeSphere;
		reactphysics3d::Collider* collider;		

	private:
		reactphysics3d::PhysicsWorld& physicsWorld;
	};
}