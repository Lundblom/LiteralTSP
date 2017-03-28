#pragma once
#include <stdlib.h>
#include <math.h>
#include <utility> 
#include <stack>
#include <vector>
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
		bool startedTraveling = false;
		int id = 0;
		int inspectedNodes = 0;
		int visitedNodes = 0;

		double computationTime = 0;
		double travelTime = 0;
		std::vector<double> computationHistory;
		std::vector<double> travelHistory;

	public:

		std::thread* travellingThread = NULL;

		Traveler(std::pair<int, int>);
		Traveler(std::pair<int, int>, int);
		~Traveler();
		bool Arrived();
		bool StartedTraveling();
		int NextDistance();
		void Travel();
		void InitiateTravel(std::stack<Node*>);
		void StartTravel();
		int Id();
		std::pair<int, int> Position();

		void InitiatePathCalculation();

		void IncrementComputationTime(double);
		double ComputationTime();
		void IncrementTravelTime(double);
		double TravelTime();

		void IncrementInspectedNodes();
		int InspectedNodes();

		void IncrementVisitedNodes();
		int VisitedNodes();

		void SaveRoundData();
		double GetTotalComputationTime();
		double GetTotalTravelTime();

	};
}