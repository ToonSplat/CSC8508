#pragma once
#include "PlayerControl.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Camera.h"
#include <map>

#define MINIMUM_MOVEMENT 0.15f
#define CONTROLLER_SENSITIVITY 1.5f

namespace NCL {
	using namespace Maths;
	struct InputState {
		Vector2 moveDir;
		bool	aiming;
		bool	shooting;
		bool	jumping;

		bool	lastShooting;
		bool	lastJumping;

		Vector2 mousePosition;
		Vector2 relativeMousePosition;
		bool	selecting;
		bool	lastSelecting;
		bool	back;
		bool	lastBack;

		bool	lastUp;
		bool	lastDown;
		bool	lastLeft;
		bool	lastRight;
		InputState() {
			moveDir			= Vector2(0, 0);
			aiming			= false;
			shooting		= false;
			jumping			= false;

			lastShooting	= false;
			lastJumping		= false;

			mousePosition	= Vector2(0, 0);
			relativeMousePosition = Vector2(0, 0);
			selecting		= false;
			lastSelecting	= false;

			back			= false;
			lastBack		= false;

			lastUp			= false;
			lastDown		= false;
			lastLeft		= false;
			lastRight		= false;
		}
	};
	class BaseInput {
	public:
		BaseInput() {}
		virtual ~BaseInput() {}

		virtual void UpdateState() { 
			if (BaseInput::invert) {
				inputs.relativeMousePosition.y *= -1;
			}
		}
		void UpdateGameControls(PlayerControl* controls, Camera* camera) {
			Vector3 forward = camera->GetForward();
			Vector3 right = camera->GetRight();
			Vector3 up = camera->GetUp();

			Maths::Vector3 linearMovement;
			linearMovement += forward * inputs.moveDir.y;
			linearMovement += right * inputs.moveDir.x;

			controls->direction[0] = short(linearMovement.x * 1000);
			controls->direction[1] = short(linearMovement.z * 1000);

			controls->camera[0] = short(camera->GetPitch() * 72.0f);
			controls->camera[1] = short(camera->GetYaw() * 72.0f);

			controls->aiming =		IsAiming();
			controls->shooting =	IsShooting();
			controls->jumping =		IsJumping();
		}

		Vector2 GetMovement() const	{ return inputs.moveDir; }
		bool IsAiming() const		{ return inputs.aiming; }
		bool IsShooting() const		{ return inputs.shooting; }
		bool IsShootingOnce() const	{ return inputs.shooting && !inputs.lastShooting; }
		bool IsJumping() const		{ return inputs.jumping && !inputs.lastJumping; }

		Vector2 GetMousePosition() const { return inputs.mousePosition; }
		Vector2 GetMouseRelPos() const	{ return inputs.relativeMousePosition; }
		bool IsSelecting() const	{ return inputs.selecting && inputs.lastSelecting == false; }
		bool IsBack() const			{ return inputs.back && inputs.lastBack == false; }

		bool IsPushingUp() const	{ return (inputs.moveDir.y > MINIMUM_MOVEMENT && inputs.lastUp == false); }
		bool IsPushingDown() const	{ return (inputs.moveDir.y < -MINIMUM_MOVEMENT && inputs.lastDown == false); }
		bool IsPushingLeft() const	{ return (inputs.moveDir.x < -MINIMUM_MOVEMENT && inputs.lastLeft == false); }
		bool IsPushingRight() const { return (inputs.moveDir.x > MINIMUM_MOVEMENT && inputs.lastRight == false); }

		static void SetInverted(bool inverted) {
			invert = inverted;
		}
	protected:
		void UpdateLastState() {
			if (inputs.moveDir.y > MINIMUM_MOVEMENT) inputs.lastUp = true; else inputs.lastUp = false;
			if (inputs.moveDir.y < -MINIMUM_MOVEMENT) inputs.lastDown = true; else inputs.lastDown = false;
			if (inputs.moveDir.x > MINIMUM_MOVEMENT) inputs.lastRight = true; else inputs.lastRight = false;
			if (inputs.moveDir.x < -MINIMUM_MOVEMENT) inputs.lastLeft = true; else inputs.lastLeft = false;
			inputs.mousePosition = Vector2(0, 0);
			inputs.lastShooting = inputs.shooting;
			inputs.lastJumping = inputs.jumping;
			inputs.lastSelecting = inputs.selecting;
			inputs.lastBack = inputs.back;
		}
		InputState inputs;
		static bool invert;
	};
}