#include "NavPathGraphLevel.h"
#include "Debug.h"

using namespace NCL::CSC8503;

NCL::CSC8503::NavPathGraphLevel::NavPathGraphLevel()
{
	AddNode(Vector3(0.0f, -8.0f, 0.0f));		//0
	AddNode(Vector3(14.0f, -8.0f, -12.0f));		//1
	AddNode(Vector3(37.0f, 4.0f, -13.0f));		//2
	AddNode(Vector3(44.0f, 12.0f, 0.0f));		//3
	AddNode(Vector3(37.0f, 4.0f, 13.0f));		//4
	AddNode(Vector3(14.0f, -8.0f, 12.0f));		//5

	AddNode(Vector3(-14.0f, -8.0f, -12.0f));	//6
	AddNode(Vector3(-37.0f, 4.0f, -13.0f));		//7
	AddNode(Vector3(-44.0f, 12.0f, 0.0f));		//8
	AddNode(Vector3(-37.0f, 4.0f, 13.0f));		//9
	AddNode(Vector3(-14.0f, -8.0f, 12.0f));		//10

	AddNode(Vector3(25.5f, -4.0f, -24.0f));		//11
	AddNode(Vector3(0.0f, -4.0f, -30.0f));		//12
	AddNode(Vector3(-25.5f, -4.0f, -24.0f));	//13

	AddNode(Vector3(25.5f, -4.0f, 24.0f));		//14
	AddNode(Vector3(0.0f, -4.0f, 30.0f));		//15
	AddNode(Vector3(-25.0f, -4.0f, 24.0f));		//16

	AddEdge(0, 1);
	AddEdge(1, 2);
	AddEdge(2, 3);
	AddEdge(3, 4);
	AddEdge(4, 5);
	AddEdge(5, 0);

	AddEdge(0, 6);
	AddEdge(6, 7);
	AddEdge(7, 8);
	AddEdge(8, 9);
	AddEdge(9, 10);
	AddEdge(10, 0);

	AddEdge(1, 11);
	AddEdge(11, 12);
	AddEdge(12, 13);
	AddEdge(13, 6);

	AddEdge(5, 14);
	AddEdge(14, 15);
	AddEdge(15, 16);
	AddEdge(16, 10);

	AddEdge(1, 5);
	AddEdge(1, 6);
	AddEdge(10, 5);
	AddEdge(10, 6);
}

void NCL::CSC8503::NavPathGraphLevel::DrawDebugPath()
{
	for (auto& [id, node] : nodes)
	{
		Debug::DrawBox(node->position, Vector3(0.3f, 0.3f, 0.3f), Debug::CYAN);
		for (auto& neighbour : node->neighbours)
		{
			Debug::DrawLine(node->position, neighbour->position, Debug::CYAN);
		}
	}
}
