#include<stdio.h>
#include<iostream>
#include<algorithm>
#include<vector>
#include<string>
#include<conio.h>
#include<fstream>
#include<sstream>

#include "slot.h"
#include "task.h"
#include "taskChain.h"
#include "graph.h"
#include "edge.h"
#include "resource.h"

# define MAX 1000
using namespace std;

class resource;


vector<slot*> globalFreeSlots;


task* checkTaskExists(vector<task*> &tasks, string name);

void autolevelTasks(vector<task*> &tasks,vector<taskChain> &taskChains,vector<resource*> &resources);

void findTaskChains(Graph* graph,vector<task*> &tasks, vector<taskChain> &taskChains);

void getSuitableSlotDuration(task *t);
//class timeLine
//{
//	vector<task> tasks;
//};

void computeTotalDurations(vector<taskChain> &taskChains)
{
	for(int i = 0; i<taskChains.size();i++)
	{
		for(int j = 0; j<taskChains[i].tasks.size();j++)
		{
			taskChains[i].totalDuration+=taskChains[i].tasks[j]->duration;
		}
	}
}
bool compareTaskChains(taskChain t1, taskChain t2)
{
	return (t1.totalDuration>t2.totalDuration);
}

bool compareFreeSlots(slot *s1, slot *s2)
{
	if(s1->start<s2->start)
	{
		return true;
	}
	if(s1->start == s2->start)
	{
		return s1->slotDuration < s2->slotDuration; 
	}
	return false;
}

void adjustSuccessors(task *t)
{
	for(int i = 0 ; i<t->successors.size();i++)
	{
		if(t->successors[i]->startDay<=t->endDay)
		{
			t->successors[i]->startDay = t->endDay + 1;
			t->successors[i]->endDay = t->successors[i]->startDay + t->successors[i]->duration -1;
		}
	}
}

void sortTaskChains(vector<taskChain> &taskChains)
{
	//int n = taskChains.size();
	sort(taskChains.begin(),taskChains.end(),compareTaskChains);
	
	//initialize start days.
	for(int i = 0; i< taskChains.size() ; i++)
	{
		taskChains[i].tasks[0]->startDay = 1;
		taskChains[i].tasks[0]->endDay = taskChains[i].tasks[0]->startDay + taskChains[i].tasks[0]->duration - 1;
		for(int j = 1 ; j<taskChains[i].tasks.size(); j++)
		{
			task *t = taskChains[i].tasks[j];
			if(t->startDay == -1 || t->startDay <= taskChains[i].tasks[j-1]->endDay)
			{
					t->startDay = taskChains[i].tasks[j-1]->endDay + 1;	
					t->endDay = t->startDay + t->duration -1;
			}
		}
	}
}

int getIndex(vector<slot*> slots, slot *s)
{
	for(int i = 0 ;i<slots.size();i++)
	{
		if(slots[i] == s)
		{
			return i;
		}
	}
	return -1;
}

