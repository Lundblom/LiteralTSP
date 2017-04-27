#include <functional>
#include <queue>
#include <vector>
#include <utility> 
#include <stack>
#include <iostream>

#include "Node.h"

namespace pathfinding 
{

	#define ASTAR_PATH_INFINITY 2147483647
	#define SQUARE_ROOT_TWO 1.414213562373095048801688724209698078569671875376948073176

	//All location offsets from the current node that we check
	static const std::vector<std::pair<int, int> > locations = {{0,-1}, {0, 1}, {-1, 0}, {1, 0}};
	static const std::vector<std::pair<int, int> > diagonal_locations = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};

	std::stack<Node*> a_star(std::vector<std::vector<Node*> >& g, std::pair<int, int> start, std::pair<int, int> end, double heuristic_coefficient, bool road_generate)
	{
		std::vector<std::vector<int> > distance(g.size(), std::vector<int>(g.size(), ASTAR_PATH_INFINITY));
		std::vector<std::vector<Node*> > previous(g.size(), std::vector<Node*>(g.size(), NULL));
		std::vector<std::vector<bool> > visited(g.size(), std::vector<bool>(g.size(), 0));

		Node* end_node = g[end.first][end.second];

		if(heuristic_coefficient < 0)
		{
			heuristic_coefficient = 0;
		}

		//Tells the queue how to sort the elements
		auto cmp = [](std::pair<Node*, int> left, std::pair<Node*, int> right) { return left.second > right.second;};

		//Creates a queue containing a Node* and the distance to it (int)
		//This is wrapped in a vector
		//The cmp lambda is used to sort the queue
		std::priority_queue< std::pair<Node*, int>, std::vector <std::pair<Node*, int> > , decltype(cmp) > queue(cmp);

		//Queue init
		for(int i = 0; i < g.size(); ++i)
		{
			for(int j = 0; j < g[i].size(); ++j)
			{
				//We dont add the first element yet
				if(i == start.first && j == start.second)
				{
					continue;
				}
				//Adds a node with infinity (unknown) distance
				std::pair<Node*, int> node(g[i][j], ASTAR_PATH_INFINITY);
				queue.push(node);
			}
		}

		//Set distance of start to 0
		distance[start.first][start.second] = 0;
		std::clog << "start in a_star: " << (*g[start.first][start.second]);
		std::pair<Node*, int> startNode(g[start.first][start.second], 0);
		queue.push(startNode);

		while (!queue.empty())
		{
			//Get the node with the lowest distance
			std::pair<Node*, int> nodePair = queue.top();
			queue.pop();

			Node* u = nodePair.first;

			//Checks if we are done
			if(u->Position().first == end.first && u->Position().second == end.second)
			{
				break;
			}

			for(int i = 0; i < locations.size() + diagonal_locations.size(); ++i)
			{
				int x;
				int y;  

				double lengthMultiplier = 1;

				if(i < locations.size())
				{
					x = locations[i].first + u->Position().first;
					y = locations[i].second + u->Position().second;
				}
				else
				{
					x = diagonal_locations[i-locations.size()].first + u->Position().first;
					y = diagonal_locations[i-locations.size()].second + u->Position().second;
					lengthMultiplier = SQUARE_ROOT_TWO;
				}

				//Bounds check
				if(x < 0 || y < 0 || x >= g.size() || y >= g.size())
				{
					continue;
				}

				if(visited[x][y])
				{
					continue;
				}
				visited[x][y] = 1;

				Node* v = g[x][y];
				if(!v->Traversable() && !road_generate)
				{
					continue;
				}

				int l = v->Length();
				if(road_generate)
				{
					if(l == 0)
					{
						l = 30;
					}
					else
					{
						if(v->Type() == Node::NodeType::ROAD)
						{
							l *= 1;
						}
						else if(v->Type() == Node::NodeType::WOOD)
						{
							l *= 5000;
						}
						else if(v->Type() == Node::NodeType::WATER)
						{
							l *= 40000;
						}
					}
				}

				int a = distance[u->Position().first][u->Position().second] + l * lengthMultiplier + v->StraightDistance(end_node) * heuristic_coefficient;

				/*std::clog << "a value: " << a << std::endl;
				std::clog << "distance: " << distance[u->Position().first][u->Position().second] << std::endl;
				std::clog << "l * lengthMultiplier: " << l * lengthMultiplier << std::endl;
				std::clog << "StraightDistance: " << v->StraightDistance(end_node) * heuristic_coefficient << std::endl;*/
				
				//If this path is shorter than the one we are currently on
				if(a < distance[x][y])
				{
					//Sets the new total path distance
					distance[x][y] = a;
					//Update our path trace, set the node we came fram
					previous[x][y] = u;
					//Push the new node with the new distance set
					queue.push(std::make_pair(v, distance[x][y]));
				}
			}
		}

		//Creates a stack with our path in it
		//Stack is used for easy backtracking
		Node* last = NULL;
		Node* current = g[end.first][end.second];
		std::stack<Node*> path;
		bool loop = false;
		while(current != NULL)
		{
			path.push(current);
			Node* cacheN = current;
			current = previous[current->Position().first][current->Position().second];
			if(last == current)
			{
				Node* startNode = g[start.first][start.second];
				Node* endNode = g[end.first][end.second];
				std::cout << "Found infinite loop, breaking...\n";
				std::cout << "Was going from " << (*startNode) << " to " << (*endNode) << std::endl;
				std::cout << "Got stuck on " << (*current) << std::endl;
				loop = true;
				break;
			}
			last = cacheN;
		}


		if(loop)
		{
			std::cout << "Lets trace the path!" << std::endl;
			std::cout << "start is " << (*g[start.first][start.second]);
			std::cout << "end is " << (*g[end.first][end.second]);
			std::stack<Node*> pathC = path;
			while(!pathC.empty())
			{
				Node* n = pathC.top();
				pathC.pop();
				std::cout << (*n);
			}
			exit(0);
		}

		return path;
	}
}