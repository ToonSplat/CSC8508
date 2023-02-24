#pragma once
#include "windows.h"
#include "psapi.h"
#include "Debug.h"

#include "ToonGameObject.h"
#include "Window.h"
#include "PhysicsObject.h"
#include <vector>
#include "PaintBallClass.h"

namespace NCL
{
	namespace CSC8503
	{
		using std::chrono::high_resolution_clock;
		class ToonDebugManager
		{
		public:
			static void Create() {
				if (instance == NULL)
					instance = new ToonDebugManager();
			}

			static void Destroy() { delete instance; }

			static ToonDebugManager& Instance() {
				if (instance == NULL)
					Create();
				return *instance;
			}

			void Update(float dt);

			void StartFrame();
			void EndFrame();

			void StartPhysics();
			void EndPhysics();

			void StartRendering();
			void EndRendering();

			void ToggleDebug() { isDebugging = !isDebugging; }

			void SetPhysicsUpdateTime(auto start, auto end) { physicsTimeTaken = ConvertTimeTaken(start, end); }
			void SetGraphicsUpdateTime(auto start, auto end) { graphicsTimeTaken = ConvertTimeTaken(start, end); }
			void SetFrameUpdateTime(auto start, auto end) { frameTimeTaken = ConvertTimeTaken(start, end); }

			double ConvertTimeTaken(auto start, auto end) {
				auto timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
				return timeTaken.count();
			}

			const int byteToMb = 1048576;

			string GetVirtualMemoryUsage() { return std::to_string(usedVirtualMem / byteToMb) + "/" + std::to_string(totalVirtualMem / byteToMb) + "MB"; }
			string GetVirutalUsageByProgram() { return std::to_string(virtualMemUsedByProgram / byteToMb) + "MB"; }
			string GetPhysicalMemoryUsage() { return std::to_string(usedPhysMem / byteToMb) + "/" + std::to_string(totalPhysMem / byteToMb) + "MB"; }
			string GetPhysicalUsgaebyProgram() { return std::to_string(physMemUsedByProgram / byteToMb) + "MB"; }
			string GetFrameTimeTaken() { return std::to_string((int)frameTimeTaken) + " microseconds"; }
			string GetPhysicsTimeTaken() { return std::to_string((int)physicsTimeTaken) + " microseconds"; }
			string GetGraphicsTimnTaken() { return std::to_string((int)graphicsTimeTaken) + " microseconds"; }

			bool GetIsDebugging() { return isDebugging; }

			void ToggleCollisionDisplay();

			void SetGameWorld(ToonGameWorld* world) { this->world = world; }

		protected:
			ToonDebugManager();
			~ToonDebugManager() {}
			static ToonDebugManager* instance;

			void DisplayCollisionBoxes(float dt);
			void CalculateMemoryUsage();
			void CalculateMemoryUsageByProgram();

			DWORDLONG totalVirtualMem;
			DWORDLONG usedVirtualMem;
			SIZE_T virtualMemUsedByProgram;

			DWORDLONG totalPhysMem;
			DWORDLONG usedPhysMem;
			SIZE_T physMemUsedByProgram;

			high_resolution_clock::time_point frameStart;
			high_resolution_clock::time_point frameEnd;

			high_resolution_clock::time_point physicsStart;
			high_resolution_clock::time_point physicsEnd;

			high_resolution_clock::time_point renderingStart;
			high_resolution_clock::time_point renderingEnd;

			ToonGameWorld* world;

			double physicsTimeTaken;
			double graphicsTimeTaken;
			double frameTimeTaken;

			bool isDebugging;
			bool isCollisionDisplayToggled;
			bool isDebugToggled;
		};
	}
}