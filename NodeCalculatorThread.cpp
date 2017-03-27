#include "NodeCalculatorThread.h"
#include <chrono>
#include <iostream>

sem_t NodeCalculatorThread::THREAD_SEM;

NodeCalculatorThread::NodeCalculatorThread(int travelers, std::vector< std::vector< Node* > >* graph)
{
	this->travelers = travelers;

	std::vector<PathStruct*> _pathData; 

	this->pathData = _pathData;
	this->graph = graph;

	t = new std::thread(&NodeCalculatorThread::work_loop, this);
}

void NodeCalculatorThread::work_loop()
{
	while(!started){}

	while(true)
	{
		for(auto it = pathData.begin(); it != pathData.end(); ++it)
		{
			if(!(*it))
			{
				std::this_thread::sleep_for(std::chrono::duration<double>(estimatedWorkTime));
			}
			else
			{

				double time = (*it)->work();
				estimatedWorkTime = (time + estimatedWorkTime) / 2;
				std::cout << estimatedWorkTime << std::endl;
				if((*it)->complete())
				{
					std::cout << "Task  is done " << std::endl;
					std::cout << "Starting stack iteration at x: " << (*it)->get_end_x() <<
						" y: " << (*it)->get_end_y() << std::endl;
					std::stack<Node*> s;

					Node* current = (*graph)[(*it)->get_end_x()][(*it)->get_end_y()];
					std::cout << "Got node: " << current << std::endl;
					while(current != NULL)
					{
						s.push(current);
						current = (*it)->get_previous(current->Position().first, current->Position().second);
						std::cout << "Continuing stack iteration at node address: " << current << std::endl;
					}

					std::cout << "Created stack with size " << s.size() << std::endl;
					(*it)->traveler->InitiateTravel(s);

					delete (*it);
					(*it) = NULL;

					sem_post(&NodeCalculatorThread::THREAD_SEM);
				}
			}
		}
	}
}

void NodeCalculatorThread::assign_new_task(Traveler* t, std::pair<int, int> start, std::pair<int, int> end, int gridSize)
{
	PathStruct* ps = new PathStruct(gridSize, start, end, t, graph);

	std::cout << "Creating new task..." << std::endl;
	
	pathData.push_back(ps);
}

void NodeCalculatorThread::start()
{
	started = true;
}