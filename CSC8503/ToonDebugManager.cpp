#include "ToonDebugManager.h"

using namespace NCL;
using namespace CSC8503;

ToonDebugManager* ToonDebugManager::instance = NULL;

ToonDebugManager::ToonDebugManager() {
	isCollisionDisplayToggled = false;
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

void ToonDebugManager::StartTimeCount(measuring m) {
	switch (m) {
	case load: loadStart = high_resolution_clock::now(); break;
	case frame:	frameStart = high_resolution_clock::now(); break;
	case audio: audioStart = high_resolution_clock::now(); break;
	case networking: networkingStart = high_resolution_clock::now(); break;
	case physics: physicsStart = high_resolution_clock::now(); break;
	case animation: animationStart = high_resolution_clock::now(); break;
	case rendering: renderingStart = high_resolution_clock::now(); break;
	}
}

void ToonDebugManager::EndTimeCount(measuring m) {
	switch (m) {
	case load: loadTimeTaken = ConvertTimeTaken(loadStart, high_resolution_clock::now()); break;
	case frame:	frameTimeTaken = ConvertTimeTaken(frameStart, high_resolution_clock::now()); break;
	case audio: audioTimeTaken = ConvertTimeTaken(audioStart, high_resolution_clock::now()); break;
	case networking: networkingTimeTaken = ConvertTimeTaken(networkingStart, high_resolution_clock::now()); break;
	case physics: physicsTimeTaken = ConvertTimeTaken(physicsStart, high_resolution_clock::now()); break;
	case animation: animationTimeTaken = ConvertTimeTaken(animationStart, high_resolution_clock::now()); break;
	case rendering: graphicsTimeTaken = ConvertTimeTaken(renderingStart, high_resolution_clock::now()); break;
	}
}

string ToonDebugManager::GetTimeTaken(measuring m) {
	switch (m) {
	case load: return loadTimeTaken; break;
	case frame:	return frameTimeTaken; break;
	case audio: return audioTimeTaken; break;
	case networking: return networkingTimeTaken; break;
	case physics: return physicsTimeTaken; break;
	case animation: return animationTimeTaken; break;
	case rendering: return graphicsTimeTaken; break;
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

string ToonDebugManager::ConvertTimeTaken(high_resolution_clock::time_point start, high_resolution_clock::time_point end) {
	std::chrono::microseconds timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	double temp = (double)timeTaken.count() / 1000.0f;
	return std::to_string(temp) + " ms";
}

void ToonDebugManager::DisplayCollisionBoxes() {
	world->GetPhysicsWorld().setIsDebugRenderingEnabled(true);
	reactphysics3d::DebugRenderer& dbr = world->GetPhysicsWorld().getDebugRenderer();
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
	dbr.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);

	/*for (int i = 0; i < (int)dbr.getNbLines(); i++) {
		Debug::DrawLine(Vector3(dbr.getLines()[i].point1.x, dbr.getLines()[i].point1.y, dbr.getLines()[i].point1.z), Vector3(dbr.getLines()[i].point2.x, dbr.getLines()[i].point2.y, dbr.getLines()[i].point2.z), Debug::RED);
	}*/

	int numTri = (int)dbr.getNbTriangles();
	if (numTri > 0)
	{
		const reactphysics3d::DebugRenderer::DebugTriangle* tri = world->GetPhysicsWorld().getDebugRenderer().getTrianglesArray();
		for (int i = 0; i < numTri; i++)
		{
			Debug::DrawTriangle(ToonUtils::ConvertToNCLVector3(tri->point1), ToonUtils::ConvertToNCLVector3(tri->point2), ToonUtils::ConvertToNCLVector3(tri->point2), Debug::YELLOW, 0.0f);
			tri++;
		}
	}

	dbr.reset();
}

void ToonDebugManager::ToggleCollisionDisplay() {
	isCollisionDisplayToggled = !isCollisionDisplayToggled;
}

