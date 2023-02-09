#include "Camera.h"
#include "ToonGameWorld.h"
#include "ToonGameObject.h"

using namespace NCL;
using namespace NCL::CSC8503;

ToonGameWorld* ToonGameWorld::instance = nullptr;

NCL::CSC8503::ToonGameWorld::ToonGameWorld()
{
	physicsWorld = physicsCommon.createPhysicsWorld();
	physicsWorld->setGravity(reactphysics3d::Vector3(0.0f, -9.81f, 0.0f));
	eventListener = new ToonEventListener(physicsWorld);
	teams.insert(new Team("The Green Gulls", Vector3(0, 1.0f, 0)));
	teams.insert(new Team("The Purple Panthers", Vector3(1.0f, 0, 1.0f)));

	instance = this;
}

NCL::CSC8503::ToonGameWorld::~ToonGameWorld()
{
	physicsCommon.destroyPhysicsWorld(physicsWorld);
	delete eventListener;
	delete mainCamera;
	delete minimapCamera;
	for (auto& team : teams)
		delete team;
}

void NCL::CSC8503::ToonGameWorld::Clear()
{
	gameObjects.clear();
	activePaintballs.clear();
	worldIDCounter = 0;
	worldStateCounter = 0;
}

void NCL::CSC8503::ToonGameWorld::ClearAndErase()
{
	for (auto& i : gameObjects) {
		delete i;
	}
	for (auto& i : activePaintballs) {
		delete i;
	}
	
	Clear();
}

void NCL::CSC8503::ToonGameWorld::AddGameObject(ToonGameObject* o)
{
	gameObjects.emplace_back(o);
	o->SetWorldID(worldIDCounter++);
	worldStateCounter++;
}

void NCL::CSC8503::ToonGameWorld::RemoveGameObject(ToonGameObject* o, bool andDelete)
{
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), o), gameObjects.end());
	if (andDelete) {
		delete o;
	}
	worldStateCounter++;
}

void NCL::CSC8503::ToonGameWorld::UpdateWorld(float dt)
{
}

void NCL::CSC8503::ToonGameWorld::OperateOnContents(ToonGameObjectFunc f)
{
	for (ToonGameObject* g : gameObjects) {
		f(g);
	}
}

Team* NCL::CSC8503::ToonGameWorld::GetTeamLeastPlayers()
{
	Team* weakestTeam = *teams.begin();
	int lowestPlayerCount = weakestTeam->getPlayerCount();
	for (Team* team : teams) {
		if (team->getPlayerCount() < lowestPlayerCount) {
			lowestPlayerCount = team->getPlayerCount();
			weakestTeam = team;
		}
	}
	return weakestTeam;
}
