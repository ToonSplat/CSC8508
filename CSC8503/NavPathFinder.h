#pragma once
#include "NavPathNode.h"
#include <queue>


namespace NCL
{
	namespace CSC8503
	{
		class NavPathFinder
		{
		public:
			static std::vector<Vector3> FindPath(NavPathGraph& pathGraph, const Vector3& from, const Vector3& to);
		};
	}
}