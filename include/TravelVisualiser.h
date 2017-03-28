#pragma once
#include <thread>
#include <vector>
#include <iostream>

#include "Traveler.h"
#include "Node.h"

namespace pathfinding 
{
	class TravelVisualiser
	{
	private:

		

		int gridSize;
		char* drawGraph;
		std::vector<Traveler*>& travelers;
		std::vector< std::vector< Node* > >& graph;
		std::thread drawThread;
		std::vector<location_t> previous_traveler_locations;
		
	public:

		static int SLEEP_TIME;
		static char TRAVERSABLE_CHAR;
		static char UNTRAVERSABLE_CHAR;

		TravelVisualiser(int, std::vector<Traveler*>& , std::vector< std::vector< Node* > >&);
		~TravelVisualiser();

		void start();
		void draw();
	};
}