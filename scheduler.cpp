#include <bits/stdc++.h>
using namespace std;
#define custom_pq priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
#define all(v) v.begin(), v.end()
int originalQuantum = 2;
struct Process {
    int process_id;
    int arrivalTime;
    int burstTime;
    int priority;
    int startTime = 0;
    int completionTime = 0;
    int waitingTime = 0;
    int turnAroundTime = 0;
    int remainingBurstTime;
    bool completionStatus = false;
    int currentWaitingTimeInQueue = 0;
};
struct MultiLevelQueue {
    queue<int> foregroundQueue; 
    deque<int> backgroundQueue;
};
bool sortByProcessId(const Process &a, const Process &b){
    return (a.process_id < b.process_id);
}
bool sortByArrivalTime(const Process &a, const Process &b){
    if(a.arrivalTime == b.arrivalTime){
        return sortByProcessId(a,b);
    }
    return (a.arrivalTime < b.arrivalTime);
}
bool sortByPriority(const Process &a, const Process &b){
    if(a.priority == b.priority){
        return sortByArrivalTime(a,b);
    }
    return (a.priority < b.priority);
}

void applyAging(custom_pq &pq , int agingInterval , int PriorityIncrease , vector<Process> &processes){
    custom_pq new_pq;
    while(pq.size()){
        int index = pq.top().second;
        int priority = pq.top().first;
        if(processes[index].currentWaitingTimeInQueue < agingInterval){
            //do nothing , 
            new_pq.push(pq.top());
        }
        else{
            // decrease waiting time and increase priority
            processes[index].currentWaitingTimeInQueue -= agingInterval;
            processes[index].priority = max(0 , processes[index].priority - PriorityIncrease);
            new_pq.push(pq.top());
        }
        pq.pop();
    }
    pq = new_pq;
}

void increase_currentWaitingTimeInQueue(custom_pq &pq , vector<Process>&processes){
    custom_pq new_pq;
    while(pq.size()){
        int index = pq.top().second;
        int priority = pq.top().first;
        processes[index].currentWaitingTimeInQueue ++;
        new_pq.push(pq.top());
        pq.pop();
    }
    pq = new_pq;
}

void firstComeFirstServe(vector<Process>& processes){
    sort(processes.begin(),processes.end(),sortByArrivalTime);
    int n = processes.size();
    int time = processes[0].arrivalTime;
    for (auto& p : processes){
        time = max(time , p.arrivalTime);

        p.completionTime = time + p.burstTime;
        p.turnAroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnAroundTime - p.burstTime;

        time += p.burstTime;
    }
}


void shortestJobFirst(vector<Process>& processes){
    sort(processes.begin(),processes.end(),sortByArrivalTime);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // pair of burst time and index
    int j = 0;
    int time = processes[0].arrivalTime;
    int completed = 0;
    int n = processes.size();
    while(completed != n){
        while(j<n && processes[j].arrivalTime <= time){
            pq.push(make_pair(processes[j].burstTime, j));
            j++;
        }
        if (!pq.empty()){
            int index = pq.top().second;
            int arrivalTime = processes[index].arrivalTime;
            int burstTime = processes[index].burstTime;
            pq.pop();

            processes[index].completionTime = (time + burstTime);
            processes[index].turnAroundTime = (processes[index].completionTime - arrivalTime);
            processes[index].waitingTime = (processes[index].turnAroundTime - burstTime);
            time += burstTime;
            completed ++;
        }
        else{
            if (j < n) {
                time = processes[j].arrivalTime;
            }
            else break;
        }
    }
}


void shortestRemaining_CPU_BurstTime(vector<Process>& processes){
    sort(processes.begin(),processes.end(),sortByArrivalTime);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    int j = 0;
    int time = processes[0].arrivalTime;
    int completed = 0;
    int n = processes.size();
    int last_instant = 2e6;
    for (int time = 0; time < last_instant; time++){
        
        while(j<n && processes[j].arrivalTime == time){
            pq.push(make_pair(processes[j].burstTime, j));
            j++;
        }
        if (!pq.empty()){
            int index = pq.top().second;
            int remainingTime = pq.top().first;
            pq.pop();

            int arrivalTime = processes[index].arrivalTime;
            int burstTime   = processes[index].burstTime;

            if (remainingTime == 1){ // process finished
                processes[index].completionTime = time + 1;
                processes[index].turnAroundTime = (processes[index].completionTime - arrivalTime);
                processes[index].waitingTime = (processes[index].turnAroundTime - burstTime);
                completed ++;
            }
            else{
                pq.push(make_pair(remainingTime - 1, index));
            }
        }
        if(completed == n)break;
    }
}


