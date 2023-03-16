#pragma once

#include "Vector3.h"
#include <vector>
#include <unordered_map>

using namespace NCL::Maths;
namespace NCL
{
	namespace CSC8503
	{
		class NavPathNode
		{
		public:
			NavPathNode(const int& _id, const Vector3& pos) : id(_id), position(pos) {}

			int id;
			Vector3 position;
			std::vector<NavPathNode*> neighbours;
		};

		class NavPathGraph
		{
		public:
			std::unordered_map<int, NavPathNode*> nodes;
			std::unordered_map<std::string, NavPathNode*> nodesCache;

			NavPathNode* GetNode(const int& id)
			{
				return nodes[id];
			}

			NavPathNode* GetNode(const Vector3& pos)
			{
				return nodesCache[pos.ToString()];
			}

			void AddNode(const Vector3& pos)
			{
				static int idCounter = 0;

				NavPathNode* node = new NavPathNode(idCounter++, pos);

				nodes[idCounter - 1] = node;
				nodesCache[pos.ToString()] = node;
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
		};
	}
}