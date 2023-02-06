#include "ToonFollowCamera.h"
#include "Window.h"
#include "Maths.h"
#include "ToonUtils.h"
#include "ToonRaycastCallback.h"
#include <reactphysics3d/reactphysics3d.h>
//#include <iostream>

NCL::CSC8503::ToonFollowCamera::ToonFollowCamera(ToonGameObject* target) : 
	followTarget(target)
{
	requiredRayDistance = defaultRayDistance = 0.35f;
	pitchOffset = 12.0f;
	h = v = 0.0f;
	radianX = radianY = 0.0f;
	followOffset = Vector3(0, 0, 10.0f);
	targetOffset = Vector3(0.0f, 1.0f, 4.0f);
	aimOffset = Vector3(0.5f, 1.0f, 2.0f);
	
	up = Vector3(0, 1, 0);
	right = Vector3(1, 0, 0);

	Quaternion rot;
	reactphysics3d::Quaternion rRot = followTarget->GetRigidbody()->getTransform().getOrientation();
	rot.x = rRot.x;
	rot.y = rRot.y;
	rot.z = rRot.z;
	rot.w = rRot.w;

	_originTransform = Matrix4::Translation(followTarget->GetRigidbody()->getTransform().getPosition().x,
		followTarget->GetRigidbody()->getTransform().getPosition().y,
		followTarget->GetRigidbody()->getTransform().getPosition().z) *

		Matrix4(rot) *

		Matrix4::Scale(followTarget->GetRenderObject()->GetTransform()->GetScale().x,
						followTarget->GetRenderObject()->GetTransform()->GetScale().y,
						followTarget->GetRenderObject()->GetTransform()->GetScale().z);

	_back = _originTransform * Vector3(0, 0, -1);
	_right = _originTransform * Vector3(1, 0, 0);

	player = (Player*)followTarget;
}

