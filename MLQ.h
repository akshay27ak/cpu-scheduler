#ifndef MLQ_H
#define MLQ_H

#include "ProcessStructure.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <iostream>

const int FIXED_PRIORITY_LEVEL = 10;

void MultiLevelQueueScheduling(vector<Process>& processes, int QUANTUM) 
{
    sort(processes.begin(),processes.end(),ArrivalComparator);
    for(int i=0;i < (int)processes.size();i++) 
    {
        processes[i].RemainingTime = processes[i].BurstTime;
    }

    int currentTime = 0;
    int completedProcesses = 0;
    int totalProcesses = processes.size();
    vector<bool> vis(totalProcesses, false);
    queue<int> foregroundQueue; 
    deque<int> backgroundQueue; // we are goona use this to maintain the order of FCFS otherwise if you use queue then there is an edge case to it

    while(completedProcesses < totalProcesses) 
    {
        for(int i=0;i < totalProcesses; i++) 
        {
            if(currentTime == processes[i].ArrivalTime) 
            {
                if(processes[i].Priority <= FIXED_PRIORITY_LEVEL) 
                {
                    foregroundQueue.push(i);
                } 
                else 
                {
                    backgroundQueue.push_back(i);
                }
            }
        }

        if(foregroundQueue.empty() && backgroundQueue.empty()) 
        {
            currentTime++;
            continue;
        }

        if(!foregroundQueue.empty()) 
        {
            int idx = foregroundQueue.front();
            foregroundQueue.pop();

            if(!vis[idx]) 
            {
                processes[idx].StartTime = currentTime;
                vis[idx] = true;
            }
            if(processes[idx].RemainingTime > QUANTUM) 
            {
                processes[idx].RemainingTime -= QUANTUM;
                int tempTime = currentTime + QUANTUM;

                // edge case if new background processes are introduced during quantum time
                for(int k=currentTime+1; k < tempTime;k++) 
                {
                    currentTime = k;
                    for(int i=0;i < totalProcesses;i++) 
                    {
                        if(currentTime == processes[i].ArrivalTime) 
                        {
                            if(processes[i].Priority <= FIXED_PRIORITY_LEVEL) 
                            {
                                foregroundQueue.push(i);
                            } 
                            else 
                            {
                                backgroundQueue.push_back(i);
                            }
                        }
                    }
                }

                currentTime = tempTime;
                foregroundQueue.push(idx);
            } 
            else 
            {
                int remainingTime = processes[idx].RemainingTime;
                processes[idx].RemainingTime = 0;
                int tempTime = currentTime + remainingTime;

                // edge case if new background processes are introduced during quantum time
                for(int k=currentTime + 1; k < tempTime;k++) 
                {
                    currentTime = k;
                    for(int i=0;i < totalProcesses;i++) 
                    {
                        if(currentTime == processes[i].ArrivalTime) 
                        {
                            if(processes[i].Priority <= FIXED_PRIORITY_LEVEL) 
                            {
                                foregroundQueue.push(i);
                            } 
                            else 
                            {
                                backgroundQueue.push_back(i);
                            }
                        }
                    }
                }

                currentTime = tempTime;
                processes[idx].CompletionTime = currentTime;
                processes[idx].TurnAroundTime = processes[idx].CompletionTime - processes[idx].ArrivalTime;
                processes[idx].WaitingTime = processes[idx].TurnAroundTime - processes[idx].BurstTime;
                completedProcesses++;
            }
        } 
        else if(!backgroundQueue.empty()) 
        {
            int idx = backgroundQueue.front();
            backgroundQueue.pop_front();

            if(!vis[idx]) 
            {
                processes[idx].StartTime = currentTime;
                vis[idx] = true;
            }

            processes[idx].RemainingTime--;
            currentTime++;

            if(processes[idx].RemainingTime > 0) 
            {
                backgroundQueue.push_front(idx);
            } 
            else 
            {
                processes[idx].CompletionTime = currentTime;
                processes[idx].TurnAroundTime = processes[idx].CompletionTime - processes[idx].ArrivalTime;
                processes[idx].WaitingTime = processes[idx].TurnAroundTime - processes[idx].BurstTime;
                completedProcesses++;
            }
        }
    }
}

#endif
