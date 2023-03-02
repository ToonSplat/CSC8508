#pragma once
#include "ToonGameObject.h"
#include "ApplyPaint.h"
#include "ImpactPoint.h"

namespace NCL {
	namespace CSC8503 {
		class PaintableObject : public ToonGameObject, public ApplyPaint {
		public:
			PaintableObject(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld) : ToonGameObject(RP3D_World, gameWorld) {
			}
			~PaintableObject(void) = default;
		};
	}
}