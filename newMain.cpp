#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <stdio.h>
#include <semaphore.h>
#include <cstdlib>
#include <math.h>
#include <stdexcept>
#include "Node.h"
#include "Traveler.h"
#include "NodeCalculatorThread.h"
#include "PathStruct.h"

#include "a_star.cpp"

#define TRAVEL_COEFFICIENT 100
#define AMOUNT_OF_CITIES 10
#define AMOUNT_OF_WOODS 5
#define AMOUNT_OF_WATER 1
#define CITY_HEURISTIC_COEFFICIENT 200000

sem_t print_sem;
sem_t traveler_count_sem;
int traveler_count = 0;

int GRIDSIZE = 0;

using namespace pathfinding;



void travel_handler(Traveler* t, NodeCalculatorThread* nct, std::vector<location_t>* targets, unsigned int* targetIndex)
{
	while(!t->Arrived())
	{
		int distance = t->NextDistance();
		if(NodeCalculatorThread::SIMULATION_MODE)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(distance * TRAVEL_COEFFICIENT));
		}
		t->IncrementTravelTime( distance );
		t->Travel();
	}
	sem_wait(&print_sem);
	std::cerr << "traveler " << t->Id() << " ocmp t: " << t->ComputationTime() << 
	", travel t: " << t->TravelTime() << ", inspected: " << t->InspectedNodes() << ", visited: " << t->VisitedNodes() << std::endl;
	sem_post(&print_sem);

	t->SaveRoundData();

	++(*targetIndex);
	if( (*targetIndex) < (*targets).size() )
	{
		nct->assign_new_task(t, t->Position(), (*targets)[(*targetIndex)]);
	}
	else
	{
		sem_wait(&traveler_count_sem);
		--traveler_count;
		std::cerr << "Traveler " << t->Id() << " COMPLETE!" << std::endl;
		if(traveler_count == 0)
		{
			sem_post(&NodeCalculatorThread::THREAD_SEM);
		}
		sem_post(&traveler_count_sem);

	}
}

void generate_cities(std::vector<location_t>& city_coordinates, std::vector<std::vector<Node*> >& g)
{
	while(true)
	{
		int firstIndex = rand() % city_coordinates.size();
		location_t firstLocation = city_coordinates[firstIndex];
		city_coordinates.erase(city_coordinates.begin() + firstIndex);
		g[firstLocation.first][firstLocation.second]->makeCity();
		if(city_coordinates.empty())
		{
			break;
		}
		int secondIndex = rand() % city_coordinates.size();
		location_t secondLocation = city_coordinates[secondIndex];
		g[secondLocation.first][secondLocation.second]->makeCity();

		std::stack<Node*> road = a_star(g, firstLocation, secondLocation, CITY_HEURISTIC_COEFFICIENT, true);


		while(!road.empty())
		{
			Node* n = road.top();
			road.pop();
			n->makeRoad();
		}
	}
}

//Probability function for terrain is
// e^(-x/d)
bool terrain_roll(int step, int divider)
{
	int roll = rand() % 10000;
	return roll <= 10000 * exp(-step/divider);
}

//Recursive function FUCK YEAH MAN
void generate_terrain_area(Node* n, std::vector<std::vector<Node*> >& g, int step, Node::NodeType type, int divider)
{
	if(type == Node::NodeType::WOOD)
	{
		n->makeWood();
	}
	else if(type == Node::NodeType::WATER)
	{
		n->makeWater();
	}
	else
	{
		throw std::runtime_error("ERROR invalid nodetype sent to generate_terrain_area");
	}
	
	location_t loc = n->Position();

	for(int i = -1; i <= 1; ++i)
	{
		for(int j = -1; j <= 1; ++j)
		{
			int x = loc.first + i;
			int y = loc.second + j;
			if(x < 0 || x >= GRIDSIZE || y < 0 || y >= GRIDSIZE)
			{
				continue;
			}
			Node* neighbour = g[x][y];
			if(neighbour->Type() == type)
			{
				continue;
			}
			if(terrain_roll(step, divider))
			{
				generate_terrain_area(neighbour, g, step+1, type, divider);
			}
		}
	}
}

void generate_terrain_areas(std::vector<location_t>& wood_coordinates, std::vector<std::vector<Node*> >& g, Node::NodeType type, int divider)
{
	for (std::vector<location_t>::iterator i = wood_coordinates.begin(); i != wood_coordinates.end(); ++i)
	{
		generate_terrain_area(g[i->first][i->second], g, 0, type, divider);
	}
}



