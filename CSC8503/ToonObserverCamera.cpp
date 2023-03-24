#include "ToonObserverCamera.h"
#include "BaseInput.h"

NCL::CSC8503::ToonObserverCamera::ToonObserverCamera(void) {
	left = 0;
	right = 0;
	top = 0;
	bottom = 0;

	pitch = 0.0f;
	yaw = 0.0f;

	fov = 45.0f;
	nearPlane = 1.0f;
	farPlane = 1000.0f;

	camType = CameraType::Perspective;
};


void NCL::CSC8503::ToonObserverCamera::UpdateCamera(float dt, BaseInput* inputs)
{
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

	float frameSpeed = 100 * dt;

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Matrix4::Rotation(pitch, Vector3(1, 0, 0));
	camForward = rotation * Vector3(0, 0, -1);
	camRight = rotation * Vector3(1, 0, 0);
	camUp = rotation * Vector3(0, 1, 0);

	position += camForward * inputs->GetMovement().y * frameSpeed;
	position += camRight * inputs->GetMovement().x * frameSpeed;
}
