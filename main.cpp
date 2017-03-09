#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "dijkstras.cpp"


using namespace pathfinding;

#define STARTX 0
#define STARTY 0
#define ENDX 7
#define ENDY 7

#define RUN_TIMES 1000
#define THREADS 20
#define TIME_BETWEEN_PATHS 100

static std::condition_variable printCV;
static std::mutex printMutex;
static bool printReady = true;

static int GRID_SIZE;

static std::vector< std::vector< Node* > > graph;

void dijkstra_generator(int thread_id)
{
	for(int i = 0; i < RUN_TIMES; ++i)
	{
		int startX;
		int startY;

		int endX;
		int endY;
		do
		{
			startX = rand() % GRID_SIZE;
			startY = rand() % GRID_SIZE;
		}
		while(!graph[startX][startY]->traversable);

		do
		{
			endX = rand() % GRID_SIZE;
			endY = rand() % GRID_SIZE;
		}
		while(!graph[endX][endY]->traversable);

		std::stack<Node*> result = dijkstras(graph, std::make_pair(startX,startY), std::make_pair(endX, endY));

		std::cout << "Out of dijkstras" << std::endl;

		bool pathArr[GRID_SIZE * GRID_SIZE];

		//std::memset(pathArr, 0, sizeof(bool) * GRID_SIZE * GRID_SIZE);

		std::cout << "created pathArr" << std::endl;

		while(!result.empty())
		{
			Node* n = result.top();
			result.pop();

			std::cout << "Got node " << (n->position.first * GRID_SIZE + n->position.second) << std::endl;

			pathArr[n->position.first * GRID_SIZE + n->position.second] = 1;
		}

		std::unique_lock<std::mutex> lock(printMutex);
		while(!printReady)
		{
			printCV.wait(lock);
		}


		std::cout << "Start (" << startX << ", " <<  startY << ")" << std::endl;
		std::cout << "End (" << endX << ", " <<  endY << ")" << std::endl;

		std::cout << "Grid" << std::endl;

		/*
		for(int i = 0; i < GRID_SIZE; ++i)
		{
			for(int j = 0; j < GRID_SIZE; ++j)
			{
				std::cout << (graph[i][j]->traversable ? "X" : "o") << " ";
			}
			std::cout << std::endl;
		}*/
		std::cout << std::endl << "Path";
		for(int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
		{
			if(i % GRID_SIZE == 0)
			{
				std::cout << std::endl;
			}
			std::cout << (pathArr[i] ? "X" : "o") << " ";

		}

		std::cout << "Thread " << thread_id << " finished time " << i << std::endl;

		printReady = true;
		lock.unlock();
		printCV.notify_one();


		std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_PATHS));
	}

}

int main(int argc, char** argv)
{

	int vertices;
	int gridSize;

	std::cin >> vertices;
	std::cin >> gridSize;

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

	for(int i = 0; i < THREADS; ++i)
	{
		threadVector[i] =  new std::thread(dijkstra_generator, (i+1));
	}


	for(int i = 0; i < THREADS; ++i)
	{
		threadVector[i]->join();
	}

	return 0;
}