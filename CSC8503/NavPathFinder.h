#pragma once
#include "NavPathNode.h"
#include <queue>

using namespace NCL::Maths;
namespace NCL
{
	namespace CSC8503
	{
		class NavPathFinder
		{
			static std::vector<Vector3> FindPath(NavPathGraph& pathGraph, const Vector3& from, const Vector3& to)
			{
				std::queue<Vector3> q;
				q.push(from);

				std::unordered_map<Vector3, Vector3> cameFrom;
				cameFrom[from] = Vector3(-1.0f, -1.0f, -1.0f);

				while (!q.empty())
				{
					Vector3 current = q.front();
					q.pop();
					if (current == to)
						break;

					for (Vector3 next : pathGraph.nodes[current]->neighbours)
					{
						if (cameFrom.find(next) == cameFrom.end())
						{
							q.push(next);
							cameFrom[next] = current;
						}
					}					
				}

				std::vector<Vector3> path;
				Vector3 current = to;
				while (current != Vector3(-1.0f, -1.0f, -1.0f))
				{
					path.emplace_back(current);
					current = cameFrom[current];
				}
				std::reverse(path.begin(), path.end());

				return path;
			}
		};
	}
}