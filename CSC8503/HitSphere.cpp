#include "HitSphere.h"
#include "PaintableObject.h"
#include "ToonGameWorld.h"
#include "ToonUtils.h"
#include "ToonLevelManager.h"

using namespace NCL;
using namespace CSC8503;

HitSphere::HitSphere(reactphysics3d::PhysicsWorld& RP3D_World, Team* team, reactphysics3d::Vector3 position, float radius /*, Weapon* weapon*/) : ToonGameObject(RP3D_World), radius(radius) {
	lifetime = 1.5f;
	teamColour = team->getTeamColour();
	this->radius = radius;

	GetTransform().SetPosition(position).
		SetScale(Vector3(radius, radius, radius));

	SetRenderObject(new ToonRenderObject(&GetTransform(), ToonLevelManager::Get()->GetMesh("sphere"), ToonLevelManager::Get()->GetTexture("basic"), ToonLevelManager::Get()->GetShader("basic")));
	GetRenderObject()->SetColour(Vector4(team->getTeamColour(), 1.0f));

	AddRigidbody();
	GetRigidbody()->setType(reactphysics3d::BodyType::DYNAMIC);

	reactphysics3d::SphereShape* sphereShape = ToonGameWorld::Get()->GetPhysicsCommon().createSphereShape(radius);
	SetCollisionShape(sphereShape);
	SetCollider(sphereShape);
	GetCollider()->getMaterial().setBounciness(0.1f);

	//GetCollider()->setIsTrigger(true);


	GetRigidbody()->setUserData(this);

	ToonGameWorld::Get()->AddGameObject(this);
	ToonGameWorld::Get()->AddHitSphere(this);

	/*
	* weaponRadius = weapon->GetRadius();
	*/
}

HitSphere::~HitSphere() {

}

void HitSphere::Update(float dt) {
	if (lifetime <= 0) {
		this->isActive = false;
		ToonGameWorld::Get()->RemoveHitSphere(this);
		ToonGameWorld::Get()->RemoveGameObject(this, true);
	}
	lifetime -= dt;
}

void HitSphere::OnCollisionBegin(ToonGameObject* otherObject) { //Must be changed to onTrigger at some point
	if (dynamic_cast<PaintableObject*>(otherObject)) {
		PaintableObject* p = (PaintableObject*)otherObject;
		p->AddImpactPoint(ImpactPoint(ToonUtils::ConvertToNCLVector3(GetRigidbody()->getTransform().getPosition()), teamColour, 5)); //radius to weapon radius
	}
}
