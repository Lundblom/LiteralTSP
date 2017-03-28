#pragma once
#include <thread>
#include <vector>
#include "PathStruct.h"
#include <semaphore.h>

namespace pathfinding 
{
	class NodeCalculatorThread
	{

	private:
		std::thread* t;
		int travelers;
		bool started = false;

		bool running = true;
		std::vector< std::vector< Node* > >* graph;
		std::vector<PathStruct*> pathData;

		sem_t task_sem;

		double estimatedWorkTime = 0;
		int gridSize = 0;

		void work_loop();
	public:

		static sem_t THREAD_SEM;
		static bool SIMULATION_MODE;

		NodeCalculatorThread(int, std::vector< std::vector< Node* > >*, int);
		~NodeCalculatorThread();
		void assign_new_task(Traveler*, std::pair<int, int>, std::pair<int, int>);
		void start();
	};
}