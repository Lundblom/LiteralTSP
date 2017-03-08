#include <iostream>
#include "dijkstras.cpp"

using namespace pathfinding;

#define GRID_SIZE 8
#define STARTX 0
#define STARTY 0
#define ENDX 7
#define ENDY 7

int main()
{
	std::vector< std::vector< Node* > > graph(GRID_SIZE, std::vector<Node*>(GRID_SIZE, NULL));

	bool traversableArr[] = {
		0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,1,1,1,
		0,0,0,0,0,1,0,0,
		0,0,0,1,1,0,0,0,
		0,0,0,1,0,0,1,0,
		0,0,0,0,0,0,1,0
	};

	int distanceArr[] = {
		1,1,2,3,1,1,1,3,
		1,1,1,1,1,1,1,2,
		1,1,1,1,1,1,3,1,
		1,1,1,10,10,1,1,1,
		1,3,1,10,10,1,1,1,
		1,1,3,1,1,1,3,1,
		1,1,6,1,1,1,1,3,
		1,1,1,1,1,4,1,3
	};


	for(int i = 0; i < GRID_SIZE; ++i)
	{
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			graph[i][j] = new Node(distanceArr[i * GRID_SIZE + j], std::make_pair(i,j), !traversableArr[i * GRID_SIZE + j]);
		}
	}

	std::stack<Node*> result = dijkstras(graph, std::make_pair(STARTX,STARTY), std::make_pair(ENDX, ENDY));

	bool pathArr[GRID_SIZE * GRID_SIZE];

	for(int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
	{
		pathArr[i] = 0;
	}

	while(!result.empty())
	{
		Node* n = result.top();
		result.pop();

		pathArr[n->position.first * GRID_SIZE + n->position.second] = 1;
	}

	std::cout << "Start (" << STARTX << ", " <<  STARTY << ")" << std::endl;
	std::cout << "End (" << ENDX << ", " <<  ENDY << ")" << std::endl;

	std::cout << "Grid";

	for(int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
	{
		if(i % GRID_SIZE == 0)
		{
			std::cout << std::endl;
		}
		std::cout << (traversableArr[i] ? "X" : "o") << " ";

	}
	std::cout << std::endl << "Path";
	for(int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
	{
		if(i % GRID_SIZE == 0)
		{
			std::cout << std::endl;
		}
		std::cout << (pathArr[i] ? "X" : "o") << " ";

	}

	return 1;
}