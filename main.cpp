#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "dijkstras.cpp"


using namespace pathfinding;

#define GRID_SIZE 8
#define STARTX 0
#define STARTY 0
#define ENDX 7
#define ENDY 7

#define RUN_TIMES 1000
#define THREADS 20
#define TIME_BETWEEN_PATHS 100

static bool traversableArr[] = {
		1,0,0,1,0,0,0,0,
		0,0,0,0,0,0,1,0,
		0,1,0,0,0,0,0,0,
		1,0,0,0,0,0,0,0,
		0,0,0,0,1,0,0,0,
		0,0,0,1,0,0,0,0,
		1,0,1,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		

	};

static int distanceArr[] = {
		1,1,2,3,1,1,1,3,
		1,1,1,1,1,1,1,2,
		1,1,1,1,1,1,3,1,
		1,1,1,10,10,1,1,1,
		1,3,1,10,10,1,1,1,
		1,1,3,1,1,1,3,1,
		1,1,6,1,1,1,1,3,
		1,1,1,1,1,4,1,3
	};

static std::condition_variable printCV;
static std::mutex printMutex;
static bool printReady = true;

static std::vector< std::vector< Node* > > graph(GRID_SIZE, std::vector<Node*>(GRID_SIZE, NULL));

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
		while(traversableArr[startX * GRID_SIZE + startY * GRID_SIZE]);

		do
		{
			endX = rand() % GRID_SIZE;
			endY = rand() % GRID_SIZE;
		}
		while(traversableArr[endY * GRID_SIZE + endY * GRID_SIZE]);

		std::stack<Node*> result = dijkstras(graph, std::make_pair(startX,startY), std::make_pair(endX, endY));

		bool pathArr[GRID_SIZE * GRID_SIZE];

		for(int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
		{
			pathArr[i] = 0;
		}

		while(!result.empty())
		{
			Node* n = result.top();
			result.pop();

			pathArr[n->position.first * GRID_SIZE + n->position.second] = 1;
		}

		std::unique_lock<std::mutex> lock(printMutex);
		while(!printReady)
		{
			printCV.wait(lock);
		}


		std::cout << "Start (" << startX << ", " <<  startY << ")" << std::endl;
		std::cout << "End (" << endX << ", " <<  endY << ")" << std::endl;

		std::cout << "Grid";

		for(int i = 0; i < GRID_SIZE * GRID_SIZE; ++i)
		{
			if(i % GRID_SIZE == 0)
			{
				std::cout << std::endl;
			}
			std::cout << (traversableArr[i] ? "X" : "o") << " ";

		}
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

int main()
{
	for(int i = 0; i < GRID_SIZE; ++i)
	{
		for(int j = 0; j < GRID_SIZE; ++j)
		{
			graph[i][j] = new Node(distanceArr[i * GRID_SIZE + j], std::make_pair(i,j), !traversableArr[i * GRID_SIZE + j]);
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