void roundRobin(vector<Process>& processes , int originalQuantum){
    sort(processes.begin(),processes.end(),sortByArrivalTime);
    queue<pair<int,int>>q;  //[index , remaining burst time]
    int j=0;
    int time = 0;
    int completed = 0;
    int n = processes.size();
    int runningQuantum = originalQuantum;
    if(processes[j].arrivalTime == 0){
        q.push(make_pair(j,processes[j].burstTime));
        j++;
    }
    int last_instant = 2e6;
    for(int time = 0;time < last_instant;time++){
        if(!q.empty()){
            int index = q.front().first;
            q.front().second = q.front().second-1;
            int remainingBurstTime = q.front().second;
            int arrivalTime = processes[index].arrivalTime;
            int burstTime = processes[index].burstTime;
            runningQuantum--;
            while(j<n && processes[j].arrivalTime == time+1){
                q.push(make_pair(j,processes[j].burstTime));
                j++;
            }

            if(remainingBurstTime==0){
                processes[index].completionTime = time+1;
                processes[index].turnAroundTime = (processes[index].completionTime - arrivalTime);
                processes[index].waitingTime = (processes[index].turnAroundTime - burstTime);
                runningQuantum = originalQuantum;
                q.pop();
            }
            else if(runningQuantum==0){
                q.pop();
                q.push(make_pair(index,remainingBurstTime));
                runningQuantum = originalQuantum;
            }
        }
        while(j<n && processes[j].arrivalTime==time+1){
            q.push(make_pair(j,processes[j].burstTime));
            j++;
        }
    }
}

void PriorityScheduling(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), sortByArrivalTime);
    int currentTime = processes[0].arrivalTime;
    int n = processes.size();
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //[priority , index]
    int completed = 0;
    int j = 0;

    while (completed < n) {
        while (j < n && !processes[j].completionStatus && processes[j].arrivalTime <= currentTime) {
            pq.push(make_pair(processes[j].priority, j));
            j++;
        }

        if (!pq.empty()) {
            int idx = pq.top().second;
            pq.pop();

            int arrivalTime = processes[idx].arrivalTime;
            int burstTime = processes[idx].burstTime;

            processes[idx].completionTime = currentTime + burstTime;
            processes[idx].turnAroundTime = processes[idx].completionTime - arrivalTime;
            processes[idx].waitingTime = processes[idx].turnAroundTime - burstTime;

            currentTime += burstTime;
            completed++;
        } else {
            if (j < n) {
                currentTime = processes[j].arrivalTime;
            }
        }
    }
}

// void MultiLevelQueueScheduling(vector<Process> &processes){

// }

void addToMLQ(vector<Process>& processes , MultiLevelQueue &mlq , int currentTime , int Priority_Threshold){
    int n = processes.size();
    for(int i=0;i<n;i++) {
        if(currentTime == processes[i].arrivalTime){
            if(processes[i].priority <= Priority_Threshold) {
                mlq.foregroundQueue.push(i);
            } 
            else{
                mlq.backgroundQueue.push_back(i);
            }
        }
    }
}
void MultiLevelQueueScheduling(vector<Process>& processes , int TimeQuantum) {
    sort(processes.begin(),processes.end(),sortByArrivalTime);
    int n = processes.size();
    for(int i=0;i < n;i++) {
        processes[i].remainingBurstTime = processes[i].burstTime;
    }
    int Priority_Threshold = 10;
    int currentTime = 0;
    int completed = 0;
    vector<bool> visited(n, false);
    MultiLevelQueue mlq;
    while(completed < n) {
        addToMLQ(processes,mlq,currentTime,Priority_Threshold);

        if(mlq.foregroundQueue.empty() && mlq.backgroundQueue.empty()) {
            currentTime++;
            continue;
        }
        else if(mlq.foregroundQueue.size() > 0) {
            int idx = mlq.foregroundQueue.front();
            mlq.foregroundQueue.pop();

            if(!visited[idx]) {
                processes[idx].startTime = currentTime;
                visited [idx] = true;
            }
            if(processes[idx].remainingBurstTime > TimeQuantum) {
                processes[idx].remainingBurstTime -= TimeQuantum;
                int tempTime = currentTime + TimeQuantum;

                // edge case if new background processes are introduced during quantum time
                for(int k=currentTime+1; k < tempTime;k++) {
                    addToMLQ(processes,mlq,k,Priority_Threshold);
                }

                currentTime = tempTime;
                mlq.foregroundQueue.push(idx);
            } 
            else {
                int remainingTime = processes[idx].remainingBurstTime;
                processes[idx].remainingBurstTime = 0;
                int tempTime = currentTime + remainingTime;

                // edge case if new background processes are introduced during quantum time
                for(int k=currentTime + 1; k < tempTime;k++) {
                    addToMLQ(processes,mlq,k,Priority_Threshold);
                }

                currentTime = tempTime;
                processes[idx].completionTime = currentTime;
                processes[idx].turnAroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnAroundTime - processes[idx].burstTime;
                completed++;
            }
        }
        else if(mlq.backgroundQueue.size() > 0) {
            int idx = mlq.backgroundQueue.front();
            mlq.backgroundQueue.pop_front();

            if(!visited[idx]) {
                processes[idx].startTime = currentTime;
                visited[idx] = true;
            }

            processes[idx].remainingBurstTime--;
            currentTime++;

            if(processes[idx].remainingBurstTime > 0) {
                mlq.backgroundQueue.push_front(idx);
            } 
            else {
                processes[idx].completionTime = currentTime;
                processes[idx].turnAroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnAroundTime - processes[idx].burstTime;
                completed++;
            }
        }
    }
}

