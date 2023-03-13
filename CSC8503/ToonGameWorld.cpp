#include "ToonGameWorld.h"
#include "ToonGameObject.h"
#include "Window.h"
#include "PaintableObject.h"
#include "PaintBallProjectile.h"
#include "Player.h"
#include "ToonEventListener.h"

using namespace NCL;
using namespace NCL::CSC8503;

NCL::CSC8503::ToonGameWorld::ToonGameWorld()
{
	physicsWorld = physicsCommon.createPhysicsWorld();
	physicsWorld->setGravity(reactphysics3d::Vector3(0.0f, -9.81f, 0.0f));

	teams.emplace(1, new Team("The Green Goblins", Team::T_GREEN_GOBLINS, 1));
	teams.emplace(2, new Team("The Purple Prawns", Team::T_PURPLE_PRAWNS, 2));
	teams.emplace(3, new Team("The Blue Bulldogs", Team::T_BLUE_BULLDOGS, 3));
	teams.emplace(4, new Team("The Orange Otters", Team::T_ORANGE_OTTERS, 4));

	teams[1]->AddSpawnPoint(TeamSpawnPointData(Vector3(45.0f, 12.0f, 0.0f), Vector3(0.0f, 1.57f, 0.0f)));
	teams[1]->AddSpawnPoint(TeamSpawnPointData(Vector3(42.0f, 4.0f, 14.0f), Vector3(0.0f, 1.57f, 0.0f)));
	teams[1]->AddSpawnPoint(TeamSpawnPointData(Vector3(42.0f, 4.0f, -14.0f), Vector3(0.0f, 1.57f, 0.0f)));

	teams[2]->AddSpawnPoint(TeamSpawnPointData(Vector3(-45.0f, 12.0f, 0.0f), Vector3(0.0f, -1.57f, 0.0f)));
	teams[2]->AddSpawnPoint(TeamSpawnPointData(Vector3(-42.0f, 4.0f, 14.0f), Vector3(0.0f, -1.57f, 0.0f)));
	teams[2]->AddSpawnPoint(TeamSpawnPointData(Vector3(-42.0f, 4.0f, -14.0f), Vector3(0.0f, -1.57f, 0.0f)));

	teams[3]->AddSpawnPoint(TeamSpawnPointData(Vector3(15.0f, 4.0f, -42.0f), Vector3(0.0f, 3.14f, 0.0f)));
	teams[3]->AddSpawnPoint(TeamSpawnPointData(Vector3(0.0f, 9.5f, -45.0f), Vector3(0.0f, 3.14f, 0.0f)));
	teams[3]->AddSpawnPoint(TeamSpawnPointData(Vector3(15.0f, 4.0f, -42.0f), Vector3(0.0f, 3.14f, 0.0f)));

	teams[4]->AddSpawnPoint(TeamSpawnPointData(Vector3(15.0f, 4.0f, 42.0f), Vector3(0.0f, 0.0f, 0.0f)));
	teams[4]->AddSpawnPoint(TeamSpawnPointData(Vector3(0.0f, 9.5f, 45.0f), Vector3(0.0f, 0.0f, 0.0f)));
	teams[4]->AddSpawnPoint(TeamSpawnPointData(Vector3(15.0f, 4.0f, 42.0f), Vector3(0.0f, 0.0f, 0.0f)));

	mainCameras[1] = new Camera();
}

NCL::CSC8503::ToonGameWorld::~ToonGameWorld()
{
	ClearAndErase();
	physicsCommon.destroyPhysicsWorld(physicsWorld);
	delete eventListener;
	for (auto& [id, camera] : mainCameras)
		delete camera;
	delete minimapCamera;
	for (auto& [id, team] : teams)
		delete team;
}

void NCL::CSC8503::ToonGameWorld::Clear()
{
	gameObjects.clear();
	paintableObjects.clear();
	activePaintballs.clear();
	activeHitSpheres.clear();
	objectsToDelete.clear();
	worldIDCounter = 0;
	worldStateCounter = 0;
}

void NCL::CSC8503::ToonGameWorld::ClearAndErase()
{
	for (auto& i : gameObjects) {
		if (dynamic_cast<Player*>(i))
			delete (Player*)i;
		else delete i;
	}

	Clear();
}

void NCL::CSC8503::ToonGameWorld::AddGameObject(ToonGameObject* o)
{
	gameObjects.emplace_back(o);
	if (!dynamic_cast<Player*>(o))
		o->SetWorldID(worldIDCounter++);
	worldStateCounter++;
}

void NCL::CSC8503::ToonGameWorld::RemoveGameObject(ToonGameObject* o, bool andDelete)
{
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), o), gameObjects.end());
	if (andDelete) {
		if (dynamic_cast<Player*>(o))
			delete (Player*)o;
		else delete o;
	}
	worldStateCounter++;
}

void ToonGameWorld::AddPaintball(PaintBallProjectile* paintball) {
	AddGameObject(paintball);
	activePaintballs.emplace(paintball);
}
void ToonGameWorld::RemovePaintball(PaintBallProjectile* paintball) {
	activePaintballs.erase(paintball);
	objectsToDelete.insert(paintball);
}

void ToonGameWorld::AddHitSphere(HitSphere* hitSphere) {
	AddGameObject(hitSphere);
	activeHitSpheres.emplace(hitSphere);	
}
void ToonGameWorld::RemoveHitSphere(HitSphere* hitSphere) {
	activeHitSpheres.erase(hitSphere);
	objectsToDelete.insert(hitSphere);
	updateMap = true;
}

void ToonGameWorld::AddPaintableObject(ToonGameObject* paintableObject) {
	paintableObjects.emplace(paintableObject);
}
void ToonGameWorld::RemovePaintableObject(ToonGameObject* paintableObject) {
	paintableObjects.erase(paintableObject);
	objectsToDelete.insert(paintableObject);
}

void ToonGameWorld::GetGameObjects(void) const {
	for (auto& object : gameObjects)
		std::cout << object->GetRigidbody()->getUserData() << std::endl;
}

void ToonGameWorld::DeleteMarkedObjects() {
	for (auto& object : objectsToDelete)
		if (dynamic_cast<Player*>(object))
			delete (Player*)object;
		else delete object;
	objectsToDelete.clear();
}

void NCL::CSC8503::ToonGameWorld::UpdateWorld(float dt)
{
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q))
	{
		showCursor = !showCursor;
		if (showCursor)
		{
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else
		{
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}

	for (auto& object : gameObjects)
		object->Update(dt);
}

void NCL::CSC8503::ToonGameWorld::OperateOnContents(ToonGameObjectFunc f)
{
	for (ToonGameObject* g : gameObjects) {
		f(g);
	}
}

Team* NCL::CSC8503::ToonGameWorld::GetTeamLeastPlayers()
{
	Team* weakestTeam = teams[1];
	int lowestPlayerCount = weakestTeam->GetPlayerCount();
	for (auto& [ID, team] : teams) {
		if (team->GetPlayerCount() < lowestPlayerCount) {
			lowestPlayerCount = team->GetPlayerCount();
			weakestTeam = team;
		}
	}
	return weakestTeam;
}