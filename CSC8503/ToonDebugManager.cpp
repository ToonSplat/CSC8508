#include "ToonDebugManager.h"

using namespace NCL;
using namespace CSC8503;

ToonDebugManager* ToonDebugManager::instance = NULL;

ToonDebugManager::ToonDebugManager() {
	isCollisionDisplayToggled = false;
}

void ToonDebugManager::StartLoad() {
	loadStart = high_resolution_clock::now();
}
void ToonDebugManager::EndLoad() {
	loadEnd = high_resolution_clock::now();
	loadTimeTaken = ConvertTimeTaken(loadStart, loadEnd);
}

void ToonDebugManager::StartFrame() {
	frameStart = high_resolution_clock::now();
}
void ToonDebugManager::EndFrame() {
	frameEnd = high_resolution_clock::now();
	frameTimeTaken = ConvertTimeTaken(frameStart, frameEnd);
}

void ToonDebugManager::StartNetworking() {
	networkingStart = high_resolution_clock::now();
}
void ToonDebugManager::EndNetworking() {
	networkingEnd = high_resolution_clock::now();
	networkingTimeTaken = ConvertTimeTaken(networkingStart, networkingEnd);
}

void ToonDebugManager::StartPhysics() {
	physicsStart = high_resolution_clock::now();
}
void ToonDebugManager::EndPhysics() {
	physicsEnd = high_resolution_clock::now();
	physicsTimeTaken = ConvertTimeTaken(physicsStart, physicsEnd);
}

void ToonDebugManager::StartAnimation() {
	animationStart = high_resolution_clock::now();
}
void ToonDebugManager::EndAnimation() {
	animationEnd = high_resolution_clock::now();
	animationTimeTaken = ConvertTimeTaken(animationStart, animationEnd);
}

void ToonDebugManager::StartRendering() {
	renderingStart = high_resolution_clock::now();
}
void ToonDebugManager::EndRendering() {
	renderingEnd = high_resolution_clock::now();
	graphicsTimeTaken = ConvertTimeTaken(frameStart, renderingEnd);
}

void ToonDebugManager::Update() {
	CalculateMemoryUsage();
	CalculateMemoryUsageByProgram();
	if (world) {
		world->GetPhysicsWorld().setIsDebugRenderingEnabled(isCollisionDisplayToggled);
		if (isCollisionDisplayToggled && world)
			DisplayCollisionBoxes();
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

void ToonDebugManager::DisplayCollisionBoxes() {
	world->GetPhysicsWorld().setIsDebugRenderingEnabled(true);
	reactphysics3d::DebugRenderer& dbr = world->GetPhysicsWorld().getDebugRenderer();
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);

	for (int i = 0; i < (int)dbr.getNbLines(); i++) {
		Debug::DrawLine(Vector3(dbr.getLines()[i].point1.x, dbr.getLines()[i].point1.y, dbr.getLines()[i].point1.z), Vector3(dbr.getLines()[i].point2.x, dbr.getLines()[i].point2.y, dbr.getLines()[i].point2.z), Debug::RED);
	}

	dbr.reset();
}

void ToonDebugManager::ToggleCollisionDisplay() {
	isCollisionDisplayToggled = !isCollisionDisplayToggled;
}