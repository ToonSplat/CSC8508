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

			void ToggleDebug() {
				isDebugging = !isDebugging;
				std::cout << isDebugging << std::endl;
			}

			void SetPhysicsUpdateTime(auto start, auto end) {
				physicsTimeTaken = ConvertTimeTaken(start, end);
			}

			void SetGraphicsUpdateTime(auto start, auto end) {
				graphicsTimeTaken = ConvertTimeTaken(start, end);
			}

			void SetFrameUpdateTime(auto start, auto end) {
				frameTimeTaken = ConvertTimeTaken(start, end);
			}

			double ConvertTimeTaken(auto start, auto end) {
				auto timeTaken = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
				return timeTaken.count();
			}

		protected:
			void DisplayCollisionBoxes(float dt);
			void CalculateMemoryUsage();
			void CalculateMemoryUsageByThis();

			DWORDLONG totalVirtualMem;
			DWORDLONG usedVirtualMem;
			SIZE_T virtualMemUsedByMe;

			DWORDLONG totalPhysMem;
			DWORDLONG usedPhysMem;
			SIZE_T physMemUsedByMe;

			double physicsTimeTaken;
			double graphicsTimeTaken;
			double frameTimeTaken;

			bool isDebugging;
			bool isDebugToggled;
		};
	}
}