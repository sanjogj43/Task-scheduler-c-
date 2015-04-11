#ifndef GRAPH_H
#define GRAPH_H

#include "task.h"
#include "taskChain.h"
#include "edge.h"
#include<vector>

class Graph
{
	public:
		std::vector<task*> vertices;
		std::vector<Edge*> edges;
		bool isConnected(task* x, task* i);
		//int getNoOfAdjacentNodes(task startVertex);
		void DFS(task* required, std::vector<task*> &visited, std::vector<taskChain> &taskChains);

};

#endif