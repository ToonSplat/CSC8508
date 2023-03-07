#include "ToonMapCamera.h"

NCL::CSC8503::ToonMapCamera::ToonMapCamera()
{
	const int width = 1280;
	const int height = 720;

	const float zoomFactor = 5.0f;
	const float distanceFromFocus = 300.0f;

	nearPlane = -1.0f;
	farPlane = 10000.0f;

	left = (-width / 2.0f) / zoomFactor;
	right = (width / 2.0f) / zoomFactor;
	top = (height / 2.0f) / zoomFactor;
	bottom = (-height / 2.0f) / zoomFactor;

	pitch = -90.0f;
	yaw = 0.0f;

	camType = CameraType::Orthographic;

	position = Vector3(0, distanceFromFocus, 0);
}
