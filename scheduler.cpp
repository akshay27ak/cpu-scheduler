#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <cassert>
#include "ProcessStructure.h"
#include "FCFS.h"
#include "RR.h"
#include "PreemptiveSJF.h"
#include "MLQ.h"
#include "Auto.h"

using namespace std;

const double TIME_CUTOFF = 1.0;
const int ITERATIONS = 1e6;

vector<Process> ParseInput(const string& input) 
{
    vector<Process> processes;
    stringstream ss(input);
    string processString;

    while(getline(ss, processString, ';')) 
    {
        stringstream ps(processString);
        string token;
        Process process;

        getline(ps, token, ',');
        process.PID = stoi(token);
        getline(ps, token, ',');
        process.ArrivalTime = stoi(token);
        getline(ps, token, ',');
        process.BurstTime = stoi(token);
        getline(ps, token, ',');
        process.Priority = stoi(token);

        processes.push_back(process);
    }

    return processes;
}

int main(int argc, char* argv[]) 
{
    if(argc < 3) 
    {
        cerr << "Usage: scheduler.exe <algorithm> <input>" << endl;
        return 1;
    }

    auto chrono_begin = chrono::steady_clock::now();
    string algorithm = argv[1];
    string input = argv[2];

    vector<Process> processes = ParseInput(input);

    if(algorithm == "FCFS") 
    {
        FCFS(processes);
    } 
    else if(algorithm == "RR") 
    {
        roundRobin(processes, QUANTUM);
    } 
    else if(algorithm == "PreSJF") 
    {
        PreemptiveShortestJobFirst(processes);
    } 
    else if(algorithm == "MLQ") 
    {
        MultiLevelQueueScheduling(processes,QUANTUM);
    } 
    else if(algorithm == "Auto") 
    {
        auto_schedule(processes);
    } 
    else 
    {
        cerr << "Invalid algorithm choice\n";
        return 1;
    }

    double totalTAT = 0;
    double totalWT = 0;
    int n = processes.size();

    for(const auto& process : processes) 
    {
        totalTAT += process.TurnAroundTime;
        totalWT += process.WaitingTime;
    }

    double averageTAT = totalTAT / n;
    double averageWT = totalWT / n;
    
    

    cout << "{ \"processes\": [";
    for (int i=0; i < (int)processes.size();i++) {
        cout << "{"<< "\"PID\":" << processes[i].PID << ","<< "\"ArrivalTime\":" << processes[i].ArrivalTime << ","<< "\"BurstTime\":" << processes[i].BurstTime << ","<< "\"Priority\":" << processes[i].Priority << ","<< "\"CompletionTime\":" << processes[i].CompletionTime << ","<< "\"WaitingTime\":" << processes[i].WaitingTime << ","<< "\"TurnAroundTime\":" << processes[i].TurnAroundTime<< "}";
        if(i < processes.size() - 1) 
        {
            cout << ",";
        }
    }
    auto chrono_end = chrono::steady_clock::now();
    long double schedulingOverhead = 1e-12 * chrono::duration_cast<chrono::microseconds>(chrono_end - chrono_begin).count();
    cout << "],"<< "\"averageTAT\": " << averageTAT << ","<< "\"averageWT\": " << averageWT << ","<< "\"schedulingOverhead\": " << schedulingOverhead << "}";
    cout.flush();
    return 0;
}
