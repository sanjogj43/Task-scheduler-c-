#ifndef EDGE_H
#define EDGE_H

#include "task.h"
class Edge
{
	public:
		task* parentTask;
		task* childTask;
		Edge(task* parent, task* child)
		{
			parentTask = parent;
			childTask = child;
		}
};

#endif