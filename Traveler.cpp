#include <utility> 
#include "Traveler.h"

namespace pathfinding
{
	Traveler::Traveler(std::pair<int, int> start) : position(start)
	{

	}

	bool Traveler::Arrived()
	{
		return this->arrived;
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
		Node* n = this->currentPath.top();
		this->currentPath.pop();
		this->position = n->Position();

		if(this->currentPath.top() == NULL)
		{
			this->arrived = true;
		}
	}

	void Traveler::InitiateTravel(std::stack<Node*> theStack)
	{
		this->arrived = false;
		this->currentPath = theStack;
	}

}