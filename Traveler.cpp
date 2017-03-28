#include <utility> 
#include <iostream>
#include "Traveler.h"

namespace pathfinding
{
	Traveler::Traveler(std::pair<int, int> start) : position(start)
	{

	}

	Traveler::Traveler(std::pair<int, int> start, int id) : position(start)
	{
		this->id = id;
	}

	Traveler::~Traveler()
	{
		delete travellingThread;
	}

	bool Traveler::Arrived()
	{
		return this->arrived;
	}

	bool Traveler::StartedTraveling()
	{
		return this->startedTraveling;
	}
	int Traveler::NextDistance()
	{
		if(this->arrived)
		{
			return -1;
		}
		return this->currentPath.top()->Length();
	}
	void Traveler::Travel()
	{
		if(this->currentPath.empty())
		{
			this->arrived = true;
			return;
		}
		Node* n = this->currentPath.top();
		this->currentPath.pop();
		this->position = n->Position();

		if(this->currentPath.empty())
		{
			this->arrived = true;
		}
	}

	void Traveler::InitiatePathCalculation()
	{
		this->computationTime = 0;
	}

	void Traveler::InitiateTravel(std::stack<Node*> theStack)
	{
		this->arrived = false;
		this->startedTraveling = false;
		this->currentPath = theStack;
		this->travelTime = 0;
	}

	void Traveler::StartTravel()
	{
		this->startedTraveling = true;
	}

	int Traveler::Id()
	{
		return this->id;
	}

	std::pair<int, int> Traveler::Position()
	{
		return this->position;
	}

	void Traveler::IncrementTravelTime(double t)
	{
		this->travelTime += t;
	}

	void Traveler::IncrementComputationTime(double t)
	{
		this->computationTime += t;
	}

	double Traveler::TravelTime()
	{
		return this->travelTime;
	}

	double Traveler::ComputationTime()
	{
		return this->computationTime;
	}

	int Traveler::InspectedNodes()
	{
		return this->inspectedNodes;
	}

	void Traveler::IncrementInspectedNodes()
	{
		++this->inspectedNodes;
	}

	int Traveler::VisitedNodes()
	{
		return this->visitedNodes;
	}

	void Traveler::IncrementVisitedNodes()
	{
		++this->visitedNodes;
	}

	void Traveler::SaveRoundData()
	{
		travelHistory.push_back(travelTime);
		computationHistory.push_back(computationTime);
	}

	double Traveler::GetTotalComputationTime()
	{
		double result = 0;

		for (auto i = computationHistory.begin(); i != computationHistory.end(); ++i)
		{
			result += (*i);
		}

		return result;
	}

	double Traveler::GetTotalTravelTime()
	{
		double result = 0;

		for (auto i = travelHistory.begin(); i != travelHistory.end(); ++i)
		{
			result += (*i);
		}

		return result;
	}

}