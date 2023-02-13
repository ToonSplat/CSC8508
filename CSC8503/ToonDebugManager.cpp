#include "ToonDebugManager.h"

using namespace NCL;
using namespace CSC8503;

ToonDebugManager::ToonDebugManager() {
	isDebugging = false;;
	isCollisionDisplayToggled = false;
	isDebugToggled = false;
}

ToonDebugManager::~ToonDebugManager() {

}

void ToonDebugManager::Update(float dt) {

	if (isDebugging) {
		isDebugToggled = true;

		DisplayCollisionBoxes(dt);
		CalculateMemoryUsage();
		CalculateMemoryUsageByProgram();
		Debug::UpdateRenderables(dt);
	}
	else if (!isDebugging && isDebugToggled) {
		Debug::UpdateRenderables(dt);
		ToonGameWorld::Get()->GetPhysicsWorld().setIsDebugRenderingEnabled(false);
		isDebugToggled = false;
	}
}

void ToonDebugManager::CalculateMemoryUsage() {
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);

	totalVirtualMem = memInfo.ullTotalPageFile;
	usedVirtualMem = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

	totalPhysMem = memInfo.ullTotalPhys;
	usedPhysMem = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
}

void ToonDebugManager::CalculateMemoryUsageByProgram() {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	virtualMemUsedByProgram = pmc.PrivateUsage;

	physMemUsedByProgram = pmc.WorkingSetSize;
}

void ToonDebugManager::DisplayCollisionBoxes(float dt) {
	if (isCollisionDisplayToggled) {
		ToonGameWorld::Get()->GetPhysicsWorld().setIsDebugRenderingEnabled(true);
		reactphysics3d::DebugRenderer& dbr = ToonGameWorld::Get()->GetPhysicsWorld().getDebugRenderer();
		dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
		dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
		dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
		dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
		dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);

		for (int i = 0; i < dbr.getNbLines(); i++) {
			Debug::DrawLine(Vector3(dbr.getLines()[i].point1.x, dbr.getLines()[i].point1.y, dbr.getLines()[i].point1.z), Vector3(dbr.getLines()[i].point2.x, dbr.getLines()[i].point2.y, dbr.getLines()[i].point2.z), Debug::RED, dt);
		}

		dbr.reset();
	}
}

void ToonDebugManager::ToggleCollisionDisplay() {
	if (isDebugging) {
		isCollisionDisplayToggled = !isCollisionDisplayToggled;
	}
}