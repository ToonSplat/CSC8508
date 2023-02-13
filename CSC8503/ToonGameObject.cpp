#include "ToonGameObject.h"
#include "ToonRenderObject.h"
#include "ToonGameWorld.h"
#include "OGLMesh.h"

NCL::CSC8503::ToonGameObject::ToonGameObject(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld) : physicsWorld(RP3D_World), gameWorld(gameWorld)
{
	worldID = -1;
	isActive = true;

	rigidBody = nullptr;
	renderObject = nullptr;
	collisionShapeBox = nullptr;
	collisionShapeSphere = nullptr;
	collider = nullptr;
}

NCL::CSC8503::ToonGameObject::~ToonGameObject()
{
	if (rigidBody) {
		if (collider)
			rigidBody->removeCollider(collider); // Not clear if need to delete Collider seperately, doing for safety
		physicsWorld.destroyRigidBody(rigidBody);
	}
	
	if(collisionShapeSphere)
		gameWorld->GetPhysicsCommon().destroySphereShape(collisionShapeSphere);
	if(collisionShapeBox)
		gameWorld->GetPhysicsCommon().destroyBoxShape(collisionShapeBox);
	
	delete renderObject;
}

void NCL::CSC8503::ToonGameObject::Draw(int subLayer)
{
	if (!renderObject || !renderObject->GetMesh())
		return;

	OGLMesh* boundMesh = (OGLMesh*)renderObject->GetMesh();

	GLuint	mode = 0;
	int		count = 0;
	int		offset = 0;

	if (boundMesh->GetSubMeshCount() == 0) {
		if (boundMesh->GetIndexCount() > 0) {
			count = boundMesh->GetIndexCount();
		}
		else {
			count = boundMesh->GetVertexCount();
		}
	}
	else {
		const SubMesh* m = boundMesh->GetSubMesh(subLayer);
		offset = m->start;
		count = m->count;
	}

	switch (boundMesh->GetPrimitiveType()) {
	case GeometryPrimitive::Triangles:		mode = GL_TRIANGLES;		break;
	case GeometryPrimitive::Points:			mode = GL_POINTS;			break;
	case GeometryPrimitive::Lines:			mode = GL_LINES;			break;
	case GeometryPrimitive::TriangleFan:	mode = GL_TRIANGLE_FAN;		break;
	case GeometryPrimitive::TriangleStrip:	mode = GL_TRIANGLE_STRIP;	break;
	case GeometryPrimitive::Patches:		mode = GL_PATCHES;			break;
	}

	if (boundMesh->GetIndexCount() > 0) {
		glDrawElements(mode, count, GL_UNSIGNED_INT, (const GLvoid*)(offset * sizeof(unsigned int)));
	}
	else {
		glDrawArrays(mode, 0, count);
	}
}

void NCL::CSC8503::ToonGameObject::AddRigidbody()
{
	rigidBody = physicsWorld.createRigidBody(transform.GetR3DTransform());
}

void NCL::CSC8503::ToonGameObject::SetCollider(reactphysics3d::CollisionShape* RP3D_CollisionShape)
{
	if (rigidBody == nullptr)
		return;

	collider = rigidBody->addCollider(RP3D_CollisionShape, reactphysics3d::Transform::identity());
	SetColliderLayer(ToonCollisionLayer::Default);

	int everythingMask = ToonCollisionLayer::Default | ToonCollisionLayer::Character;
	SetColliderLayerMask(ToonCollisionLayer(everythingMask));
}

Vector3 NCL::CSC8503::ToonGameObject::GetPosition() const
{
	if (rigidBody)
		return ToonUtils::ConvertToNCLVector3(rigidBody->getTransform().getPosition());

	return Vector3();
}

Quaternion NCL::CSC8503::ToonGameObject::GetOrientation() const
{
	if (rigidBody)
		return ToonUtils::ConvertToNCLQuaternion(rigidBody->getTransform().getOrientation());

	return Quaternion();
}

void NCL::CSC8503::ToonGameObject::SetPosition(const reactphysics3d::Vector3& newPos)
{
	if(rigidBody)
		rigidBody->setTransform(reactphysics3d::Transform(newPos, rigidBody->getTransform().getOrientation()));
}

void NCL::CSC8503::ToonGameObject::SetPosition(const Vector3& newPos)
{
	SetPosition(reactphysics3d::Vector3(newPos.x, newPos.y, newPos.z));
}

void NCL::CSC8503::ToonGameObject::SetPosition(const float& x, const float& y, const float& z)
{
	SetPosition(reactphysics3d::Vector3(x, y, z));
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const reactphysics3d::Vector3& newRotVec3)
{
	SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(newRotVec3));
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const Vector3& newRotVec3)
{
	SetOrientation(reactphysics3d::Quaternion::fromEulerAngles(reactphysics3d::Vector3(newRotVec3.x, newRotVec3.y, newRotVec3.z)));
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const reactphysics3d::Quaternion& newRotQuat)
{
	if(rigidBody)
		rigidBody->setTransform(reactphysics3d::Transform(rigidBody->getTransform().getPosition(), newRotQuat));
}

void NCL::CSC8503::ToonGameObject::SetOrientation(const Quaternion& newRotQuat)
{
	SetOrientation(reactphysics3d::Quaternion(newRotQuat.x, newRotQuat.y, newRotQuat.z, newRotQuat.w));
}

void NCL::CSC8503::ToonGameObject::CalculateModelMatrix()
{
	if (!rigidBody)
		return;

	reactphysics3d::Transform currTransform = rigidBody->getTransform();
	reactphysics3d::Transform interpolated = reactphysics3d::Transform::interpolateTransforms(prevTransform, currTransform, gameWorld->interpolationFactor);
	prevTransform = currTransform;

	float matrix[16];
	interpolated.getOpenGLMatrix(matrix);

	modelMatrix = Matrix4(matrix) * Matrix4::Scale(GetScale());

	//modelMatrix = Matrix4::Translation(GetPosition()) * Matrix4(GetOrientation()) * Matrix4::Scale(GetScale());
}

Matrix4 NCL::CSC8503::ToonGameObject::GetModelMatrixNoRotation() const
{
	return Matrix4::Translation(GetPosition()) * Matrix4::Scale(GetScale());
}