int main(int argc, char** argv)
{
	int vertices;
	int gridSize;
	std::vector< std::vector< Node* > > graph;
	std::vector<location_t> targets; 
	
	srand(time(0));

	std::cin >> vertices;
	std::cin >> gridSize;

	GRIDSIZE = gridSize;

	graph = std::vector< std::vector< Node* > >(gridSize, std::vector<Node*>(gridSize, NULL));

	int counter = 0;
	int rowCounter = 0;

	while(counter < vertices)
	{
		bool isTraversable = false;
		int weight = 0;
		std::cin >> weight;;
		if(weight > 0)
		{
			isTraversable = true;
		}

		graph[rowCounter][counter % gridSize] = 
			new Node(weight, std::make_pair(rowCounter, counter % gridSize), isTraversable);

		++counter;
		if(counter % gridSize == 0)
		{
			++rowCounter;
		}
	}

	//GENERATE CITIES
	std::vector<location_t> city_coordinates(AMOUNT_OF_CITIES);
	for(int i = 0; i < AMOUNT_OF_CITIES; ++i)
	{
		city_coordinates[i] = location_t(rand() % gridSize, rand() % gridSize);
		std::clog << "Created city at (" << city_coordinates[i].first << ", " << city_coordinates[i].second << ")" << std::endl;
	}

	std::vector<location_t> wood_coordinates(AMOUNT_OF_WOODS);
	for(int i = 0; i < AMOUNT_OF_WOODS; ++i)
	{
		wood_coordinates[i] = location_t(rand() % gridSize, rand() % gridSize);
	}

	std::vector<location_t> water_coordinates(AMOUNT_OF_WATER);
	for(int i = 0; i < AMOUNT_OF_WATER; ++i)
	{
		do
		{
			water_coordinates[i] = location_t(rand() % gridSize, rand() % gridSize);
		}
		while(graph[water_coordinates[i].first][water_coordinates[i].second]->Type()
			 == Node::NodeType::WATER);
	}

	targets = std::vector<location_t>(city_coordinates);

	generate_terrain_areas(wood_coordinates, graph, Node::NodeType::WOOD, 4);
	generate_terrain_areas(water_coordinates, graph, Node::NodeType::WATER, 30);
	generate_cities(city_coordinates, graph);
	

	//PRINT MAP

	for(int i = 0; i < gridSize; ++i)
	{
		for(int j = 0; j < gridSize; ++j)
		{
			if(graph[i][j]->Type() == 0 && graph[i][j]->Traversable())
			{
				std::clog << "o";
			}
			else if(graph[i][j]->Type() == 0 && !graph[i][j]->Traversable())
			{
				std::clog << "@";
			}
			else if(graph[i][j]->Type() == Node::NodeType::ROAD)
			{
				std::clog << ".";
			}
			else if(graph[i][j]->Type() == Node::NodeType::WOOD)
			{
				std::clog << "W";
			}
			else if(graph[i][j]->Type() == Node::NodeType::WATER)
			{
				std::clog << "_";
			}
			else if(graph[i][j]->Type() == Node::NodeType::CITY)
			{
				std::clog << "X";
			}
		}
		std::clog << std::endl;
	}



	int travelersCount = atoi(argv[1]);

	traveler_count = travelersCount;

	std::vector<unsigned int> target_index(travelersCount);


	NodeCalculatorThread nct(travelersCount, &graph, gridSize);
	std::vector<Traveler*> travelers;
	//TravelVisualiser travelVisualiser(gridSize, travelers, graph);

	for(int i = 0; i < travelersCount; ++i)
	{
		travelers.push_back(new Traveler(std::make_pair(0,0), i ));
		target_index[i] = 0;
	}

	sem_init(&NodeCalculatorThread::THREAD_SEM, 0, 0);
	sem_init(&print_sem, 0, 1);
	sem_init(&traveler_count_sem, 0, 1);

	for(int i = 0; i < travelersCount; ++i)
	{
		nct.assign_new_task(travelers[i], std::make_pair(0,0), std::make_pair(gridSize-1, gridSize-1));
	}

	nct.start();

	while(true)
	{
		sem_wait(&NodeCalculatorThread::THREAD_SEM);
		if(traveler_count == 0)
		{
			for(int i = 0; i < travelers.size(); ++i)
			{
				if(travelers[i]->travellingThread)
				{
					travelers[i]->travellingThread->join();
				}
			}
			break;
		}
		for(int i = 0; i < travelersCount; ++i)
		{
			if(!travelers[i]->Arrived() && !travelers[i]->StartedTraveling())
			{
				if(travelers[i]->travellingThread)
				{
					travelers[i]->travellingThread->join();
					delete travelers[i]->travellingThread;
				}
				travelers[i]->StartTravel();
				travelers[i]->travellingThread = new std::thread(travel_handler, travelers[i], &nct, &targets, &target_index[i]);
				break;
			}
		}
	}


	std::cout << "# h\ttravel\tcomp\n";
	//std::cout << "# id\tt*c\n";
	for(int i = 0; i < travelers.size(); ++i)
	{	
		double gttt = travelers[i]->GetTotalTravelTime();
		double gtct =  travelers[i]->GetTotalComputationTime();
		std::cout << travelers[i]->Id() * HEURISTIC_COEFFICIENT << "\t" << gttt <<
		 "\t" << gtct << std::endl;
		delete travelers[i];
	}

	for(int i = 0; i < gridSize; ++i)
	{
		for(int j = 0; j < gridSize; ++j)
		{
			delete graph[i][j]; 
		}
	}

	return 0;
}
