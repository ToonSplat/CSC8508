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
			NavPathNode(const Vector3& pos) : position(pos) {}

			Vector3 position;
			std::vector<Vector3> neighbours;
		};

		class NavPathGraph
		{
		public:
			std::unordered_map<Vector3, NavPathNode*> nodes;

			void AddNode(const Vector3& pos)
			{
				NavPathNode* node = new NavPathNode(pos);
				nodes[pos] = node;
			}

			void AddEdge(const Vector3& from, const Vector3& to)
			{
				nodes[from]->neighbours.emplace_back(to);
				nodes[to]->neighbours.emplace_back(from);
			}
		};
	}
}