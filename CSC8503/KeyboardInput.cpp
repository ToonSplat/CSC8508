#pragma once
#include "KeyboardInput.h"
#include "Keyboard.h"
#include "Mouse.h"

using namespace NCL;

void KeyboardInput::UpdateState() {
	UpdateLastState();

	inputs.moveDir = Vector2(0, 0);
	if (keyboard->KeyDown(KeyboardKeys::W) || keyboard->KeyDown(KeyboardKeys::UP))
	{
		inputs.moveDir.y += 1;
	}
	if (keyboard->KeyDown(KeyboardKeys::S) || keyboard->KeyDown(KeyboardKeys::DOWN))
	{
		inputs.moveDir.y -= 1;
	}
	if (keyboard->KeyDown(KeyboardKeys::A) || keyboard->KeyDown(KeyboardKeys::LEFT))
	{
		inputs.moveDir.x -= 1;
	}
	if (keyboard->KeyDown(KeyboardKeys::D) || keyboard->KeyDown(KeyboardKeys::RIGHT))
	{
		inputs.moveDir.x += 1;
	}

	inputs.mousePosition = mouse->GetWindowPosition();
	inputs.relativeMousePosition = mouse->GetRelativePosition();
	inputs.aiming = mouse->ButtonHeld(MouseButtons::RIGHT);
	inputs.shooting = mouse->ButtonHeld(MouseButtons::LEFT);
	inputs.jumping = keyboard->KeyDown(KeyboardKeys::SPACE);
	inputs.selecting = keyboard->KeyDown(KeyboardKeys::RETURN);
	inputs.back = keyboard->KeyDown(KeyboardKeys::ESCAPE);

	BaseInput::UpdateState();
}