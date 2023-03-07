#include "ToonMinimapCamera.h"
#include "ToonUtils.h"
#include <reactphysics3d/reactphysics3d.h>

NCL::CSC8503::ToonMinimapCamera::ToonMinimapCamera(ToonGameObject& target) :
	followTarget(target)
{
	const int width	= 1280;
	const int height = 720;
	const float zoomFactor = 15.0f;

	nearPlane	= -1.0f;
	farPlane	= 10000.0f;

	left	= (- width / 2.0f) / zoomFactor;
	right	= (width  / 2.0f) / zoomFactor;
	top		= (height / 2.0f) / zoomFactor;
	bottom	= (-height / 2.0f) / zoomFactor;

	pitch = -90.0f;
	yaw = 0.0f;

	camType	= CameraType::Orthographic;
}


void NCL::CSC8503::ToonMinimapCamera::UpdateCamera(float dt, BaseInput* inputs)
{
	yaw -= (inputs->GetMouseRelPos().x);
	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	Vector3 focusPoint = ToonUtils::ConvertToNCLVector3(followTarget.GetRigidbody()->getTransform().getPosition());
	position = Vector3(focusPoint.x, focusPoint.y + distanceFromFocus, focusPoint.z);
}
