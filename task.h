#ifndef TASK_H
#define TASK_H

#include<vector>
#include "resource.h"
#include<string>

class task
{
	public:
		std::string name;
		
		std::vector<task*> predecessors;
		std::vector<task*> successors;
		
		bool isOver;
		resource *resourceOnTask; 
		
		int startDay;
		int duration;
		int endDay;
		
		task()
		{
			name = "";
			startDay = -1;
			duration = -1;
			endDay = -1;
			resourceOnTask = NULL;
		}
};

#endif