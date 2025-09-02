#ifndef AUTO_H
#define AUTO_H

#include "ProcessStructure.h"
#include <queue>
#include <math.h>
#include "FCFS.h"
#include "RR.h"
#include "PreemptiveSJF.h"
#include "MLQ.h"

const int THRESHOLD = 10;
const int LOAD = 7;
void auto_schedule(vector<Process> & processes){
    double sum = 0,mean = 0,STDDEV = 0;
    int less_priority = 0,high_priority = 0,N = processes.size();
    for(int i=0;i < (int)processes.size();i++)
    {
        sum+=processes[i].BurstTime;
        if(processes[i].Priority <= 10) high_priority++;
        else less_priority++;
    }
    mean = (sum/(N*1.0))*1.0;
    for(int i=0;i < (int)processes.size();i++)
    {
        STDDEV += ((processes[i].BurstTime - mean) * (processes[i].BurstTime - mean));
    }
    STDDEV = sqrt(STDDEV/N);
    if(less_priority != 0 && high_priority != 0) //agar diff priorities hai so better to give them different queues
    {
        MultiLevelQueueScheduling(processes,QUANTUM);
        return;
    }
    if(STDDEV < mean/2.0) //iska matlab basically burst times are low and with less variation so FCFS most suitable
    {
       FCFS(processes);
       return;
    }
    if(N > 7) // RR is just fcfs with fair response to every so if we have varitaion in burst times but many processes so better to with it
    {
        roundRobin(processes,QUANTUM);
        return;
    }
    
    PreemptiveShortestJobFirst(processes); //as such this is best algo greedily if above specific conditions do not match so this is it.
}
#endif