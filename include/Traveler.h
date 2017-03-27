#pragma once
#include <stdlib.h>
#include <math.h>
#include <utility> 
#include <stack>
#include <thread>
#include "Node.h"

namespace pathfinding
{
	class Traveler
	{
	private:
		std::pair<int, int> position;
		std::stack<Node*> currentPath;
		bool arrived = true;

	public:

		std::thread* travellingThread = NULL;

		Traveler(std::pair<int, int>);
		bool Arrived();
		int NextDistance();
		void Travel();
		void InitiateTravel(std::stack<Node*>);

	};
}