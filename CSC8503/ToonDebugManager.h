#pragma once
#include "windows.h"
#include "psapi.h"
#include "Debug.h"

#include "ToonGameObject.h"
#include "Window.h"
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

			void Update();

			enum measuring{load, frame, audio, networking, physics, animation, rendering};

			void StartTimeCount(measuring m);
			void EndTimeCount(measuring m);

			string ConvertTimeTaken(high_resolution_clock::time_point start, high_resolution_clock::time_point end);

			const int byteToMb = 1048576;

			string GetVirtualMemoryUsage() { return std::to_string(usedVirtualMem / byteToMb) + "/" + std::to_string(totalVirtualMem / byteToMb) + "MB"; }
			string GetVirutalUsageByProgram() { return std::to_string(virtualMemUsedByProgram / byteToMb) + "MB"; }
			string GetPhysicalMemoryUsage() { return std::to_string(usedPhysMem / byteToMb) + "/" + std::to_string(totalPhysMem / byteToMb) + "MB"; }
			string GetPhysicalUsagebyProgram() { return std::to_string(physMemUsedByProgram / byteToMb) + "MB"; }

			string GetTimeTaken(measuring m);

			void ToggleCollisionDisplay();

			void SetGameWorld(ToonGameWorld* world) { this->world = world; }

		protected:
			ToonDebugManager();
			~ToonDebugManager() {}
			static ToonDebugManager* instance;

			void DisplayCollisionBoxes();
			void CalculateMemoryUsage();
			void CalculateMemoryUsageByProgram();

			DWORDLONG totalVirtualMem;
			DWORDLONG usedVirtualMem;
			SIZE_T virtualMemUsedByProgram;

			DWORDLONG totalPhysMem;
			DWORDLONG usedPhysMem;
			SIZE_T physMemUsedByProgram;

			high_resolution_clock::time_point loadStart;
			high_resolution_clock::time_point frameStart;
			high_resolution_clock::time_point audioStart;
			high_resolution_clock::time_point networkingStart;
			high_resolution_clock::time_point physicsStart;
			high_resolution_clock::time_point animationStart;
			high_resolution_clock::time_point renderingStart;

			ToonGameWorld* world;

			string loadTimeTaken;
			string frameTimeTaken;
			string audioTimeTaken;
			string networkingTimeTaken;
			string physicsTimeTaken;
			string animationTimeTaken;
			string graphicsTimeTaken;

			bool isCollisionDisplayToggled;
		};
	}
}