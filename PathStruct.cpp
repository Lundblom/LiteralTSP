#include "PathStruct.h"
#include "Node.h"

using namespace pathfinding;

#define PATH_INFINITY 1000000

PathStruct::PathStruct(int gridSize, std::pair<int, int> start, std::pair<int, int> end)
{
	std::vector<std::vector<int> > _distance(gridSize, std::vector<int>(gridSize, PATH_INFINITY));
	std::vector<std::vector<Node*> > _previous(gridSize, std::vector<Node*>(gridSize, NULL));

	std::priority_queue< std::pair<Node*, int>, std::vector <std::pair<Node*, int> > , decltype(cmp) > _queue(cmp);

	distance = _distance;
	previous = _previous;
	queue = _queue;

	this->start = start;
	this->end = end;
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

inline bool complete()
{
	return _complete;
}