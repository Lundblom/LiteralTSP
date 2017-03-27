#pragma once
#include <thread>
#include <vector>
#include "PathStruct.h"
#include <semaphore.h>

class NodeCalculatorThread
{

private:
	std::thread* t;
	int travelers;
	bool started = false;
	std::vector< std::vector< Node* > >* graph;
	std::vector<PathStruct*> pathData;

	double estimatedWorkTime = 0;



	void work_loop();
public:

	static sem_t THREAD_SEM;

	NodeCalculatorThread(int, std::vector< std::vector< Node* > >*);
	void assign_new_task(Traveler*, std::pair<int, int>, std::pair<int, int>, int);
	void start();
};