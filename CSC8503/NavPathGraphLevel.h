#pragma once
#include "NavPathNode.h"

namespace NCL
{
	namespace CSC8503
	{
		class NavPathGraphLevel : public NavPathGraph
		{
		public:
			NavPathGraphLevel();
			~NavPathGraphLevel() {};

			void DrawDebugPathGraph();
		};
	}
}