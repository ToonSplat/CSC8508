#pragma once
#include "ToonTransform.h"
#include "ToonRenderObject.h"
#include "ToonNetworkObject.h"
#include "ToonUtils.h"
#include <reactphysics3d/reactphysics3d.h>
#include "OGLRenderer.h"

namespace NCL::CSC8503
{
	enum ToonCollisionLayer
	{
		Nothing = 0,
		Default = 1,
		Character = 2
	};

	class ToonGameWorld;
	class ToonGameObject
	{
	public:
		ToonGameObject(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld);
		~ToonGameObject();

		virtual void Update(float dt) { /*std::cout << "Base class update\n";*/ };
		virtual void Draw(OGLRenderer& r, bool isMinimap = false);

		const std::string& GetName() const { return name; }

		bool IsActive() const { return isActive; }
		bool HasSkin() const { return hasSkin; }
		void SetActive(const bool& status) { isActive = status; }
		
		ToonTransform& GetTransform() { return transform; }

		ToonRenderObject* GetRenderObject() const { return renderObject; }
		void SetRenderObject(ToonRenderObject* newRenderObject) { renderObject = newRenderObject; newRenderObject->SetGameObject(this); }

		ToonNetworkObject* GetNetworkObject() const { return networkObject; }
		void SetNetworkObject(ToonNetworkObject* newNetworkObject) { networkObject = newNetworkObject; newNetworkObject; }

		reactphysics3d::RigidBody* GetRigidbody() const { return rigidBody; }
		void SetRigidbody(reactphysics3d::RigidBody* RP3D_Rigidbody) { rigidBody = RP3D_Rigidbody; }
		void AddRigidbody();
		
		reactphysics3d::Collider* GetCollider() const { return collider; };
		void SetCollider(reactphysics3d::CollisionShape* RP3D_CollisionShape, reactphysics3d::Transform collisionTransform = reactphysics3d::Transform::identity());

		reactphysics3d::SphereShape* GetCollisionShapeSphere() const { return collisionShapeSphere; };
		void SetCollisionShape(reactphysics3d::SphereShape* RP3D_CollisionShape) { collisionShapeSphere = RP3D_CollisionShape; }

		reactphysics3d::BoxShape* GetCollisionShapeBox() const { return collisionShapeBox; };
		void SetCollisionShape(reactphysics3d::BoxShape* RP3D_CollisionShape) { collisionShapeBox = RP3D_CollisionShape; }

		reactphysics3d::CapsuleShape* GetCollisionShapeCapsule() const { return collisionShapeCapsule; };
		void SetCollisionShape(reactphysics3d::CapsuleShape* RP3D_CollisionShape) { collisionShapeCapsule = RP3D_CollisionShape; }

		reactphysics3d::ConcaveMeshShape* GetCollisionShapeConcave() const { return collisionShapeConcave; };
		void SetCollisionShape(reactphysics3d::ConcaveMeshShape* RP3D_CollisionShape) { collisionShapeConcave = RP3D_CollisionShape; }

		reactphysics3d::ConvexMeshShape* GetCollisionShapeConvex() const { return collisionShapeConvex; };
		void SetCollisionShape(reactphysics3d::ConvexMeshShape* RP3D_CollisionShape) { collisionShapeConvex = RP3D_CollisionShape; }

		Vector3 GetPosition() const;
		Quaternion GetOrientation() const;
		Vector3 GetScale() const { return ToonUtils::ConvertToNCLVector3(transform.GetScale()); };

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

		void CalculateModelMatrix();
		Matrix4 GetModelMatrix() const { return modelMatrix; }
		Matrix4 GetModelMatrixNoRotation() const;

		int GetColliderLayer() const 
		{ 
			if (collider)
				return collider->getCollisionCategoryBits();

			return ToonCollisionLayer::Nothing;
		}

		void SetColliderLayer(ToonCollisionLayer newLayer) 
		{ 
			if (collider) collider->setCollisionCategoryBits(newLayer);
		}

		void SetColliderLayerMask(ToonCollisionLayer newMask)
		{
			if (collider) collider->setCollideWithMaskBits(newMask);
		}

	protected:
		bool isActive;
		bool hasSkin;
		int worldID;
		std::string	name;

		reactphysics3d::Transform prevTransform;
		ToonTransform transform;
		ToonRenderObject* renderObject;
		ToonNetworkObject* networkObject;

		reactphysics3d::RigidBody* rigidBody;
		reactphysics3d::BoxShape* collisionShapeBox;
		reactphysics3d::SphereShape* collisionShapeSphere;
		reactphysics3d::CapsuleShape* collisionShapeCapsule;
		reactphysics3d::ConcaveMeshShape* collisionShapeConcave;
		reactphysics3d::ConvexMeshShape* collisionShapeConvex;
		reactphysics3d::Collider* collider;
		Matrix4 modelMatrix;

		ToonGameWorld* gameWorld;

	private:
		reactphysics3d::PhysicsWorld& physicsWorld;
	};
}