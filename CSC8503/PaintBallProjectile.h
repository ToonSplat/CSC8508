#pragma once
#include "ToonGameObject.h"

using namespace NCL::CSC8503;

class PaintBallProjectile : public ToonGameObject {
public:
	PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, const Vector3& position, const Vector3& rotationEuler, const float& radius);
	~PaintBallProjectile();
};