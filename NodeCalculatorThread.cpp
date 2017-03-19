#include "NodeCalculatorThread.h"

NodeCalculatorThread::NodeCalculatorThread(int travelers)
{
	this->travelers = travelers;

	std::vector<PathStruct*> _pathData(travelers); 

	for(int i = 0; i < travelers; ++i)
	{
		_pathData[i] = NULL: 
	}

	this->pathData = _pathData;
}

void work_loop()
{
	while(true)
	{
		for(int i = 0; i < travelers; ++i)
		{
			if(pathData[i] == null)
			{
				sleep(estimatedWorkTime);
			}
			else
			{
				double time = pathData[i].work();
				estimatedWorkTime = (time + estimatedWorkTime) / 2;

				if(pathData[i].complete())
				{
					//Expel it from the array and send the result to the main thread
				}
			}
		}
	}
}

void NodeCalculatorThread::assign_new_task(PathStruct* ps, std::pair<int, int> start, std::pair<int, int> end)
{
	PathStructWrapper psw(ps, start, end);
	stuctData.push_back(psw);
}