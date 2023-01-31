#pragma once
#include "ToonTransform.h"
#include <reactphysics3d/reactphysics3d.h>

namespace NCL::CSC8503
{
	class ToonRenderObject;
	class ToonGameObject
	{
	public:
		ToonGameObject(reactphysics3d::PhysicsWorld& RP3D_World);
		~ToonGameObject();

		const std::string& GetName() const { return name; }

		bool IsActive() const { return isActive; }

		ToonTransform& GetTransform() { return transform; }

		ToonRenderObject* GetRenderObject() const { return renderObject; }
		void SetRenderObject(ToonRenderObject* newRenderObject) { renderObject = newRenderObject; }

		reactphysics3d::RigidBody* GetRigidbody() const { return rigidBody; }
		void SetRigidbody(reactphysics3d::RigidBody* RP3D_Rigidbody) { rigidBody = RP3D_Rigidbody; }
		void AddRigidbody();
		
		reactphysics3d::Collider* GetCollider() const { return collider; };
		void SetCollider(reactphysics3d::CollisionShape* RP3D_CollisionShape);

		reactphysics3d::CollisionShape* GetCollisionShape() const { return collisionShape; };
		void SetCollisionShape(reactphysics3d::CollisionShape* RP3D_CollisionShape) { collisionShape = RP3D_CollisionShape; }

		void SetWorldID(int newID) { worldID = newID; }
		int	GetWorldID() const { return worldID; }

	protected:
		bool isActive;
		int worldID;
		std::string	name;

		ToonTransform transform;
		ToonRenderObject* renderObject;
		reactphysics3d::RigidBody* rigidBody;
		reactphysics3d::CollisionShape* collisionShape;
		reactphysics3d::Collider* collider;

	private:
		reactphysics3d::PhysicsWorld& physicsWorld;
	};
}