#pragma once
#include <utility>

namespace pathfinding
{
	typedef std::pair<int, int> location_t;

	#define ROAD_COEFFICIENT 0.2

	class Node 
	{	
	private:
		enum NodeType
		{
			NORMAL,
			CITY,
			ROAD,
			FOREST,
			MOUNTAIN
		};

		int length;
		location_t position;
		bool traversable = true;
		NodeType type = NodeType::NORMAL;

	public:
		Node(int, std::pair<int, int>, bool);

		int Length();
		std::pair<int, int> Position();
		bool Traversable();

		NodeType Type() const;
		
		double StraightDistance(Node* n);
		void makeRoad();
	};

}