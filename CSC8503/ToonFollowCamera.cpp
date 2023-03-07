#include "ToonFollowCamera.h"
#include "Maths.h"
#include "ToonUtils.h"
#include "ToonRaycastCallback.h"
#include "ToonGameWorld.h"

NCL::CSC8503::ToonFollowCamera::ToonFollowCamera(ToonGameWorld* gameWorld, ToonGameObject* target, float fov) : 
	gameWorld(gameWorld), followTarget(target)
{
	player = (Player*)followTarget;

	targetOffset = Vector3(0.75f, 1.6f, 3.5f);
	
	up = Vector3(0, 1, 0);
	right = Vector3(1, 0, 0);
	forward = Vector3(0, 0, -1);

	this->fov = fov;
	startFOV = fov;
	aimFOV = startFOV - 20.0f;
	vFov = 0.0f;
	zoomSmoothess = 0.1f;
}

void NCL::CSC8503::ToonFollowCamera::UpdateCamera(float dt, BaseInput* inputs)
{
	if ( (gameWorld != nullptr) && !gameWorld->ShowCursor())
	{
		Window::GetWindow()->ShowOSPointer(false);
		Window::GetWindow()->LockMouseToWindow(true);
	}
	
	//Update the mouse by how much
	pitch -= (inputs->GetMouseRelPos().y);
	yaw -= (inputs->GetMouseRelPos().x);

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	Matrix4 modelMatrixNoRot = followTarget->GetModelMatrixNoRotation();

	Matrix4 rotMatrix = Matrix4::Rotation(yaw, up) * Matrix4::Rotation(pitch, right);
	Matrix4 finalMatrix = modelMatrixNoRot * rotMatrix;	

	Vector3 targetWorldPos = ToonUtils::ConvertToNCLVector3(followTarget->GetRigidbody()->getTransform().getPosition());	
	Vector3 targetLocalPos = finalMatrix.Inverse() * Vector4(targetWorldPos, 1.0f);

	Vector3 posOffsetLocal = targetLocalPos + targetOffset;
	position = finalMatrix * Vector4(posOffsetLocal, 1.0f);

	if (player && player->IsAiming())
		fov = SmoothDamp(fov, aimFOV, vFov, zoomSmoothess, FLT_MAX, dt);
	else
		fov = SmoothDamp(fov, startFOV, vFov, zoomSmoothess, FLT_MAX, dt);

	reactphysics3d::Ray ray(ToonUtils::ConvertToRP3DVector3(targetWorldPos + Vector3(0, 2.5f, 0)), ToonUtils::ConvertToRP3DVector3(position));
	ToonRaycastCallback wallHitData;
	gameWorld->GetPhysicsWorld().raycast(ray, &wallHitData, ToonCollisionLayer::Default);

	if (wallHitData.IsHit())
		position = wallHitData.GetHitWorldPos();

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	camForward = rotation * Vector3(0, 0, -1);
	camRight = rotation * Vector3(1, 0, 0);
	camUp = rotation * Vector3(0, 1, 0);
}