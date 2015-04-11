#include "graph.h"

bool Graph::isConnected(task* x, task* i)
{
	for(int j = 0 ; j<this->edges.size();j++)
	{
		if(this->edges[j]->parentTask->name == x->name && edges[j]->childTask->name == i->name)
		{
			return true;
		}
	}
	return false;
}

void Graph::DFS(task* required, std::vector<task*> &visited, std::vector<taskChain> &taskChains) 
{
    int i, j;
    //the current node, where I am in recursivity
    task* x = visited.back();  

    int ok = 1;

	int n = this->vertices.size();
	//n = number of adjacent nodes or total number of nodes. 
    for (i = 0; i < n; i++) 
	{
        //for all children
		task* l = this->vertices[i];
		if (isConnected(x, l)) 
		{ 
            //need this for ok, explanation down
            for (j = 0; j < visited.size(); j++) 
            {
                if (l->name == visited[j]->name)
                    ok = 0;
            }
            //if it is in the vector already, exit for
            if (!ok)  
			{
                continue;
			}
            ok = 1;
            //If I reached the target, I have the vector containing the nodes from the path
            if (l->name == required->name) 
			{ 
                //introduce the target node in the path
                visited.push_back(l); 
				taskChain tempChain;
                for (j = 0; j < visited.size(); j++) 
				{
                    //print the path
                    //cout<< visited[j] << " ";
					tempChain.tasks.push_back(visited[j]);
					
                }
				taskChains.push_back(tempChain);
                //cout<< "\n";
                //delete the vector. create one vector every time when traversing the graph
                visited.erase(visited.begin() + visited.size() - 1); 
                break;
            }
        }
    }
    //the case when I still have to traverse undiscovered nodes
    for (i = 0; i < n; i++) 
	{ 
        //for all children
		task* l = this->vertices[i];
        if (isConnected(x, l)) { 

            for (j = 0; j < visited.size(); j++) {
                if (l->name == visited[j]->name)
                    ok = 0;
            }
            //if it is already in the vector OR I reached the target, I exit the for
            if (!ok || l == required) 
                continue;
            ok = 1;
            //introduce the child in the vector
            visited.push_back(l); 
            //recursive for this child
            DFS(required, visited, taskChains);  
            //delete the old vector
            visited.erase(visited.begin() + visited.size() - 1); 
        }
    }
}
