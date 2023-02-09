#include "ToonMinimapCamera.h"
#include "Window.h"
#include "Maths.h"
#include "ToonUtils.h"
#include "ToonRaycastCallback.h"
#include <reactphysics3d/reactphysics3d.h>

NCL::CSC8503::ToonMinimapCamera::ToonMinimapCamera(ToonGameObject& target) :
	followTarget(target)
{
	int width	= 1280;
	int height	= 720;


	nearPlane	= -1.0f;
	farPlane	= 10000.0f;

	left	= -width  / 2.0f;
	right	=  width  / 2.0f;
	top		=  height / 2.0f;
	bottom	= -height / 2.0f;

	left	= left	 / zoomFactor;
	right	= right	 / zoomFactor;
	top		= top	 / zoomFactor;
	bottom	= bottom / zoomFactor;


	pitch		= -90.0f;
	yaw = 0.0f;

	camType		= CameraType::Orthographic;

}


void NCL::CSC8503::ToonMinimapCamera::UpdateCamera(float dt)
{
	yaw -= (Window::GetMouse()->GetRelativePosition().x);
	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	Vector3 focusPoint = ToonUtils::ConvertToNCLVector3(followTarget.GetRigidbody()->getTransform().getPosition());
	position = Vector3(focusPoint.x, focusPoint.y + distanceFromFocus, focusPoint.z);
}
