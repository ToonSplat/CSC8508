#include "PaintBallProjectile.h"
#include "ToonGameWorld.h"
#include "ToonLevelManager.h"

using namespace NCL::CSC8503;

PaintBallProjectile::PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, const float& _impactSize, Team* _team) : 
	ToonGameObject(RP3D_World), impactSize(_impactSize), team(_team) {
}

PaintBallProjectile::~PaintBallProjectile(){
}
