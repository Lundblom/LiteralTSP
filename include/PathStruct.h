#pragma once
#include <functional>
#include <queue>
#include <vector>
#include <utility> 
#include <stack>
#include <time.h>
#include <sys/time.h>

#include "Node.h"
#include "Traveler.h"

#define HEURISTIC_COEFFICIENT 0.2

namespace pathfinding
{
	class PathStruct
	{
	private:
		std::vector<std::vector<int> > distance;
		std::vector<std::vector<Node*> > previous;
		std::vector<std::vector<bool> > visited;
		std::function<bool(std::pair<Node*, int>, std::pair<Node*, int>)> cmp = [](std::pair<Node*, int> left, std::pair<Node*, int> right) { return left.second > right.second;};
		std::priority_queue< std::pair<Node*, int>, std::vector <std::pair<Node*, int> > , decltype(cmp) > queue;

		std::vector< std::vector< Node* > >* graph;

		std::pair<int, int> start;
		std::pair<int, int> end;
		Node* end_node;

		int gridSize = 0;
		bool _complete = false;
		
		double get_wall_time(){
	    struct timeval time;
	    if (gettimeofday(&time,NULL)){
	        //  Handle error
	        return 0;
	    }
	    return (double)time.tv_sec + (double)time.tv_usec * .000001;
		}

	public:
		Traveler* traveler;

		PathStruct(int, std::pair<int, int>, std::pair<int, int>, Traveler*, std::vector< std::vector< Node* > >*);

		int get_distance(int x, int y);
		void set_distance(int x, int y, int val);

		Node* get_previous(int x, int y);
		void set_previous(int x, int y, Node* val);

		void queue_push(std::pair<Node*, int> p);
		std::pair<Node*, int> queue_pop();
		inline bool queue_is_empty();
		bool complete();



		int get_start_x();
		int get_start_y();
		int get_end_x();
		int get_end_y();

		double work();


	};
}