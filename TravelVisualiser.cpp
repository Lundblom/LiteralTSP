#include "TravelVisualiser.h"

int pathfinding::TravelVisualiser::SLEEP_TIME = 1000;
char pathfinding::TravelVisualiser::TRAVERSABLE_CHAR = 'o';
char pathfinding::TravelVisualiser::UNTRAVERSABLE_CHAR = 'x';

namespace pathfinding 
{

	

	TravelVisualiser::TravelVisualiser(int _gridSize, 
		std::vector<Traveler*>&  _travelers, std::vector< std::vector< Node* > >& _graph) 
		: gridSize(_gridSize), travelers(_travelers), graph(_graph)
	{
		drawGraph = new char[gridSize * gridSize];
		previous_traveler_locations = std::vector<location_t>(_travelers.size());
	}

	TravelVisualiser::~TravelVisualiser()
	{

	}

	void TravelVisualiser::draw()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));
		for(int i = 0; i < previous_traveler_locations.size(); ++i)
		{

			char charToDraw = (graph[previous_traveler_locations[i].first]
				[previous_traveler_locations[i].second]->Traversable() ?
				TRAVERSABLE_CHAR : UNTRAVERSABLE_CHAR);

			drawGraph[previous_traveler_locations[i].first + 
				gridSize * previous_traveler_locations[i].second] = charToDraw;
		}
		for(int i = 0; i < travelers.size(); ++i)
		{
			previous_traveler_locations[i] = travelers[i]->Position();
			drawGraph[previous_traveler_locations[i].first + gridSize * previous_traveler_locations[i].second]
			 	= travelers[i]->Id();
		}

		for(int i = 0; i < gridSize * gridSize; ++i)
		{
			if(i % gridSize == 0 && i != 0)
			{
				std::cerr << std::endl;
				std::cerr << drawGraph[i];
			}
		}

	}

	void TravelVisualiser::start()
	{
		drawThread = std::thread(&TravelVisualiser::draw, this);
	}
}