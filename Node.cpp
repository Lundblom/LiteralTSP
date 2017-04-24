#include <stdlib.h>
#include <math.h>
#include "Node.h"
#include <iostream>
namespace pathfinding
{

	int Node::WOOD_LENGTH = 10;
	int Node::WOOD_NODES = 0;
	int Node::WATER_NODES = 0;
	double Node::WOOD_THRESHOLD = 0.2;
	double Node::WATER_THRESHOLD = 0.1;

	Node::Node(int _length, std::pair<int, int> _position, bool _traversable) :
			length(_length), position(_position), traversable(_traversable) 
	{
		
	}

	int Node::Length() const
	{
		return this->length;
	}

	bool Node::Traversable() const
	{
		return this->traversable;
	}

	std::pair<int, int> Node::Position() const
	{
		return this->position;
	}

	void Node::makeRoad()
	{
		if(this->type != NodeType::ROAD && this->type != NodeType::CITY)
		{
			this->type = NodeType::ROAD;
			this->length *= ROAD_COEFFICIENT;
			if(!this->traversable)
			{
				this->traversable = true;
				this->length = 5;
			}
		}
		else
		{
			//Already is road
		}
	}

	void Node::makeCity()
	{
		this->length = 1;
		this->type = NodeType::CITY;
		this->traversable = true;
	}

	void Node::makeWood()
	{
		this->length = WOOD_LENGTH;
		this->type = NodeType::WOOD;
		++WOOD_NODES;
	}

	void Node::makeWater()
	{
		this->traversable = false;
		this->type = NodeType::WATER;
		++WATER_NODES;
	}

	Node::NodeType Node::Type() const
	{
		return this->type;
	}

	double Node::StraightDistance(Node* n)
	{
		int xDistance = abs(this->Position().first - n->Position().first);
		int yDistance = abs(this->Position().second - n->Position().second);

		return sqrt(xDistance^2 + yDistance^2);
	}

	std::ostream& operator<<(std::ostream& os, const Node& n)  
	{  
		location_t l = n.Position();
	    os << "Node x: " << l.first << " y: " << l.second << " length: " << n.Length() << " type: " << n.Type() << std::endl;
	    return os;  
	} 

    int Node::TOTAL_WATER_NODES()
	{
		return Node::WATER_NODES;
	}

	int Node::TOTAL_WOOD_NODES()
	{
		return Node::WOOD_NODES;
	}
}