#ifndef SCHEDULER_H
#define SCHEDULER_H

using namespace std;
struct Process {
    int PID;
    int ArrivalTime;
    int BurstTime;
    int Priority;
    int CompletionTime=0;
    int WaitingTime=0;
    int TurnAroundTime=0;
    int StartTime=0;
    int RemainingTime=0;
};

#endif 
