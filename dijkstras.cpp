#include <functional>
#include <queue>
#include <vector>
#include <utility> 
#include <stack>

#include "Node.h"
#include "a_star.cpp"

namespace pathfinding 
{

	#define PATH_INFINITY 1000000

	std::stack<Node*> dijkstras(std::vector<std::vector<Node*> >& g, std::pair<int, int> start, std::pair<int, int> end)
	{
		return a_star(g, start, end, 0);
	}

}