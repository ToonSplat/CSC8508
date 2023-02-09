#include "ToonFollowCamera.h"
#include "Maths.h"
#include "ToonUtils.h"
#include "ToonRaycastCallback.h"
#include "../ThirdParty/imgui/imgui.h"
//#include <iostream>

NCL::CSC8503::ToonFollowCamera::ToonFollowCamera(ToonGameObject* target) : 
	followTarget(target)
{
	player = (Player*)followTarget;

	followOffset = Vector3(0, 2.5f, 0.0f);
	targetOffset = Vector3(0.0f, 1.0f, 4.0f);
	aimOffset = Vector3(0.5f, 1.00f, 2.0f);
	
	up = Vector3(0, 1, 0);
	right = Vector3(1, 0, 0);
	forward = Vector3(0, 0, -1);

	requiredRayDistance = defaultRayDistance = 1.0f;
	pitchOffset = -2.0f;
	h = v = 0.0f;
	smoothness = 0.1f;

	distanceThresholdMoving = 50.0f;
	distanceThresholdStanding = 2.0f;	

	reached = false;
}

void NCL::CSC8503::ToonFollowCamera::UpdateCamera(float dt)
{
	if ( (ToonGameWorld::Get() != nullptr) && !ToonGameWorld::Get()->ShowCursor())
	{
		Window::GetWindow()->ShowOSPointer(false);
		Window::GetWindow()->LockMouseToWindow(true);
	}

	v -= (Window::GetMouse()->GetRelativePosition().y);
	v = Clamp(v, -80.0f, 80.0f);

	h -= (Window::GetMouse()->GetRelativePosition().x);
	if (h < 0) h += 360.0f;
	if (h > 360.0f) h -= 360.0f;
	
	UpdatePitchAndYaw();

	Matrix4 modelMatrixNoRot = followTarget->GetModelMatrixNoRotation();
	Matrix4 modelMatrixWithRot = followTarget->GetModelMatrix();

	Matrix4 rotMatrix = Matrix4::Rotation(yaw, up) * Matrix4::Rotation(pitch, right);
	Matrix4 finalMatrix = modelMatrixNoRot * rotMatrix;	

	Vector3 targetWorldPos = ToonUtils::ConvertToNCLVector3(followTarget->GetRigidbody()->getTransform().getPosition());	
	Vector3 targetLocalPos = finalMatrix.Inverse() * Vector4(targetWorldPos, 1.0f);	

	Vector3 targetlocalPosOffset = targetLocalPos + (player->IsAiming() ? aimOffset : targetOffset);
	targetlocalPosOffset *= requiredRayDistance;
	Vector3 targetWorldPosOffset = finalMatrix * Vector4(targetlocalPosOffset, 1.0f);
	
	Vector3 targetAimLookAtLocal = targetLocalPos + Vector3(aimOffset.x, aimOffset.y, 0);
	Vector3 targetAimLookAtWorld = modelMatrixWithRot * Vector4(targetAimLookAtLocal, 1.0f);

	//Debug::DrawBox(targetAimLookAtWorld, Vector3(0.1f, 0.1f, 0.1f), Debug::GREEN, 0);

	Vector3 targetAimPosLocal = targetLocalPos + aimOffset;
	targetAimPosLocal *= requiredRayDistance;
	Vector3 targetAimPosWorld = finalMatrix * Vector4(targetAimPosLocal, 1.0f);

	//Debug::DrawBox(targetAimPosWorld, Vector3(0.1f, 0.1f, 0.1f), Debug::RED, 0);

	distanceThresholdMoving = player->IsMoving() ? player->GetRigidbody()->getLinearVelocity().length() * 0.2f : 50.0f;
	float FinalThreshold = player->IsMoving() ? distanceThresholdMoving : distanceThresholdStanding;
	Vector3 FinalLookAt = player->IsAiming() ? targetAimLookAtWorld : targetWorldPos + followOffset;
	Vector3 FinalPos = position;
	if (player->IsAiming())
	{
		FinalPos = Vector3::SmoothDamp(FinalPos, targetAimPosWorld, refVel, smoothness, FLT_MAX, dt);
		reached = false;
	}
	else if (!reached)
	{
		float distance = abs(targetWorldPosOffset.LengthSquared() - FinalPos.LengthSquared());		
		reached = distance <= FinalThreshold;
	}

	if (reached)
		FinalPos = Vector3::SmoothDamp(FinalPos, targetWorldPosOffset, refVel, 0.02f, FLT_MAX, dt);
	else
		FinalPos = Vector3::SmoothDamp(FinalPos, targetWorldPosOffset, refVel, smoothness, FLT_MAX, dt);

	position = FinalPos;
	//position = Lerp(position, FinalPos, dt * 20.0f);
	//position = Vector3::SmoothDamp(position, FinalPos, refVel, 0.1f, FLT_MAX, dt);

	reactphysics3d::Ray ray(ToonUtils::ConvertToRP3DVector3(targetWorldPos), ToonUtils::ConvertToRP3DVector3(position));
	ToonRaycastCallback wallHitData;
	ToonGameWorld::Get()->GetPhysicsWorld().raycast(ray, &wallHitData, ToonCollisionLayer::Default);

	if (wallHitData.IsHit())
		position = wallHitData.GetHitWorldPos();

	/*Matrix4 viewMatrix = Matrix4::BuildViewMatrix(position, FinalLookAt, up).Inverse();
	Quaternion q(viewMatrix);
	Vector3 qEuler = q.ToEuler();
	pitch = player->IsAiming() ? Lerp(pitch, qEuler.x + pitchOffset, dt * 20.0f) : qEuler.x;
	yaw = player->IsAiming() ? Lerp(yaw, qEuler.y, dt * 20.0f) : qEuler.y;*/
	//pitch = q.ToEuler().x + pitchOffset;
	//yaw = q.ToEuler().y;


	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	camForward = rotation * Vector3(0, 0, -1);
	camRight = rotation * Vector3(1, 0, 0);
	camUp = rotation * Vector3(0, 1, 0);

	//Debug::DrawBox(position + camForward * 50.0f, Vector3(0.1f, 0.1f, 0.1f), Debug::YELLOW, 0);
	
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