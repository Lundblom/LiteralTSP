#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <stdio.h>
#include <semaphore.h>
#include "Node.h"
#include "Traveler.h"
#include "NodeCalculatorThread.h"


#define STARTX 0
#define STARTY 0
#define ENDX 7
#define ENDY 7

#define TRAVEL_COEFFICIENT 40

static std::condition_variable printCV;
static std::mutex printMutex;
static bool printReady = true;

static int GRID_SIZE;
static int THREADS;
static int RUN_TIMES;


using namespace pathfinding;

static std::vector< std::vector< Node* > > graph;

/*void path_generator(int thread_id, int type)
{

	int startX = 0;
	int startY = 0;

	double totalTime = 0;
	double totalLength = 0;

	int endX = -1;
	int endY = -1;

	for(int i = 0; i < RUN_TIMES; ++i)
	{
		startX = 0;
		startY = 0;

		endX = GRID_SIZE - 1;
		endY = GRID_SIZE - 1;

		std::stack<Node*> result;
		double cpu0  = get_cpu_time();
		double wall0 = get_wall_time();
		if(type == 1)
			result = a_star(graph, std::make_pair(startX,startY), std::make_pair(endX, endY), 1000000);
		else
			result = dijkstras(graph, std::make_pair(startX,startY), std::make_pair(endX, endY));
		double cpu1  = get_cpu_time();
		double wall1 = get_wall_time();


		totalLength += result.size();
		totalTime += (cpu1 - cpu0);

		bool* pathArr = new bool[GRID_SIZE * GRID_SIZE];

		memset(pathArr, 0, sizeof(bool) * GRID_SIZE * GRID_SIZE);

		while(!result.empty())
		{
			Node* n = result.top();
			result.pop();
			pathArr[n->Position().first * GRID_SIZE + n->Position().second] = 1;
		}
		
		std::unique_lock<std::mutex> lock(printMutex);
		while(!printReady)
		{
			printCV.wait(lock);
		}

		std::cout << "Thread " << thread_id << " finished time " << (i+1) << std::endl;

		printReady = true;
		lock.unlock();
		printCV.notify_one();
	}

	std::cout << "Thread " << thread_id << " finished with time per length unit: " << ((totalTime) / (totalLength)) << std::endl;

}*/

void travel_handler(Traveler* t)
{
	std::cout << "Started new travel handler." << std::endl;
	while(!t->Arrived())
	{
		int distance = t->NextDistance();
		std::this_thread::sleep_for(std::chrono::milliseconds(distance * TRAVEL_COEFFICIENT));
		std::cout << "Traveler traversing distance " << distance << std::endl;
		t->Travel();
	}
	std::cout << "Exiting travel handler." << std::endl;
}

int main(int argc, char** argv)
{

	int vertices;
	int gridSize;

	std::cin >> vertices;
	std::cin >> gridSize;

	THREADS = atoi(argv[1]);
	RUN_TIMES = atoi(argv[2]);

	std::cout << "vertices: " << vertices << std::endl;
	std::cout << "gridSize: " << gridSize << std::endl;

	GRID_SIZE = gridSize;

	graph = std::vector< std::vector< Node* > >(GRID_SIZE, std::vector<Node*>(GRID_SIZE, NULL));

	int counter = 0;
	int rowCounter = 0;
	while(counter < vertices)
	{
		bool isTraversable;
		std::cin >> isTraversable;
		graph[rowCounter][counter % GRID_SIZE] = new Node(1, std::make_pair(rowCounter, counter % GRID_SIZE), isTraversable);
		++counter;
		if(counter % GRID_SIZE == 0)
		{
			++rowCounter;
		}
	}

	int travelersCount = atoi(argv[3]);

	NodeCalculatorThread nct(travelersCount, &graph);
	std::vector<Traveler*> travelers;

	for(int i = 0; i < travelersCount; ++i)
	{
		travelers.push_back(new Traveler(std::make_pair(0,0)));
	}

	sem_init(&NodeCalculatorThread::THREAD_SEM, 0, 0);

	for(int i = 0; i < travelersCount; ++i)
	{
		nct.assign_new_task(travelers[i], std::make_pair(0,0), std::make_pair(gridSize-1, gridSize-1), gridSize);
	}

	nct.start();

	while(true)
	{
		std::cout << "Waiting for semaphore..." << std::endl;
		sem_wait(&NodeCalculatorThread::THREAD_SEM);
		for(int i = 0; i < travelersCount; ++i)
		{
			if(!travelers[i]->Arrived())
			{
				std::cout << "Traveler " << i << " has not arrived, creating new thread." << std::endl;
				if(!travelers[i]->travellingThread)
				{
					delete travelers[i]->travellingThread;
				}
				travelers[i]->travellingThread = new std::thread(travel_handler, travelers[i]);
				break;
			}
		}
	}


	return 0;
}
