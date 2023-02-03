#include "ToonFollowCamera.h"
#include "Window.h"
#include "Maths.h"
#include "ToonUtils.h"
#include "ToonRaycastCallback.h"
#include <reactphysics3d/reactphysics3d.h>
//#include <iostream>

NCL::CSC8503::ToonFollowCamera::ToonFollowCamera(ToonGameObject& target) : 
	followTarget(target)
{
	requiredRayDistance = defaultRayDistance = 10.0f;
	pitchOffset = 12.0f;
	h = v = 0.0f;
}

void NCL::CSC8503::ToonFollowCamera::UpdateCamera(float dt)
{
	v -= (Window::GetMouse()->GetRelativePosition().y);
	v = Clamp(v, -80.0f, 80.0f);

	h -= (Window::GetMouse()->GetRelativePosition().x);
	if (h < 0) h += 360.0f;
	if (h > 360.0f) h -= 360.0f;

	Matrix4 yawMat = Matrix4::Rotation(h, Vector3(0, 1, 0));
	Matrix4 pitchMat = Matrix4::Rotation(v, yawMat * Vector3(-1, 0, 0));
	Matrix4 finalRotMat = pitchMat * yawMat;

	Vector3 focusPoint = ToonUtils::ConvertToNCLVector3(followTarget.GetRigidbody()->getTransform().getPosition());
	Vector3 lookDirection = finalRotMat * Vector3(0, 0, -1);

	position = focusPoint - lookDirection * requiredRayDistance;

	//Prevent Wall Clipping
	/*Ray collisionRay = Ray(focusPoint, -lookDirection);
	RayCollision collisionRayData;
	if (world.Raycast(collisionRay, collisionRayData, true, &followTarget))
	{
		if(collisionRayData.rayDistance < requiredRayDistance)
			position = focusPoint - lookDirection * (collisionRayData.rayDistance - 1.0f);
	}*/

	//reactphysics3d::Ray wallRay(ToonUtils::ConvertToRP3DVector3(position), ToonUtils::ConvertToRP3DVector3(focusPoint));
	//reactphysics3d::RaycastInfo wallRayInfo;
	//if (followTarget.GetCollider()->raycast(wallRay, wallRayInfo))
	//{
	//	reactphysics3d::Ray hitToCamRay(wallRayInfo.worldPoint, ToonUtils::ConvertToRP3DVector3(position));
	//	ToonRaycastCallback hitCallback;

	//	physicsWorld.raycast(hitToCamRay, &hitCallback);

	//	float distance = focusPoint.Length() - hitCallback.GetHitWorldPos().Length();
	//	if(distance < requiredRayDistance)
	//		position = focusPoint - lookDirection * (distance - 1.0f);

	//	std::cout << "Distance: " << distance << std::endl;
	//	//Debug::DrawLine(hitCallback.GetHitWorldPos(), hitCallback.GetHitWorldPos() + hitCallback.GetHitNormal(), Debug::RED, 5.0);
	//}

	/*Vector3 rayDir = finalRotMat * Vector3(0, 0, -1);
	Ray collisionRay = Ray(followTarget.GetTransform().GetPosition(), -rayDir);
	RayCollision collisionData;
	if (world.Raycast(collisionRay, collisionData, true, &followTarget))
	{
		Vector3 collideOffset = collisionData.collidedAt - followTarget.GetTransform().GetPosition();
		if (collideOffset.Length() <= requiredRayDistance)
		{
			Debug::DrawBox(collisionData.collidedAt, Vector3(1, 1, 1), Debug::WHITE);
			followOffset = finalRotMat * startOffset;
			position = followTarget.GetTransform().GetPosition() - rayDir * collisionData.rayDistance;
		}
		else
		{
			followOffset = finalRotMat * startOffset;
			position = followTarget.GetTransform().GetPosition() + followOffset;
		}
	}
	else
	{
		followOffset = finalRotMat * startOffset;
		position = followTarget.GetTransform().GetPosition() + followOffset;
	}*/

	//followOffset = finalRotMat * Vector3(0, 20.0f, 30.0f);
	//position = followTarget.GetTransform().GetPosition() + followOffset;

	//float distance = (position - followTarget.GetPosition()).Length();
	//std::cout << distance << std::endl;


	//Stare at GOAT!
	Matrix4 viewMatrix = Matrix4::BuildViewMatrix(position, focusPoint, Vector3(0, 1, 0)).Inverse();
	Quaternion q(viewMatrix);
	pitch = q.ToEuler().x + pitchOffset;
	yaw = q.ToEuler().y;

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	camForward = rotation * Vector3(0, 0, -1);
	camRight = rotation * Vector3(1, 0, 0);
	camUp = rotation * Vector3(0, 1, 0);
}