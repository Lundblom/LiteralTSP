#include <semaphore.h>
#include "PathStruct.h"
#include "Node.h"
#include "Traveler.h"

using namespace pathfinding;

#define PATH_INFINITY 1000000
#define PATH_ITERATIONS 20
#define HEURISTIC_COEFFICIENT 10000

static const std::vector<std::pair<int, int> >locations = {{0,-1}, {0, 1}, {-1, 0}, {1, 0}};

PathStruct::PathStruct(int gridSize, std::pair<int, int> start, std::pair<int, int> end, Traveler* t, std::vector< std::vector< Node* > >* graph)
{
	std::vector<std::vector<int> > _distance(gridSize, std::vector<int>(gridSize, PATH_INFINITY));
	std::vector<std::vector<Node*> > _previous(gridSize, std::vector<Node*>(gridSize, NULL));

	std::priority_queue< std::pair<Node*, int>, std::vector <std::pair<Node*, int> > , decltype(cmp) > _queue(cmp);

	distance = _distance;
	previous = _previous;
	queue = _queue;

	this->start = start;
	this->end = end;
	this->traveler = t;
	this->graph = graph;
	this->end_node = (*graph)[end.first][end.second];
}

int PathStruct::get_start_x()
{
	return this->start.first;
}
int PathStruct::get_start_y()
{
	return this->start.second;
}
int PathStruct::get_end_x()
{
	return this->end.first;
}
int PathStruct::get_end_y()
{
	return this->end.second;
}

int PathStruct::get_distance(int x, int y)
{
	return distance[x][y];
}

void PathStruct::set_distance(int x, int y, int val)
{
	distance[x][y] = val;
}

Node* PathStruct::get_previous(int x, int y)
{
	return previous[x][y];
}

void PathStruct::set_previous(int x, int y, Node* val)
{
	previous[x][y] = val;
}

void PathStruct::queue_push(std::pair<Node*, int> p)
{
	queue.push(p);
}

inline bool PathStruct::queue_is_empty()
{
	return queue.empty();
}

std::pair<Node*, int> PathStruct::queue_pop()
{
	std::pair<Node*, int> p = queue.top();
	queue.pop();

	return p;
}

double PathStruct::work()
{
	double wall0 = get_wall_time();

	for(int i = 0; i < PATH_ITERATIONS && !queue.empty(); ++i)
	{
		//Get the node with the lowest distance
		std::pair<Node*, int> nodePair = queue.top();
		queue.pop();

		Node* u = nodePair.first;

		//Checks if we are done
		if(u->Position().first == end.first && u->Position().second == end.second)
		{
			_complete = true;
			break;
		}

		for(int i = 0; i < locations.size(); ++i)
		{
			int x = locations[i].first + u->Position().first;
			int y = locations[i].second + u->Position().second;

			//Bounds check
			if(x < 0 || y < 0 || x >= (*graph).size() || y >= (*graph).size())
			{
				continue;
			}

			Node* v = (*graph)[x][y];
			if(!v->Traversable())
			{
				continue;
			}

			int a = distance[u->Position().first][u->Position().second] + v->Length() + v->StraightDistance(end_node) * HEURISTIC_COEFFICIENT;
			
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

	double wall1 = get_wall_time();

	return (wall1 - wall0);
} 


bool PathStruct::complete()
{
	return _complete;
}