#include <bits/stdc++.h>
using namespace std;
int quantum;
int interval;
//you can change how many processes you run by changing the txt file below VVV
ifstream infile("100processes.txt");
struct process
{
	int id; //PID
	int burst; //burst value
	int arrival; //arrival time
	int priority; //priority value
	int ageIndex; //age index
	int burstLeft;
	int quantumLeft;
	int intervalLeft;
	bool completed = false;

};
vector <process> proc; //list of processes

bool compare(process &a, process &b)
{
	if (a.arrival != b.arrival)
		return a.arrival < b.arrival;
	else
		return a.priority > b.priority;
}

void initialize()
{
	string temp1;
	int id; //PID
	int burst; //default burst value
	int arrival; //default arrival time
	int priority; //default priority
	int ageIndex; //default age index
	int i = 0; //iterator
	process temp; //temp process
	process *tempPtr; //temp process pointer
	tempPtr = &temp;
	getline(infile, temp1); //parse line of title strings
	while(infile){ //while(hasNext)
	if(infile >> id 
			  >> burst
			  >> arrival 
			  >> priority 
			  >> ageIndex){
			
			
			temp.id = id; //assigns PID to temp process 
			temp.burst = burst; //assigns burst value
			temp.burstLeft = burst;
			temp.arrival = arrival; //assigns arrival time
			temp.priority = priority; //assigns priority
			temp.ageIndex = ageIndex; //assigns age index
			temp.intervalLeft = interval;
			temp.quantumLeft = quantum;
			proc.insert(proc.end(), temp);
			//cout << "id is: " << temp.id << " " << temp.burst << " " << temp.arrival << " " << temp.priority << " " << temp.ageIndex << endl;
			
	}
	i++;
	}

	
}

void findWaitTime(vector<process> proc, int size, int wait[]) 
{ 
    // waiting time for first process is 0 
    wait[0] = 0; 
  
    // calculating waiting time 
    for (int  i = 1; i < size; i++ ) 
        wait[i] =  proc[i-1].burst + wait[i-1] ; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime(vector<process> proc, int size, int wait[], int turnaround[]){ 
    // calculating turnaround time by adding 
    // burst[i] + wait[i] 
    for (int  i = 0; i < size; i++) 
        turnaround[i] = proc[i].burst + wait[i]; 
} 
  
//Function to calculate average time 
void findavgTime(vector<process> proc, int n){ 
    int wait[n], turnaround[n], total_wait = 0, total_turnaround = 0; 
  
    //Function to find waiting time of all processes 
    findWaitTime(proc, n, wait); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(proc, n, wait, turnaround); 

	//cout << "\nProcesses  "<< " Burst time  "
    //     << " Waiting time  " << " Turn around time\n"; 
    // Calculate total waiting time and total turn 
    // around time 
    for (int  i=0; i<n; i++) 
    {
        total_wait = total_wait + wait[i]; 
        total_turnaround = total_turnaround + turnaround[i];
	} 
	cout << "\nNumber of Process Scheduled: " << proc.size() << endl;
	cout << "Average Wait Time: " << (double)total_wait / proc.size() << endl;
	cout << "Average Turnaround Time: " << (double)total_turnaround / proc.size() << endl;  
} 

void priorityScheduling(vector<process> proc, int n, int quantum, int interval){ 
    // Sort processes by priority 
    sort(proc.begin(), proc.end(), compare);
	int clock = 1; // clock tick
	int completed = 0;
	int prev = 0;
	process temp; // temporary process
	process temp2; // temporary process
	vector<process> readyQueue;
	//debug
	//for(int i = 0; i < n; i++){
	//	temp = proc[i];
	//	cout << temp.id << " " << temp.arrival << " " << temp.burst << endl;
	//}
	
	map<int, vector<process>, greater<int>> priorityMap;
	while(completed < n){
		if(!proc.empty()){
			temp = proc.front();
			if(temp.arrival <= clock){ //adds process to queue at specified arrival time
				proc.erase(proc.begin());
				priorityMap[temp.priority].push_back(temp);
			}
		}

		if(!priorityMap.empty()){
			//cout << priorityMap.begin()->first << endl; //prints current highest priority
			temp2 = priorityMap.begin()->second.back();
			priorityMap.begin()->second.pop_back();
			cout << "clock tick: " << clock << endl;
			cout << temp2.id << " " << temp2.burstLeft << " " << temp2.priority <<  endl;
			//new logic - simplier, but doesnt work yet
			if(temp2.burstLeft > 0){
				if(temp2.quantumLeft > 0){
					//runs
					cout << "pid: " << temp2.id << " runs" << endl;
					temp2.burstLeft--;
					temp2.quantumLeft--;
					
				}
				else{
					//demote
						temp2.priority = (temp2.priority - quantum / 4) % 100;
						cout << "pid: " << temp2.id << " demoted to queue with priority " << temp2.priority << endl;
						temp2.quantumLeft = quantum;
						
				}
				if(temp2.intervalLeft > 0){
					temp2.intervalLeft--;	
				}
				else{
					//promote
					temp2.priority = (temp2.priority + temp2.ageIndex) % 100;
					temp2.intervalLeft = interval;
					cout << "pid: " << temp2.id << " promoted to queue with priority " << temp2.priority << endl;				
				}
				priorityMap.begin()->second.push_back(temp2);
			}
			else{
				completed++;
				cout << "pid: " << temp2.id << " terminates" << endl;
			}

				/* old logic - works "better"
				if(temp2.quantumLeft > 0 && temp2.burstLeft > 0){
					if(temp2.quantumLeft > 0){ //can run
						temp2.quantumLeft--;
						if(temp2.burstLeft > 0){ //has burst left
							
							cout << "pid: " << temp2.id << " runs" << endl;
							temp2.burstLeft--;
						}						
					}
					else if(temp2.quantumLeft <= 0){ //demoted
						temp2.priority = (temp2.priority - quantum / 4) % 100;
						cout << "pid: " << temp2.id << " demoted to queue with priority " << temp2.priority << endl;
						temp2.quantumLeft = quantum;
						priorityMap.begin()->second.push_back(temp2);
					}
					if(temp2.intervalLeft <= 0){ //promoted
						temp2.priority = (temp2.priority + temp2.ageIndex) % 100;
						temp2.intervalLeft = interval;
						cout << "pid: " << temp2.id << " promoted to queue with priority " << temp2.priority << endl;
						priorityMap.begin()->second.push_back(temp2);
						
					}
					else if(temp2.intervalLeft > 0){
						temp2.intervalLeft--;
						priorityMap.begin()->second.push_back(temp2);
					}
				}					
				else if(temp2.burstLeft <= 0){ //is completed
						completed++;
						cout << "pid: " << temp2.id << " terminates" << endl;
						
				}
				*/	
		}
			clock++;
			//cout << clock << endl; //print current tick
	}
	//findavgTime(proc, n);
} 

int main()
{
	cout << "input time quantum: ";
	cin >> quantum;
	cout << "input ageing interval: ";
	cin >> interval;
	
	initialize();
	priorityScheduling(proc, proc.size(), quantum, interval);

	return 0;
}
