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
		public:
			static std::vector<Vector3> FindPath(NavPathGraph& pathGraph, Vector3& from, Vector3& to)
			{
				NavPathNode* fromNode = pathGraph.GetNode(from);
				NavPathNode* toNode = pathGraph.GetNode(to);

				std::queue<NavPathNode*> q;
				q.push(fromNode);

				std::unordered_map<std::string, Vector3> cameFrom;
				cameFrom[from.ToString()] = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

				while (!q.empty())
				{
					NavPathNode* currentNode = q.front();
					q.pop();
					if (currentNode == toNode)
						break;

					for (NavPathNode* nextNode : currentNode->neighbours)
					{
						if (cameFrom.find(nextNode->position.ToString()) == cameFrom.end())
						{
							q.push(nextNode);
							cameFrom[nextNode->position.ToString()] = currentNode->position;
						}
					}					
				}

				std::vector<Vector3> path;
				NavPathNode* current = toNode;
				Vector3 pos = current->position;
				while (pos != Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX))
				{
					path.emplace_back(current->position);
					if (cameFrom.find(current->position.ToString()) != cameFrom.end())
						pos = cameFrom[current->position.ToString()];
					else
						pos = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
				}

				std::reverse(path.begin(), path.end());

				/*while (current != nullptr)
				{
					path.emplace_back(current->position);
					current = cameFrom[current];
				}
				std::reverse(path.begin(), path.end());*/

				return path;
			}
		};
	}
}