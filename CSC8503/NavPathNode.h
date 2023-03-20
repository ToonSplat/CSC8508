#pragma once

#include "Vector3.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>

using namespace NCL::Maths;
namespace NCL
{
	namespace CSC8503
	{
		class NavPathNode
		{
		public:
			NavPathNode(const int& _id, const Vector3& pos, const bool& jumpNode) : id(_id), position(pos), isJumpNode(jumpNode) {}

			int id;
			Vector3 position;
			bool isJumpNode;
			std::vector<NavPathNode*> neighbours;
		};

		class NavPathGraph
		{
		public:
			/*~NavPathGraph()
			{
				for (auto n : allNodes)
					delete n;
			}*/

			std::unordered_map<int, NavPathNode*> nodes;
			std::vector<NavPathNode*> allNodes;
			std::unordered_map<std::string, NavPathNode*> nodesCache;

			NavPathNode* GetNode(const int& id)
			{
				return nodes[id];
			}

			NavPathNode* GetNode(const Vector3& pos)
			{
				return nodesCache[pos.ToString()];
			}

			void AddNode(const Vector3& pos, const bool& isJumpNode = false)
			{
				static int idCounter = 0;

				NavPathNode* node = new NavPathNode(idCounter++, pos, isJumpNode);

				nodes[idCounter - 1] = node;
				nodesCache[pos.ToString()] = node;
				allNodes.emplace_back(node);
			}

			void AddEdge(const int& fromID, const int& toID)
			{
				NavPathNode* fromNode = GetNode(fromID);
				NavPathNode* toNode = GetNode(toID);

				fromNode->neighbours.emplace_back(toNode);
				toNode->neighbours.emplace_back(fromNode);

				/*nodes[fromID]->neighbours.emplace_back(nodes[toID]);
				nodes[toID]->neighbours.emplace_back(nodes[fromID]);*/
			}

			NavPathNode* GetNearestNode(const Vector3& pos)
			{
				std::vector<NavPathNode*> allNodesTmp(allNodes.begin(), allNodes.end());
				std::sort(allNodesTmp.begin(), allNodesTmp.end(), [&](NavPathNode* a, NavPathNode* b)->bool
					{
						float distA = (pos - a->position).LengthSquared();
						float distB = (pos - b->position).LengthSquared();
						return distA < distB;
					});

				return allNodesTmp[0];
			}

			NavPathNode* GetRandomNode()
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(0, (int)allNodes.size() - 1);

				return allNodes[dis(gen)];
			}
		};
	}
}