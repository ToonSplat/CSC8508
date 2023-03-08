#include "Player.h"
#include "Maths.h"
#include "ToonRaycastCallback.h"

using namespace NCL;
using namespace NCL::Maths;
using namespace CSC8503;

Player::Player(reactphysics3d::PhysicsWorld& RP3D_World, ToonGameWorld* gameWorld, Team* team) : ToonGameObjectAnim(RP3D_World, gameWorld), team(team)
{
	team->AddPlayer();
	isAiming = false;
	isMoving = false;
	isGrounded = false;

	moveSpeed = 1500.0f;
	rotationSpeed = 6.0f;
	aimingSpeed = 10.0f;

	if (!LoadAnim("Player_Idle")) return;
	if (!LoadAnim("Player_Idle_Aim")) return;
	if (!LoadAnim("Player_Run")) return;
	if (!LoadAnim("Player_Run_Aim_F")) return;
	if (!LoadAnim("Player_Run_Aim_FL")) return;
	if (!LoadAnim("Player_Run_Aim_FR")) return;
	if (!LoadAnim("Player_Run_Aim_L")) return;
	if (!LoadAnim("Player_Run_Aim_R")) return;
	if (!LoadAnim("Player_Run_Aim_B")) return;
	if (!LoadAnim("Player_Run_Aim_BL")) return;
	if (!LoadAnim("Player_Run_Aim_BR")) return;
	if (!LoadAnim("Player_Jump")) return;
	if (!LoadAnim("Player_Dance")) return;
	if (!LoadAnim("Player_Defeat")) return;

	PlayAnim("Player_Idle");

	audiosystem = AudioSystem::GetAudioSystem();
}

Player::~Player()
{
	team->RemovePlayer();
}

bool Player::WeaponUpdate(float dt, PlayerControl* controls)
{
	return weapon.Update(dt, controls);
}

void Player::MovementUpdate(float dt, PlayerControl* controls) {

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F5))
		renderObject->GetShader()->ReloadShader();

	isGrounded = IsGrounded();	

	reactphysics3d::Vector3 linearMovement = reactphysics3d::Vector3(controls->direction[0] / 1000.0f, 0, controls->direction[1] / 1000.0f);
	linearMovement.normalize();

	if (isGrounded)
	{
		groundDir = NCL::Maths::Vector3::ProjectOnPlane(ToonUtils::ConvertToNCLVector3(linearMovement), groundNormal);
		groundDir.Normalise();
		Debug::DrawLine(GetPosition(), GetPosition() + groundNormal, Debug::GREEN);
		Debug::DrawLine(GetPosition(), GetPosition() + NCL::Maths::Vector3::Cross(groundNormal, groundDir), Debug::RED);
		Debug::DrawLine(GetPosition(), GetPosition() + groundDir, Debug::BLUE);
	}

	isMoving = linearMovement.length() >= 0.1f;
	isAiming = controls->aiming;

	if (isAiming) {
		targetAngle = controls->camera[1];
	}
	else if (isMoving)
		targetAngle = RadiansToDegrees(atan2(-linearMovement.x, -linearMovement.z));

	reactphysics3d::Quaternion newRot = ToonUtils::ConvertToRP3DQuaternion(NCL::Maths::Quaternion::EulerAnglesToQuaternion(0, targetAngle, 0));
	SetOrientation(reactphysics3d::Quaternion::slerp(ToonUtils::ConvertToRP3DQuaternion(GetOrientation()), newRot, (controls->aiming ? aimingSpeed : rotationSpeed) * dt));


	if (isMoving)
		rigidBody->applyWorldForceAtCenterOfMass(linearMovement * moveSpeed * dt);
	if (controls->jumping) {
		GetRigidbody()->applyWorldForceAtCenterOfMass(reactphysics3d::Vector3(0, 1, 0) * 1000.0f);
		controls->jumping = false;
	}

	audiosystem->SetListenerTransform(GetTransform().GetMatrix());
	
}

void Player::Update(float dt) {
	
	/*string pos = std::to_string(GetPosition().x);
	pos += ", " + std::to_string(GetPosition().y);
	pos += ", " + std::to_string(GetPosition().z);
	
	Debug::Print(pos, NCL::Maths::Vector2(2, 70), Debug::WHITE);*/

	/*Debug::DrawLine(GetPosition(), GetPosition() + Up(), Debug::GREEN);
	Debug::DrawLine(GetPosition(), GetPosition() + Right(), Debug::RED);
	Debug::DrawLine(GetPosition(), GetPosition() + Forward(), Debug::BLUE);*/
	
	ToonGameObjectAnim::Update(dt);
	reactphysics3d::Vector3 linVel = GetRigidbody()->getLinearVelocity();
	linVel = GetRigidbody()->getTransform().getInverse().getOrientation() * linVel;
	linVel.y = 0;
	isMoving = linVel.length() >= 0.5f;
	linVel.normalize();
	
	if (isGrounded)
	{
		if (isAiming)
		{
			if (isMoving)
			{
				if (linVel.z < -0.5) {
					if (linVel.x > 0.1)
						PlayAnim("Player_Run_Aim_FR");
					else if (linVel.x < -0.1)
						PlayAnim("Player_Run_Aim_FL");
					else PlayAnim("Player_Run_Aim_F");
				}
				else if (linVel.z > 0.5) {
					if (linVel.x > 0.1)
						PlayAnim("Player_Run_Aim_BR");
					else if (linVel.x < -0.1)
						PlayAnim("Player_Run_Aim_BL");
					else PlayAnim("Player_Run_Aim_B");
				}
				else if (linVel.x > 0.5)
					PlayAnim("Player_Run_Aim_R");
				else if (linVel.x < -0.5)
					PlayAnim("Player_Run_Aim_L");
				else {
					std::cout << "How did we get here?\n";
					PlayAnim("Player_Idle_Aim");
				}
			}
			else
				PlayAnim("Player_Idle_Aim");
		}
		else
		{
			if (isMoving) PlayAnim("Player_Run");
			else PlayAnim("Player_Idle");
		}
	}
	else
		PlayAnim("Player_Jump");
}

void Player::SetPositionRotation(TeamSpawnPointData spawnPoint, Camera* followCamera)
{
	SetPosition(spawnPoint.GetPosition());
	SetOrientation(spawnPoint.GetRotation());

	followCamera->SetYaw(RadiansToDegrees(spawnPoint.GetRotation().y));
	targetAngle = RadiansToDegrees(spawnPoint.GetRotation().y);
}

void Player::SetWeapon(PaintBallClass* base) {
	weapon = base->MakeInstance();
	//std::cout << "WEAPON MADE" << std::endl;
	weapon.SetOwner(this);
	weapon.SetTeam(team);
}

bool Player::IsGrounded()
{
	NCL::Maths::Vector3 startPos = GetPosition() + NCL::Maths::Vector3(0, 2.0f, 0);
	Debug::DrawBox(startPos, NCL::Maths::Vector3(0.5f, 0.5f, 0.5f), Debug::CYAN);
	reactphysics3d::Ray ray(ToonUtils::ConvertToRP3DVector3(startPos), ToonUtils::ConvertToRP3DVector3(GetPosition() + NCL::Maths::Vector3(0, -20.0f, 0)));	
	ToonRaycastCallback groundHitData;
	gameWorld->GetPhysicsWorld().raycast(ray, &groundHitData, ToonCollisionLayer::Default);

	if (groundHitData.IsHit())
	{
		float distance = std::abs((groundHitData.GetHitWorldPos() - startPos).Length());
		groundNormal = groundHitData.GetHitNormal();
		return distance <= 2.5f;
	}

	return false;
}
