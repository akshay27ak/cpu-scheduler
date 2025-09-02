#ifndef PREEMPTIVESJF_H
#define PREEMPTIVESJF_H

#include "ProcessStructure.h"
#include <algorithm>
#include <climits>

int ShortestBurstLeft(vector<Process> &processes,int CurrentTime)
{
    int first_occurence = -1;
    int min_remaining_time = INT_MAX;
    for(int i=0;i < (int)processes.size();i++)
    {
        if(processes[i].ArrivalTime <= CurrentTime && processes[i].RemainingTime > 0 && processes[i].RemainingTime < min_remaining_time)
        {
            first_occurence = i;
            min_remaining_time = processes[i].RemainingTime;
        }
    }
    return first_occurence;
}

void PreemptiveShortestJobFirst(vector<Process>& processes) 
{
    sort(processes.begin(), processes.end(), ArrivalComparator);
    for(int i=0;i < (int)processes.size();i++)
    {
        processes[i].RemainingTime = processes[i].BurstTime;  //initially no process has executed
    }
    int CurrentTime = 0;
    int total_number_of_processes=processes.size(),so_far_completed=0;
    vector<bool> vis(processes.size(),false);

    while(so_far_completed != total_number_of_processes)
    {
        int idx = ShortestBurstLeft(processes,CurrentTime);

        if(idx==-1)
        {
            CurrentTime++;
            continue;
        }
        if(!vis[idx])
        {
            processes[idx].StartTime = CurrentTime;
            vis[idx]=true;
        }
        processes[idx].RemainingTime--;
        CurrentTime++;

        if(processes[idx].RemainingTime == 0)
        {
            processes[idx].CompletionTime = CurrentTime;
            processes[idx].TurnAroundTime = processes[idx].CompletionTime - processes[idx].ArrivalTime;
            processes[idx].WaitingTime = processes[idx].TurnAroundTime - processes[idx].BurstTime;
            so_far_completed++;
        }
    }
}

#endif