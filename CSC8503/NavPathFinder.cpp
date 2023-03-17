#include "NavPathFinder.h"
//#include <chrono>

using namespace NCL::Maths;
//using namespace std::chrono;

std::vector<Vector3> NCL::CSC8503::NavPathFinder::FindPath(NavPathGraph& pathGraph, const Vector3& from, const Vector3& to)
{
	/*NavPathNode* fromNode = pathGraph.GetNode(from);
	NavPathNode* toNode = pathGraph.GetNode(to);*/

	NavPathNode* fromNode = pathGraph.GetNearestNode(from);
	NavPathNode* toNode = pathGraph.GetNearestNode(to);

	std::queue<NavPathNode*> q;
	q.push(fromNode);

	std::vector<bool> visited;
	std::vector<int> dist;
	std::vector<int> pred;

	visited.resize(pathGraph.nodes.size(), false);
	dist.resize(pathGraph.nodes.size(), INT_MAX);
	pred.resize(pathGraph.nodes.size(), -1);

	visited[fromNode->id] = true;
	dist[fromNode->id] = 0;

	//auto startTime = high_resolution_clock::now();
	while (!q.empty())
	{
		NavPathNode* currentNode = q.front();
		//std::cout << "Node: " << currentNode->id << " | " << currentNode->position << std::endl;
		q.pop();

		for (NavPathNode* neighbourNode : currentNode->neighbours)
		{
			if (!visited[neighbourNode->id])
			{
				visited[neighbourNode->id] = true;
				dist[neighbourNode->id] = dist[currentNode->id] + 1;
				pred[neighbourNode->id] = currentNode->id;				

				q.push(neighbourNode);

				if (neighbourNode == toNode)
					break;
			}
		}
	}
#pragma region OLD
	/*std::unordered_map<std::string, Vector3> cameFrom;
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
}*/

/*NavPathNode* current = toNode;
Vector3 pos = current->position;
while (pos != Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX))
{
	path.emplace_back(current->position);
	if (cameFrom.find(current->position.ToString()) != cameFrom.end())
		pos = cameFrom[current->position.ToString()];
	else
		pos = Vector3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
}

std::reverse(path.begin(), path.end());*/

/*while (current != nullptr)
{
	path.emplace_back(current->position);
	current = cameFrom[current];
}
std::reverse(path.begin(), path.end());*/
#pragma endregion

	std::vector<Vector3> path;
	int crawl = toNode->id;
	path.emplace_back(pathGraph.nodes[crawl]->position);
	while (pred[crawl] != -1)
	{
		path.emplace_back(pathGraph.nodes[pred[crawl]]->position);
		crawl = pred[crawl];
	}

	std::reverse(path.begin(), path.end());
	//auto stop = high_resolution_clock::now();
	//auto duration = duration_cast<seconds>(stop - startTime);
	//std::cout << "BFS Path Finding Duration: " << duration << std::endl;

	return path;
}
