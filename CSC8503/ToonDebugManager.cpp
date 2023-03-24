#include "ToonDebugManager.h"

using namespace NCL;
using namespace CSC8503;

ToonDebugManager* ToonDebugManager::instance = NULL;

ToonDebugManager::ToonDebugManager() {
	isCollisionDisplayToggled = false;
	showAIPathGraph = false;
	showAIPathDebug = false;
	sectionStats.emplace("Loading", CodeSectionStats());
	sectionStats.emplace("Audio", CodeSectionStats());
	sectionStats.emplace("Animation", CodeSectionStats());
	sectionStats.emplace("Networking", CodeSectionStats());
	sectionStats.emplace("Physics", CodeSectionStats());
	sectionStats.emplace("Rendering", CodeSectionStats());
	sectionStats.emplace("Frame", CodeSectionStats());
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

void ToonDebugManager::StartTimeCount(string section) {
	sectionStats[section].start = high_resolution_clock::now();
}

void ToonDebugManager::EndTimeCount(string section) {
	sectionStats[section].end = high_resolution_clock::now();
	ConvertTimeTaken(sectionStats[section]);
}

void ToonDebugManager::ConvertTimeTaken(CodeSectionStats& section) {
	std::chrono::microseconds timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(section.end - section.start);
	double temp = (double)timeTaken.count() / 1000.0f;
	section.timeTaken = std::to_string(temp) + " ms";
}

string ToonDebugManager::GetTimeTaken(string section) {
	return sectionStats[section].timeTaken;
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

