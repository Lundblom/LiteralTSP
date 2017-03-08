#include "FibHeap.h"
#include <functional>
#include <queue>
#include <vector>
#include <utility> 
#include <stack>

namespace pathfinding 
{

	#define PATH_INFINITY 1000000

	struct Node
	{
		int length;
		std::pair<int, int> position;
		bool traversable = true;

		Node(int _length, std::pair<int, int> _position, bool _traversable) :
			length(_length), position(_position), traversable(_traversable) {}

	};

	std::stack<Node*> dijkstras(std::vector<std::vector<Node*> >& g, std::pair<int, int> start, std::pair<int, int> end)
	{
		std::vector<std::vector<int> > distance(g.size(), std::vector<int>(g.size(), PATH_INFINITY));
		std::vector<std::vector<Node*> > previous(g.size(), std::vector<Node*>(g.size(), NULL));

		auto cmp = [](std::pair<Node*, int> left, std::pair<Node*, int> right) { return left.second > right.second;};

		std::priority_queue< std::pair<Node*, int>, std::vector <std::pair<Node*, int> > , decltype(cmp) > queue(cmp);

		for(int i = 0; i < g.size(); ++i)
		{
			for(int j = 0; j < g[i].size(); ++j)
			{
				if(i == start.first && j == start.second)
				{
					continue;
				}
				std::pair<Node*, int> node(g[i][j], PATH_INFINITY);
				queue.push(node);
			}
		}

		distance[start.first][start.second] = 0;
		std::pair<Node*, int> startNode(g[start.first][start.second], 0);
		queue.push(startNode);

		while (!queue.empty())
		{
			std::pair<Node*, int> nodePair = queue.top();
			queue.pop();

			Node* u = nodePair.first;

			if(u->position.first == end.first && u->position.second == end.second)
			{
				break;
			}

			std::vector<std::pair<int, int> >locations = {{0,-1}, {0, 1}, {-1, 0}, {1, 0}};

			for(int i = 0; i < locations.size(); ++i)
			{
				int x = locations[i].first + u->position.first;
				int y = locations[i].second + u->position.second;

				if(x < 0 || y < 0 || x >= g.size() || y >= g.size())
				{
					continue;
				}

				Node* v = g[x][y];
				if(!v->traversable)
				{
					continue;
				}
				int a = distance[u->position.first][u->position.second] + v->length;

				if(a < distance[x][y])
				{
					distance[x][y] = a;
					previous[x][y] = u;
					queue.push(std::make_pair(v, distance[x][y]));
				}
			}
		}

		Node* current = g[end.first][end.second];
		std::stack<Node*> path;
		while(current != NULL)
		{
			path.push(current);
			current = previous[current->position.first][current->position.second];
		}

		return path;

	}

}