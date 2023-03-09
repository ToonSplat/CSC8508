#pragma once
#include "PaintableObject.h"

namespace NCL::CSC8503
{
	class ToonScreen : public PaintableObject
	{
	public:
		ToonScreen(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld);
		~ToonScreen();

		virtual void Update(float dt) override;
		virtual void Draw(OGLRenderer& r, bool isMinimap = false) override;

	protected:
		int numberOfRows;
		int index;
		int maxIndex;

		float updateTimer;
		float updateTimerLimit;

		Vector2 GetTextureOffset();
	};
}