#include "ToonDebugManager.h"

using namespace NCL;
using namespace CSC8503;

ToonDebugManager::ToonDebugManager() {
	isDebugging = false;
	isDebugToggled = false;
}

ToonDebugManager::~ToonDebugManager() {

}

void ToonDebugManager::Update(float dt) {

	if (isDebugging) {
		isDebugToggled = true;

		DisplayCollisionBoxes(dt);
		CalculateMemoryUsage();
		CalculateMemoryUsageByThis();
		Debug::UpdateRenderables(dt);

		std::cout << "FPS -  " << "REDO" << std::endl << "Virtual Memory - " << usedVirtualMem << "/" << totalVirtualMem << std::endl << "Physical Memory - " << usedPhysMem << "/" << totalPhysMem << std::endl;
		std::cout << "virtual used by me - " << virtualMemUsedByMe << std::endl << "physcial used by me - " << physMemUsedByMe << std::endl;
		std::cout << "Frame Time Taken - " << frameTimeTaken << "ms" << std::endl << "Physics Time Taken - " << physicsTimeTaken << "ms" << std::endl << "Graphics Time Taken - " << graphicsTimeTaken << "ms" << std::endl;
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

void ToonDebugManager::CalculateMemoryUsageByThis() {
	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	virtualMemUsedByMe = pmc.PrivateUsage;

	physMemUsedByMe = pmc.WorkingSetSize;
}

void ToonDebugManager::DisplayCollisionBoxes(float dt) {
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