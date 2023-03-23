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
		class ToonDebugManager{
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

			enum measuring { load, frame, audio, networking, physics, animation, rendering};

			void StartTimeCount(measuring m);
			void EndTimeCount(measuring m);

			string ConvertTimeTaken(high_resolution_clock::time_point start, high_resolution_clock::time_point end);

			const int byteToMb = 1048576;
			string ConvertMemoryUsage(auto a) {return std::to_string(a / byteToMb) + " MB"; }



      string GetVirtualMemoryUsage() { return ConvertMemoryUsage(usedVirtualMem); }
			string GetVirutalUsageByProgram() { return ConvertMemoryUsage(virtualMemUsedByProgram); }
			string GetTotalVirtualMemory(){ return ConvertMemoryUsage(totalVirtualMem); }
			string GetPhysicalMemoryUsage() { return ConvertMemoryUsage(usedPhysMem); }
			string GetPhysicalUsagebyProgram() { return ConvertMemoryUsage(physMemUsedByProgram); }
			string GetTotalPhysicalMemory() { return ConvertMemoryUsage(totalPhysMem); }

      bool isAIPresent;

			string GetTimeTaken(measuring m);

			void ToggleCollisionDisplay();
			bool GetAIPathGraphStatus() const { return showAIPathGraph; }
			bool GetAIPathDebugStatus() const { return showAIPathDebug; }

			void SetAIPathGraphStatus(bool s) { showAIPathGraph = s; }
			void SetAIPathDebugStatus(bool s) { showAIPathDebug = s; }

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
			
			bool showAIPathGraph;
			bool showAIPathDebug;
		};
	}
}