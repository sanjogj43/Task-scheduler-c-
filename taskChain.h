#ifndef TASK_CHAIN_H
#define TASK_CHAIN_H

#include "task.h"
#include <vector>

class taskChain
{
	public:
		std::vector<task*> tasks;
		int totalDuration;
		taskChain()
		{
			totalDuration = 0;
		}
};

#endif