#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <stdio.h>
#include <semaphore.h>
#include <cstdlib>
#include "Node.h"
#include "Traveler.h"
#include "NodeCalculatorThread.h"
#include "PathStruct.h"
#include "a_star.cpp"

#define TRAVEL_COEFFICIENT 400
#define AMOUNT_OF_CITIES 5
#define CITY_HEURISTIC_COEFFICIENT 20000

sem_t print_sem;
sem_t traveler_count_sem;
int traveler_count = 0;



using namespace pathfinding;



void travel_handler(Traveler* t, NodeCalculatorThread* nct, std::vector<location_t>* targets, unsigned int* targetIndex)
{
	while(!t->Arrived())
	{
		int distance = t->NextDistance();
		if(NodeCalculatorThread::SIMULATION_MODE)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(distance * TRAVEL_COEFFICIENT));
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
		if(city_coordinates.empty())
		{
			break;
		}
		int secondIndex = rand() % city_coordinates.size();
		location_t secondLocation = city_coordinates[secondIndex];

		std::stack<Node*> road = a_star(g, firstLocation, secondLocation, CITY_HEURISTIC_COEFFICIENT);

		while(!road.empty())
		{
			Node* n = road.top();
			road.pop();
			n->makeRoad();
			std::clog << n->Position().first << ", " << n->Position().second << std::endl;
		}
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

	graph = std::vector< std::vector< Node* > >(gridSize, std::vector<Node*>(gridSize, NULL));

	int counter = 0;
	int rowCounter = 0;

	while(counter < vertices)
	{
		bool isTraversable;
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

	generate_cities(city_coordinates, graph);

	//PRINT MAP

	for(int i = 0; i < gridSize; ++i)
	{
		for(int j = 0; j < gridSize; ++j)
		{
			if(graph[i][j]->Type() == 0)
			{
				std::clog << "o";
			}
			else if(graph[i][j]->Type() == 2)
			{
				std::clog << "X";
			}
		}
		std::clog << std::endl;
	}



	int travelersCount = atoi(argv[1]);
	int targetCount = atoi(argv[2]);

	traveler_count = travelersCount;

	std::vector<unsigned int> target_index(travelersCount);
	for(int i = 0; i < targetCount; ++i)
	{
		location_t loc(rand() % (gridSize-1) + 1, rand() % (gridSize-1) + 1);
		targets.push_back(loc);
	}


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
	//travelVisualiser.start();

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
				
				//delete travelers[i];
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
