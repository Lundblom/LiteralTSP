#include "NodeCalculatorThread.h"
#include <chrono>

sem_t NodeCalculatorThread::THREAD_SEM;

NodeCalculatorThread::NodeCalculatorThread(int travelers)
{
	this->travelers = travelers;

	std::vector<PathStruct*> _pathData(travelers); 

	for(int i = 0; i < travelers; ++i)
	{
		_pathData[i] = NULL;
	}

	this->pathData = _pathData;

	t = new std::thread(&NodeCalculatorThread::work_loop, this);
}

void NodeCalculatorThread::work_loop()
{
	while(true)
	{
		for(int i = 0; i < travelers; ++i)
		{
			if(pathData[i] == NULL)
			{
				std::this_thread::sleep_for(std::chrono::duration<double>(estimatedWorkTime));
			}
			else
			{
				double time = pathData[i]->work();
				estimatedWorkTime = (time + estimatedWorkTime) / 2;

				if(pathData[i]->complete())
				{
					std::stack<Node*> s;

					Node* current = (*graph)[pathData[i]->get_end_x()][pathData[i]->get_end_y()];
					while(current != NULL)
					{
						s.push(current);
						current = pathData[i]->get_previous(current->Position().first, current->Position().second);
					}

					pathData[i]->traveler->InitiateTravel(s);

					delete pathData[i];
					pathData.erase(pathData.begin() + i);

					sem_post(&NodeCalculatorThread::THREAD_SEM);
				}
			}
		}
	}
}

void NodeCalculatorThread::assign_new_task(Traveler* t, std::pair<int, int> start, std::pair<int, int> end, int gridSize)
{
	PathStruct* ps = new PathStruct(gridSize, start, end, t, graph);
	
	pathData.push_back(ps);
}