#include "Camera.h"
#include "ToonGameWorld.h"
#include "ToonGameObject.h"

using namespace NCL;
using namespace NCL::CSC8503;

ToonGameWorld* ToonGameWorld::instance = nullptr;

NCL::CSC8503::ToonGameWorld::ToonGameWorld()
{
	mainCamera = new Camera();
	instance = this;
}

NCL::CSC8503::ToonGameWorld::~ToonGameWorld()
{
}

void NCL::CSC8503::ToonGameWorld::Clear()
{
	gameObjects.clear();
	worldIDCounter = 0;
	worldStateCounter = 0;
}

void NCL::CSC8503::ToonGameWorld::ClearAndErase()
{
	for (auto& i : gameObjects) {
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
