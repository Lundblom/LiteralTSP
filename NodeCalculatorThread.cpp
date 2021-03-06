#include "NodeCalculatorThread.h"
#include <chrono>
#include <iostream>

sem_t pathfinding::NodeCalculatorThread::THREAD_SEM;
bool pathfinding::NodeCalculatorThread::SIMULATION_MODE = 1;



using namespace pathfinding;

NodeCalculatorThread::NodeCalculatorThread(int travelers, std::vector< std::vector< Node* > >* graph, int gridSize)
{
	this->travelers = travelers;

	std::vector<PathStruct*> _pathData; 
	this->pathData = _pathData;

	for(int i = 0; i < travelers; ++i)
	{
		pathData.push_back(NULL);
	}

	this->graph = graph;
	this->gridSize = gridSize;

	sem_init(&task_sem, 0, 1);

	t = new std::thread(&NodeCalculatorThread::work_loop, this);
}

NodeCalculatorThread::~NodeCalculatorThread()
{
	running = false;
	t->join();
	delete t;
}

void NodeCalculatorThread::work_loop()
{
	while(!started){}

	while(running)
	{
		for(auto it = pathData.begin(); it != pathData.end() && running; ++it)
		{
			if(!(*it))
			{
				if(NodeCalculatorThread::SIMULATION_MODE)
				{
					std::this_thread::sleep_for(std::chrono::duration<double>(estimatedWorkTime));
				}
			}
			else
			{
				double time = (*it)->work();
				estimatedWorkTime = (time + estimatedWorkTime) / 2;
				if((*it)->complete())
				{
					std::stack<Node*> s;

					Node* current = (*graph)[(*it)->get_end_x()][(*it)->get_end_y()];
					Node* last = NULL;
					while(current != NULL)
					{
						s.push(current);
						Node* cacheN = current;
						current = (*it)->get_previous(current->Position().first, current->Position().second);
						if(last == current)
						{
							std::cout << "Found infinite loop, breaking...\n";
							std::cout << "Got stuck on " << (*current) << std::endl;
							exit(0);
						}
						last = cacheN;

					}
					(*it)->traveler->InitiateTravel(s);

					delete (*it);
					(*it) = NULL;

					sem_post(&NodeCalculatorThread::THREAD_SEM);
				}
			}
		}
	}
}

void NodeCalculatorThread::assign_new_task(Traveler* t, std::pair<int, int> start,
 std::pair<int, int> end)
{
	PathStruct* ps = new PathStruct(gridSize, start, end, t, graph);
	
	//std::clog << "Watiting for task_sem" << std::endl;
	sem_wait(&task_sem);
	for(int i = 0; i < pathData.size(); ++i)
	{
		if(!pathData[i])
		{
			pathData[i] = ps;
			break;
		}
	}
	//std::clog << "Releasing task_sem" << std::endl;
	sem_post(&task_sem);
}

void NodeCalculatorThread::start()
{
	started = true;
}