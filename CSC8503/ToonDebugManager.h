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

			void StartLoad();
			void EndLoad();

			void StartFrame();
			void EndFrame();

			void StartNetworking();
			void EndNetworking();

			void StartPhysics();
			void EndPhysics();

			void StartAnimation();
			void EndAnimation();

			void StartRendering();
			void EndRendering();

			double ConvertTimeTaken(high_resolution_clock::time_point start, high_resolution_clock::time_point end) {
				std::chrono::microseconds timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
				return (double)timeTaken.count();
			}

			const int byteToMb = 1048576;

			string GetVirtualMemoryUsage() { return std::to_string(usedVirtualMem / byteToMb) + "/" + std::to_string(totalVirtualMem / byteToMb) + "MB"; }
			string GetVirutalUsageByProgram() { return std::to_string(virtualMemUsedByProgram / byteToMb) + "MB"; }
			string GetPhysicalMemoryUsage() { return std::to_string(usedPhysMem / byteToMb) + "/" + std::to_string(totalPhysMem / byteToMb) + "MB"; }
			string GetPhysicalUsagebyProgram() { return std::to_string(physMemUsedByProgram / byteToMb) + "MB"; }
			string GetLoadTimeTaken() { return std::to_string(loadTimeTaken / 1000.0f) + " ms"; }
			string GetFrameTimeTaken() { return std::to_string(frameTimeTaken / 1000.0f) + " ms"; }
			string GetNetworkingTimeTaken() { return std::to_string(networkingTimeTaken / 1000.0f) + " ms"; }
			string GetPhysicsTimeTaken() { return std::to_string(physicsTimeTaken / 1000.0f) + " ms"; }
			string GetAnimationTimeTaken() { return std::to_string(animationTimeTaken / 1000.0f) + " ms"; }
			string GetGraphicsTimeTaken() { return std::to_string(graphicsTimeTaken / 1000.0f) + " ms"; }

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
			high_resolution_clock::time_point loadEnd;

			high_resolution_clock::time_point frameStart;
			high_resolution_clock::time_point frameEnd;

			high_resolution_clock::time_point networkingStart;
			high_resolution_clock::time_point networkingEnd;

			high_resolution_clock::time_point physicsStart;
			high_resolution_clock::time_point physicsEnd;

			high_resolution_clock::time_point animationStart;
			high_resolution_clock::time_point animationEnd;

			high_resolution_clock::time_point renderingStart;
			high_resolution_clock::time_point renderingEnd;

			ToonGameWorld* world;

			double loadTimeTaken;
			double frameTimeTaken;
			double networkingTimeTaken;
			double physicsTimeTaken;
			double animationTimeTaken;
			double graphicsTimeTaken;

			bool isCollisionDisplayToggled;
		};
	}
}