void allocateResources(vector<taskChain> &taskChains,vector<resource*> &resources)
{
	
	// arrange all the free slots in a global array.
	for(int i=0;i<resources.size();i++)
	{
		for(int j=0;j<resources[i]->freeSlots.size();j++)
		{
			globalFreeSlots.push_back(resources[i]->freeSlots[j]);
		}
	}

	sort(globalFreeSlots.begin(),globalFreeSlots.end(),compareFreeSlots);

	// check if the task can fit in any free slot of any resource with least start time.
	// assign all the resources in global free slot vector.
	for(int i = 0 ; i < taskChains.size(); i++)
	{
		taskChain tc = taskChains[i];
		for(int j=0;j<tc.tasks.size();j++)
		{	
			task *t = tc.tasks[j];
			
			if(t->resourceOnTask == NULL)
			{
				
				for(int k =0;k<globalFreeSlots.size() && t->resourceOnTask == NULL;k++)
				{
					slot *s = globalFreeSlots[k]; 
					if(s->slotDuration>= t->duration && t->startDay>=s->start && t->endDay<=s->end)
					{
						//t->startDay = s->start;
						//t->endDay = s->start + t->duration -1;
						t->resourceOnTask = s->resourceOnSlot;
						//adjustSuccessors();
						swap(globalFreeSlots[k],globalFreeSlots.back());
						globalFreeSlots.pop_back();
						int ind = getIndex(s->resourceOnSlot->freeSlots,s);
						if(ind >-1)
						{
							swap(s->resourceOnSlot->freeSlots[ind],s->resourceOnSlot->freeSlots.back());
							s->resourceOnSlot->freeSlots.pop_back();
						}
						
						int remDuration1 = t->startDay - s->start;
						if(remDuration1 > 0)
						{
							slot *s1 = new slot(s->start,t->startDay-1);
							s1->resourceOnSlot = s->resourceOnSlot;	

							globalFreeSlots.push_back(s1);
							//sort(globalFreeSlots.begin(),globalFreeSlots.end(),compareFreeSlots);
							s->resourceOnSlot->freeSlots.push_back(s1);
						}

						int remDuration2 = s->end - t->endDay;
						if(remDuration2 > 0)
						{
							slot *s1 = new slot(t->endDay+1, s->end);
							s1->resourceOnSlot = s->resourceOnSlot;
							globalFreeSlots.push_back(s1);						
							s->resourceOnSlot->freeSlots.push_back(s1);
						}
						sort(globalFreeSlots.begin(),globalFreeSlots.end(),compareFreeSlots);
					}
				}
				if(t->resourceOnTask == NULL)
				{
					getSuitableSlotDuration(t);
					//adjustSuccessors();
				}
			}
			//getSuitableFreeSlotWithMatchingDuration(taskChains[i].tasks[j],resources); 
			//	globalFreeSlots.push_back();
		}
	}
	// if not assign it to the resource with least end time. 
}

void getSuitableSlotDuration(task *t)
{
	for(int i = 0 ; i<globalFreeSlots.size() ; i++)
	{
		slot* s = globalFreeSlots[i];
		if(t->duration< s->slotDuration && s->start >= t->startDay)
		{
			t->resourceOnTask = s->resourceOnSlot;
			t->startDay = s->start;
			t->endDay = t->startDay + t->duration-1;
			adjustSuccessors(t);
			swap(globalFreeSlots[i],globalFreeSlots.back());
			globalFreeSlots.pop_back();
			int ind = getIndex(s->resourceOnSlot->freeSlots,s);
			if(ind >-1)
			{
				swap(s->resourceOnSlot->freeSlots[ind],s->resourceOnSlot->freeSlots.back());
				s->resourceOnSlot->freeSlots.pop_back();
			}
						
			int remDuration1 = t->startDay - s->start;
			if(remDuration1 > 0)
			{
				slot *s1 = new slot(s->start,t->startDay-1);
				s1->resourceOnSlot = s->resourceOnSlot;	

				globalFreeSlots.push_back(s1);
				//sort(globalFreeSlots.begin(),globalFreeSlots.end(),compareFreeSlots);
				s->resourceOnSlot->freeSlots.push_back(s1);
			}

			int remDuration2 = s->end - t->endDay;
			if(remDuration2 > 0)
			{
				slot *s1 = new slot(t->endDay+1, s->end);
				s1->resourceOnSlot = s->resourceOnSlot;
				globalFreeSlots.push_back(s1);						
				s->resourceOnSlot->freeSlots.push_back(s1);
			}
			sort(globalFreeSlots.begin(),globalFreeSlots.end(),compareFreeSlots);
			return;		
		}
	}
}

void initialize(Graph* graph, vector<task*> &tasks, ifstream& fp) 
{
   fp.clear();
   fp.seekg(0, std::ios::beg);
   //vector<task*> tasks;
   //Graph *graph;
   int population;
   int pos1, pos2;
   string line;
 
   while (!fp.eof()) 
   {
      task *task1 = new task();
	  getline(fp, line);
	  while(line!="")
	  {
		task1->name = line;
		getline(fp, line);
		pos1 = -1;
		pos2 = line.find(',');
			while(pos2 != -1)
			{
				string preName = line.substr(pos1+1, pos2-pos1-1);
			    task *existingTask = checkTaskExists(tasks,preName);
			    if(existingTask != NULL)
				{
					task1->predecessors.push_back(existingTask);
					existingTask->successors.push_back(task1);
					Edge *edge = new Edge(existingTask,task1);
					graph->edges.push_back(edge);
				}
				pos1 = pos2;
				pos2 = line.find(',',pos2+1);
			}
			if(pos2 == -1 && line == "-")
			{
					
			}
			getline(fp, line);
			task1->duration = atoi(line.c_str());
			getline(fp, line);

			tasks.push_back(task1);
	        graph->vertices.push_back(task1);

			/*if(fp.eof())
				break;*/
	  }
   }
}

