#include <stdlib.h>
#include <math.h>
#include "Node.h"
namespace pathfinding
{
	Node::Node(int _length, std::pair<int, int> _position, bool _traversable) :
			length(_length), position(_position), traversable(_traversable) 
	{
		
	}

	int Node::Length()
	{
		return this->length;
	}

	bool Node::Traversable()
	{
		return this->traversable;
	}

	std::pair<int, int> Node::Position()
	{
		return this->position;
	}

	double Node::StraightDistance(Node* n)
	{
		int xDistance = abs(this->Position().first - n->Position().first);
		int yDistance = abs(this->Position().second - n->Position().second);

		return sqrt(xDistance^2 + yDistance^2);
	}
}