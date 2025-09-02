#ifndef RR_H
#define RR_H
#include "ProcessStructure.h"
#include <algorithm>
#include <queue>

const int QUANTUM = 2;
void roundRobin(vector<Process>& processes, int TimeQuantum) 
{
    int CurrentTime = 0;
    queue<int> readyQueue;
    vector<int> remainingTime(processes.size());
    vector<int> ArrivalTimes(processes.size());

    for (int i=0; i < (int)processes.size(); ++i) 
    {
        remainingTime[i] = processes[i].BurstTime;
        ArrivalTimes[i] = processes[i].ArrivalTime;
    }

    int completed = 0;
    int n = processes.size();
    int currentProcess = 0;
    vector<int> lastExecutionTime(processes.size(), 0);

    while(completed != n) 
    {
        bool done = true;

        for(int i=0; i < (int)processes.size();i++) 
        {
            if(ArrivalTimes[i] <= CurrentTime && remainingTime[i] > 0) 
            {
                readyQueue.push(i);
                ArrivalTimes[i] = 1e8; // Ensure it isn't added again
            }
        }

        if(!readyQueue.empty()) 
        {
            currentProcess = readyQueue.front();
            readyQueue.pop();

            if(remainingTime[currentProcess] > TimeQuantum) 
            {
                if(lastExecutionTime[currentProcess] == 0) 
                {
                    processes[currentProcess].StartTime = CurrentTime;
                }

                CurrentTime += TimeQuantum;
                remainingTime[currentProcess] -= TimeQuantum;
                lastExecutionTime[currentProcess] = CurrentTime;

                for(int i=0; i < (int)processes.size();i++) 
                {
                    if(ArrivalTimes[i] <= CurrentTime && remainingTime[i] > 0 && i != currentProcess) 
                    {
                        readyQueue.push(i);
                        ArrivalTimes[i] = 1e8; // Ensure it isn't added again
                    }
                }

                readyQueue.push(currentProcess);
            } 
            else 
            {
                if(lastExecutionTime[currentProcess] == 0) 
                {
                    processes[currentProcess].StartTime = CurrentTime;
                }

                CurrentTime += remainingTime[currentProcess];
                processes[currentProcess].CompletionTime = CurrentTime;
                processes[currentProcess].TurnAroundTime = CurrentTime - processes[currentProcess].ArrivalTime;
                processes[currentProcess].WaitingTime = processes[currentProcess].TurnAroundTime - processes[currentProcess].BurstTime;
                remainingTime[currentProcess] = 0;
                completed++;
            }

            done = false;
        }

        if(done) 
        {
            CurrentTime++;
        }
    }
}

#endif