#pragma once
#include <utility>

namespace pathfinding
{
	class Node 
	{
	private:
		int length;
		std::pair<int, int> position;
		bool traversable = true;
	public:
		Node(int, std::pair<int, int>, bool);

		int Length();
		std::pair<int, int> Position();
		bool Traversable();
		
		double StraightDistance(Node* n);
	};

}