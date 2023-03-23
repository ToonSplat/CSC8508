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
		struct CodeSectionStats {
			high_resolution_clock::time_point start;
			high_resolution_clock::time_point end;
			string timeTaken;
		};

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

			void StartTimeCount(string section);
			void EndTimeCount(string section);

			const int byteToMb = 1048576;
			string ConvertMemoryUsage(auto a) {return std::to_string(a / byteToMb) + " MB"; }

			string GetVirtualMemoryUsage() { return ConvertMemoryUsage(usedVirtualMem); }
			string GetVirutalUsageByProgram() { return ConvertMemoryUsage(virtualMemUsedByProgram); }
			string GetTotalVirtualMemory(){ return ConvertMemoryUsage(totalVirtualMem); }
			string GetPhysicalMemoryUsage() { return ConvertMemoryUsage(usedPhysMem); }
			string GetPhysicalUsagebyProgram() { return ConvertMemoryUsage(physMemUsedByProgram); }
			string GetTotalPhysicalMemory() { return ConvertMemoryUsage(totalPhysMem); }

			bool isAIPresent;

			string GetTimeTaken(string section);

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

			void ConvertTimeTaken(CodeSectionStats& section);

			void DisplayCollisionBoxes();
			void CalculateMemoryUsage();
			void CalculateMemoryUsageByProgram();

			DWORDLONG totalVirtualMem;
			DWORDLONG usedVirtualMem;
			SIZE_T virtualMemUsedByProgram;

			DWORDLONG totalPhysMem;
			DWORDLONG usedPhysMem;
			SIZE_T physMemUsedByProgram;

			std::map<string, CodeSectionStats> sectionStats;

			ToonGameWorld* world;

			bool isCollisionDisplayToggled;
			
			bool showAIPathGraph;
			bool showAIPathDebug;
		};
	}
}