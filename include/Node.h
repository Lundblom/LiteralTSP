#pragma once
#include <utility>
#include <iostream>

namespace pathfinding
{
	typedef std::pair<int, int> location_t;

	#define ROAD_COEFFICIENT 0.2

	

	class Node 
	{
	public:
		enum NodeType
		{
			NORMAL,
			CITY,
			ROAD,
			WOOD,
			MOUNTAIN,
			WATER
		};
	private:
		
		static int WOOD_LENGTH;
		int length;
		location_t position;
		bool traversable = true;
		NodeType type = NodeType::NORMAL;

	public:
		

		Node(int, std::pair<int, int>, bool);

		int Length() const;
		std::pair<int, int> Position() const;
		bool Traversable() const;

		NodeType Type() const;
		
		double StraightDistance(Node* n);
		void makeRoad();
		void makeWood();
		void makeWater();
		void makeCity();

		friend std::ostream& operator<< (std::ostream&, const Node&);
	};

}