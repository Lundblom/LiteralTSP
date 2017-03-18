#include <thread>
#include <vector>
#include "PathStruct.h"

class NodeCalculatorThread
{

private:
	std::thread t;
	int travelers;
	std::vector<PathStructWrapper> structData;

	double estimatedWorkTime = 0;



	void work_loop();
public:
	NodeCalculatorThread();
	void assign_new_task(PathStruct*, std::pair<int, int>, std::pair<int, int>);

};