#ifndef FCFS_H
#define FCFS_H
#include "ProcessStructure.h"
#include <algorithm>

bool ArrivalComparator(Process P1, Process P2)
{
    return P1.ArrivalTime < P2.ArrivalTime;
}

void FCFS(vector<Process>& processes) 
{
    int n = processes.size();
    int CurrentTime = 0;
    sort(processes.begin(), processes.end(), ArrivalComparator);
    for(int i=0;i < n;i++) 
    {
        if(CurrentTime < processes[i].ArrivalTime) 
        {
          CurrentTime = processes[i].ArrivalTime;
        }

        processes[i].CompletionTime = CurrentTime + processes[i].BurstTime;
        processes[i].TurnAroundTime = processes[i].CompletionTime - processes[i].ArrivalTime;
        processes[i].WaitingTime = processes[i].TurnAroundTime - processes[i].BurstTime;

        CurrentTime += processes[i].BurstTime;
    }
}
#endif