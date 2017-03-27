#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <stdio.h>
#include "dijkstras.cpp"
#include "measure_time.h"


using namespace pathfinding;

#define STARTX 0
#define STARTY 0
#define ENDX 7
#define ENDY 7

static std::condition_variable printCV;
static std::mutex printMutex;
static bool printReady = true;

static int GRID_SIZE;
static int THREADS;
static int RUN_TIMES;

static std::vector< std::vector< Node* > > graph;

void path_generator(int thread_id, int type)
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

	std::thread* threadVector[THREADS];

	double cpu0  = get_cpu_time();
	double wall0 = get_wall_time();
	for(int i = 0; i < THREADS; ++i)
	{
		threadVector[i] =  new std::thread(path_generator, (i+1), 1);
	}

	for(int i = 0; i < THREADS; ++i)
	{
		threadVector[i]->join();
	}
	double cpu1  = get_cpu_time();
	double wall1 = get_wall_time();

	std::cout << "Total CPU time taken: " << (cpu1 - cpu0) << std::endl;
	std::cout << "Total WALL time taken: " << (wall1 - wall0) << std::endl;

	return 0;
}
