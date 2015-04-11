#ifndef RESOURCE_H
#define RESOURCE_H

#include "slot.h"
#include <vector>
#include<string>
#define MAX 1000
class resource
{
	public:
		
		std::string name;
		//task *currentTask;
		//bool isWorking;
		//int days;
		//task *taskAssigned;
		//vector<slot> occupiedSlots;
		std::vector<slot*> freeSlots;

		resource()
		{
			name = "";
			slot *freeSlot = new slot(1,MAX);
			freeSlot->resourceOnSlot = this;
			freeSlots.push_back(freeSlot);
			//currentTask = NULL;
			//isWorking = false;
			//days = 0; 
		}
};

#endif