void auto_schedule(std::vector<Process> & processes){
    int n = processes.size();
    int mx_priority = 0 , mn_priority = 1e7;
    int mx_arrival = 0 , mn_arrival = 1e7;
    int THRESHOLD = 10;
    int LOAD = 5;
    for(auto &p: processes){
        mx_priority = max(mx_priority,p.priority);
        mn_priority = min(mn_priority,p.priority);
        mx_arrival = max(mx_arrival,p.priority);
        mn_arrival = min(mn_arrival,p.priority);
    }
    if(mx_priority-mn_priority >= THRESHOLD){
        MultiLevelQueueScheduling(processes,originalQuantum);
    }
    else if(mx_arrival-mn_arrival <= 8){
        shortestRemaining_CPU_BurstTime(processes);
    }
    else if(mx_arrival-mn_arrival > THRESHOLD+LOAD){
        roundRobin(processes,originalQuantum);
    }
    else{
        // if all other conditions are false the easiest algorithm to implement will be FCFS    
        firstComeFirstServe(processes);
    }
}

void displayProcesses(vector<Process> &processes) {
    int n = processes.size();
    long double total_waiting_time = 0;
    long double total_turnaround_time = 0;
    sort(processes.begin(),processes.end(),sortByArrivalTime);
    for (const auto& process : processes) {
        std::cout << process.process_id << "\t"
                  << process.arrivalTime << "\t\t" 
                  << process.burstTime << "\t\t"
                  << process.completionTime<<"\t\t"
                  << process.turnAroundTime << "\t\t"
                  << process.waitingTime << "\n";
    }

}

vector<int> parseInput(const string& input) {
    istringstream iss(input); 
    vector<int> numbers;
    int number;

    while (iss >> number) {
        numbers.push_back(number);
    }

    return numbers;
}
vector<Process> createProcesses(const vector<int>& processIds,const vector<int>& arrivalTimes,const vector<int>& burstTimes,const vector<int>& priorities) {
    int n = processIds.size();
    vector<Process> processes(n);

    for (int i = 0; i < n; ++i) {
        processes[i].process_id = processIds[i];
        processes[i].arrivalTime = arrivalTimes[i];
        processes[i].burstTime = burstTimes[i];
        if (!priorities.empty() && i < priorities.size()) {
            processes[i].priority = priorities[i];
        } else {
            processes[i].priority = 1;  // Default priority
        }
    }

    return processes;
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        cerr << "Error: Insufficient arguments provided.\n";
        return 1;
    }

    string algo = argv[1];
    vector<int> processIds = parseInput(argv[2]);
    vector<int> arrivalTimes = parseInput(argv[3]);
    vector<int> burstTimes = parseInput(argv[4]);
    vector<int> priorities = parseInput(argv[5]);
    if (argc >= 7 && argv[6] != nullptr && argv[6][0] != '\0') {
        originalQuantum = stoi(argv[6]);
    }

    // Validate input sizes
    if (processIds.size() != arrivalTimes.size() || 
        arrivalTimes.size() != burstTimes.size() || 
        (priorities.size() > 0 && priorities.size() != processIds.size())) {
        cerr << "Error: Input data sizes do not match.\n";
        return 1;
    }

    vector<Process> processes = createProcesses(processIds,arrivalTimes,burstTimes,priorities);



    // cout << "Algorithm: " << algo << endl;


    if (algo == "FCFS") {
        firstComeFirstServe(processes);
    } else if (algo == "SJF") {
        shortestJobFirst(processes);
    } else if (algo == "SRTF") {
        shortestRemaining_CPU_BurstTime(processes);
    } else if (algo == "RR") {
        roundRobin(processes, originalQuantum);
    }
    else if(algo == "PS"){
        PriorityScheduling(processes);
    }
    else if(algo == "MLQ"){
        MultiLevelQueueScheduling(processes,originalQuantum);
    }
    else if(algo == "AUTO"){
        auto_schedule(processes);
    }
    else {
        cerr << "Unsupported algorithm: " << algo << endl;
        return 1;
    }

    displayProcesses(processes);

    return 0;
}
