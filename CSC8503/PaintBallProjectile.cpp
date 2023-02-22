#include "PaintBallProjectile.h"
#include "ToonGameWorld.h"
#include "ToonLevelManager.h"

using namespace NCL::CSC8503;

PaintBallProjectile::PaintBallProjectile(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, const float& _impactSize, Team* _team) : 
	ToonGameObject(RP3D_World, gameWorld), impactSize(_impactSize), team(_team) {
	ConfigureSound();
}

PaintBallProjectile::~PaintBallProjectile(){
	delete speaker;
}

void PaintBallProjectile::ConfigureSound() {
	speaker = new AudioEmitter();
	speaker->SetPriority(SoundPriority::LOW);
	speaker->SetLooping(false);
	speaker->SetSound(Audio::GetSound("splash.wav"));
}