#pragma once
#include <utility>

namespace pathfinding
{
	typedef std::pair<int, int> location_t;

	class Node 
	{	
	private:
		int length;
		location_t position;
		bool traversable = true;
	public:
		Node(int, std::pair<int, int>, bool);

		int Length();
		std::pair<int, int> Position();
		bool Traversable();
		
		double StraightDistance(Node* n);
	};

}