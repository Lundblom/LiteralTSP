#include <functional>
#include <queue>
#include <vector>
#include <utility> 
#include <stack>

#include "Node.h"

using namespace pathfinding;

class PathStruct
{
private:
	std::vector<std::vector<int> > distance;
	std::vector<std::vector<Node*> > previous;
	std::function<bool(std::pair<Node*, int>, std::pair<Node*, int>)> cmp = [](std::pair<Node*, int> left, std::pair<Node*, int> right) { return left.second > right.second;};
	std::priority_queue< std::pair<Node*, int>, std::vector <std::pair<Node*, int> > , decltype(cmp) > queue;

	std::pair<int, int> start;
	std::pair<int, int> end;

	bool _complete;

public:
	PathStruct(int, std::pair<int, int>, std::pair<int, int>);

	int get_distance(int x, int y);
	void set_distance(int x, int y, int val);

	Node* get_previous(int x, int y);
	void set_previous(int x, int y, Node* val);

	void queue_push(std::pair<Node*, int> p);
	std::pair<Node*, int> queue_pop();
	inline bool queue_is_empty();
	inline bool complete();

	double work();


};