void NCL::CSC8503::ToonFollowCamera::UpdateCamera(float dt)
{
	v -= (Window::GetMouse()->GetRelativePosition().y);
	v = Clamp(v, -80.0f, 80.0f);

	lookEuler += Vector3(Window::GetMouse()->GetRelativePosition().y, Window::GetMouse()->GetRelativePosition().x, 0) * dt * 75.0f;
	lookEuler.y = (float)((int)lookEuler.y % 360);

	//std::cout << lookEuler << std::endl;
	//lookEuler.z = (float)((int)lookEuler.z % 360);

	h -= (Window::GetMouse()->GetRelativePosition().x);
	if (h < 0) h += 360.0f;
	if (h > 360.0f) h -= 360.0f;

	radianX = DegreesToRadians(v);
	radianY = DegreesToRadians(-h);

	Quaternion rot;
	reactphysics3d::Quaternion rRot = followTarget->GetRigidbody()->getTransform().getOrientation();
	rot.x = rRot.x;
	rot.y = rRot.y;
	rot.z = rRot.z;
	rot.w = rRot.w;

	Matrix4 modelMatrixNoRot = Matrix4::Translation(followTarget->GetRigidbody()->getTransform().getPosition().x,
		followTarget->GetRigidbody()->getTransform().getPosition().y,
		followTarget->GetRigidbody()->getTransform().getPosition().z) *		

		Matrix4::Scale(followTarget->GetRenderObject()->GetTransform()->GetScale().x, 
						followTarget->GetRenderObject()->GetTransform()->GetScale().y, 
						followTarget->GetRenderObject()->GetTransform()->GetScale().z);

	Matrix4 modelMatrixWithRot = Matrix4::Translation(followTarget->GetRigidbody()->getTransform().getPosition().x,
		followTarget->GetRigidbody()->getTransform().getPosition().y,
		followTarget->GetRigidbody()->getTransform().getPosition().z) *

		Matrix4(rot) *

		Matrix4::Scale(followTarget->GetRenderObject()->GetTransform()->GetScale().x,
			followTarget->GetRenderObject()->GetTransform()->GetScale().y,
			followTarget->GetRenderObject()->GetTransform()->GetScale().z);

	Matrix4 rotMatrix = Matrix4::Rotation(h, up) * Matrix4::Rotation(v, right);
	Matrix4 finalMatrix = modelMatrixNoRot * rotMatrix;

	Vector3 targetWorldPos = ToonUtils::ConvertToNCLVector3(followTarget->GetRigidbody()->getTransform().getPosition());	
	Vector3 targetLocalPos = finalMatrix.Inverse() * Vector4(targetWorldPos, 1.0f);

	Vector3 localOffset = targetLocalPos + (player->IsAiming() ? aimOffset : targetOffset);
	Vector3 worldOffset = finalMatrix * Vector4(localOffset, 1.0f);
	
	Vector3 targetAimLocal = targetLocalPos + Vector3(0.65f, 0.75f, 0);
	Vector3 targetAimWorld = modelMatrixWithRot * Vector4(targetAimLocal, 1.0f);

	Debug::DrawBox(targetAimWorld, Vector3(0.1f, 0.1f, 0.1f), Debug::GREEN, 0);

	Matrix4 viewMatrix = Matrix4::BuildViewMatrix(position, targetWorldPos, up).Inverse();
	Quaternion q(viewMatrix);
	pitch = q.ToEuler().x;
	yaw = q.ToEuler().y;

	position = Lerp(position, worldOffset, dt * 10.0f);

#pragma region From Unity Tutorial
	/*float cYaw = lookEuler.y;
float cPitch = lookEuler.x;
_rotY = Quaternion::RotateTowards(_rotY, Quaternion::AxisAngleToQuaterion(up, cYaw), dt * 400.0f);
_rotX = Quaternion::RotateTowards(_rotX, Quaternion::AxisAngleToQuaterion(_right, cPitch), dt * 400.0f);

Vector3 offset = targetOffset;
Vector3 shoulderOffset = _rotY * _originTransform * offset;
Vector3 armOffset = _rotY * (_rotX * _back * requiredRayDistance);

Vector3 shoulderPosition = targetPos + shoulderOffset;
position = shoulderPosition + armOffset;

//Debug::DrawLine(shoulderPosition, shoulderPosition + up, Debug::RED);

Matrix4 viewMatrix = Matrix4::BuildViewMatrix(position, shoulderPosition, up).Inverse();
Quaternion q(viewMatrix);
pitch = q.ToEuler().x;
yaw = q.ToEuler().y;*/
#pragma endregion


	/*position.x = targetPos.x + followOffset.z * cos(radianY) * cos(radianX) + followOffset.x;
	position.y = targetPos.y + followOffset.z * sin(radianX) + followOffset.y;
	position.z = targetPos.z + followOffset.z * sin(radianY) * cos(radianX);*/
#pragma region OLD
	/*Matrix4 yawMat = Matrix4::Rotation(h, Vector3(0, 1, 0));
	Matrix4 pitchMat = Matrix4::Rotation(v, yawMat * Vector3(-1, 0, 0));
	Matrix4 finalRotMat = pitchMat * yawMat;

	Vector3 focusPoint = ToonUtils::ConvertToNCLVector3(followTarget.GetRigidbody()->getTransform().getPosition());
	Vector3 lookDirection = finalRotMat * Vector3(0, 0, -1);

	position = focusPoint - lookDirection * requiredRayDistance;

	//Stare at GOAT!
	Matrix4 viewMatrix = Matrix4::BuildViewMatrix(position, focusPoint, Vector3(0, 1, 0)).Inverse();
	Quaternion q(viewMatrix);
	pitch = q.ToEuler().x + pitchOffset;
	yaw = q.ToEuler().y;

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	camForward = rotation * Vector3(0, 0, -1);
	camRight = rotation * Vector3(1, 0, 0);
	camUp = rotation * Vector3(0, 1, 0);*/
#pragma endregion


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
}