int main(int argc, char *argv[])
{
	Graph *graph = new Graph();
	vector<task*> tasks;

	vector<resource*> resources;
	vector<taskChain> taskChains;
	char ans = 'y',ans3='y';
	int noRes;

		if ( argc != 2 ) 
		{
			std::cout << "usage: " << argv[0] << " <filename>" <<std::endl;
			return 0;
		}

		ifstream fp;
		fp.open(argv[1], std::ios::in);
		if (!fp.is_open()) 
		{
			std::cout << "can't open file " << argv[1] << std::endl;
			return 0;
		}
		
		initialize(graph,tasks,fp);
		
		cout<<"\nenter the number of resources : ";
		cin>>noRes;
		cout<<"\n\nenter names : ";
		for(int i = 0;i<noRes;i++)
		{
			resource *r1 = new resource();
			cin>>r1->name;
			resources.push_back(r1);
		}

		findTaskChains(graph,tasks,taskChains);
		
		computeTotalDurations(taskChains);
		
		sortTaskChains(taskChains);
		
		allocateResources(taskChains,resources);

		cout<<"\n\n allocated resources are as follows :\n";
		
		int maxEndDay = -1;
		for(int i = 0;i<taskChains.size();i++)
		{
			
			cout<< "\n\ntask chain "<<i<<" :";
			for(int j =0;j<taskChains[i].tasks.size();j++)
			{
				cout<<"\n"<<taskChains[i].tasks[j]->name<<" ---> "<<taskChains[i].tasks[j]->resourceOnTask->name<<" start day : " <<taskChains[i].tasks[j]->startDay;
				if(taskChains[i].tasks[j]->endDay>maxEndDay)
				{
					maxEndDay = taskChains[i].tasks[j]->endDay;
				}

			}
		}

		cout<<"\n\nTotal number of days required for execution are : "<<maxEndDay;

	_getch();
}

void findTaskChains(Graph *graph,vector<task*> &tasks, vector<taskChain> &taskChains)
{
	vector<task*> noSuccTasks;
	vector<task*> noPredTasks;

	for(int i=0;i<tasks.size();i++)
	{
		if(tasks[i]->successors.size() == 0)
		{
			noSuccTasks.push_back(tasks[i]);
		}
	}

	for(int i = 0;i<tasks.size();i++)
	{
		if(tasks[i]->predecessors.size() == 0)
		{
			noPredTasks.push_back(tasks[i]);
		}
	}

	for(int i = 0 ;i<noSuccTasks.size();i++)
	{
		task* required = noSuccTasks[i];
		for(int j = 0;j<noPredTasks.size();j++)
		{
			vector<task*> visited;
			visited.push_back(noPredTasks[j]);
			graph->DFS(required, visited,taskChains);
		}
	}

	for(int i = 0;i<tasks.size();i++)
	{
		if(tasks[i]->predecessors.size() == 0 && tasks[i]->successors.size() == 0)
		{	
			taskChain remainingTasks;
			remainingTasks.tasks.push_back(tasks[i]);
			taskChains.push_back(remainingTasks);
		}
	}

	cout<<"\n\n the critical paths are as follows:";
		for(int i =0 ;i<taskChains.size();i++)
		{
		 cout<<"\ncritical path "<<i<<" : ";
		 for(int j=0 ;j<taskChains[i].tasks.size();j++)
		 {
			cout<<taskChains[i].tasks[j]->name<<"\t";
		 }
		 cout<<"\n";
		}

}


//bool checkEndDateOfPredecessors(task &task1, int startDay)
//{
//	vector<task>::iterator it;
//	int j=0;
//	for(it = task1.predecessors.begin();it!=task1.predecessors.end();it++,j++)
//	{
//		if(startDay > task1.predecessors[j].endDay)
//		{
//			continue;
//		}
//		else
//		{
//			return false;
//		}
//	}
//	return true;
//}

task* checkTaskExists(vector<task*> &tasks, string name)
{
	vector<task*>::iterator it;
	int j=0;
	for(it = tasks.begin();it!=tasks.end();it++,j++)
	{
		if(tasks[j]->name == name)
		{
			return tasks[j];
		}
	}
		return NULL;
}