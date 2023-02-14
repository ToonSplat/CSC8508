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
		class ToonDebugManager
		{
		public:
			ToonDebugManager();
			~ToonDebugManager();

			virtual void Update(float dt);

			void ToggleDebug() { isDebugging = !isDebugging; }

			void SetPhysicsUpdateTime(auto start, auto end) { physicsTimeTaken = ConvertTimeTaken(start, end); }
			void SetGraphicsUpdateTime(auto start, auto end) { graphicsTimeTaken = ConvertTimeTaken(start, end); }
			void SetFrameUpdateTime(auto start, auto end) { frameTimeTaken = ConvertTimeTaken(start, end); }

			double ConvertTimeTaken(auto start, auto end) {
				auto timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
				return timeTaken.count();
			}

			string GetVirtualMemoryUsage() { return std::to_string(usedVirtualMem) + "/" + std::to_string(totalVirtualMem); }
			string GetVirutalUsageByProgram() { return std::to_string(virtualMemUsedByProgram); }
			string GetPhysicalMemoryUsage() { return std::to_string(usedPhysMem) + "/" + std::to_string(totalPhysMem); }
			string GetPhysicalUsgaebyProgram() { return std::to_string(physMemUsedByProgram); }
			string GetFrameTimeTaken() { return std::to_string(trunc(frameTimeTaken)) + " microseconds"; }
			string GetPhysicsTimeTaken() { return std::to_string(trunc(physicsTimeTaken)) + " microseconds"; }
			string GetGraphicsTimnTaken() { return std::to_string(trunc(graphicsTimeTaken)) + " microseconds"; }
			
			bool GetIsDebugging() { return isDebugging; }

			void ToggleCollisionDisplay();

		protected:
			void DisplayCollisionBoxes(float dt);
			void CalculateMemoryUsage();
			void CalculateMemoryUsageByProgram();

			DWORDLONG totalVirtualMem;
			DWORDLONG usedVirtualMem;
			SIZE_T virtualMemUsedByProgram;

			DWORDLONG totalPhysMem;
			DWORDLONG usedPhysMem;
			SIZE_T physMemUsedByProgram;

			double physicsTimeTaken;
			double graphicsTimeTaken;
			double frameTimeTaken;

			bool isDebugging;
			bool isCollisionDisplayToggled;
			bool isDebugToggled;
		};
	}
}