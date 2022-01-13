#include <bits/stdc++.h>
using namespace std;
#define N 100000000
int k, n, q;
int quantum;
int interval;
int now = 0;
ifstream infile("100processes.txt");
struct process
{
	int id; //PID
	int burst; //burst value
	int arrival; //arrival time
	int priority; //priority value
	int ageIndex; //age index

	int pos; //posi after arrange
	int work; //worktime - burst?
	int begin; //begintime
	int end; //endtime
	int turnaround; //turnaroundtime
	int wait; //waittime - time quantum or age index
	bool in; //in(out)preplist?
	bool finish; //(un)finished?
	int rest; //restofworktime
	int block_p; //recentblockpoint
	int block_t; //blocktime
};
vector <process> proc; //list of processes

bool compare(process &a, process &b)
{
	if (a.priority != b.priority)
		return a.priority > b.priority;
	else
		return a.id < b.id;
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
			temp.arrival = arrival; //assigns arrival time
			temp.priority = priority; //assigns priority
			temp.ageIndex = ageIndex; //assigns age index
			
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
void findTurnAroundTime(vector<process> proc, int size, int wait[], int turnaround[]) 
{ 
    // calculating turnaround time by adding 
    // burst[i] + wait[i] 
    for (int  i = 0; i < size; i++) 
        turnaround[i] = proc[i].burst + wait[i]; 
} 
  
//Function to calculate average time 
void findavgTime(vector<process> proc, int n) 
{ 
    int wait[n], turnaround[n], total_wait = 0, total_turnaround = 0; 
  
    //Function to find waiting time of all processes 
    findWaitTime(proc, n, wait); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(proc, n, wait, turnaround); 

    // Calculate total waiting time and total turn 
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wait = total_wait + wait[i]; 
        total_turnaround = total_turnaround + turnaround[i]; 
	} 
	cout << "Number of Process Scheduled: " << proc.size() << endl;
	cout << "Average Wait Time: " << (double)total_wait / proc.size() << endl;
	cout << "Average Turnaround Time: " << (double)total_turnaround / proc.size() << endl;  
} 
  
void priorityScheduling(vector<process> proc, int n) 
{ 
    // Sort processes by priority 
    sort(proc.begin(), proc.end(), compare); 
    findavgTime(proc, n); 
} 
  
/*
	cout << "Number of Process Scheduled: " << proc.size() << endl;
	cout << "Average Wait Time: " << (double)totalTime / proc.size() << endl;
	cout << "Average Turnaround Time: " << (double)totalTurnaround / proc.size() << endl;
	*/


int main()
{
	cout << "input time quantum: ";
	cin >> quantum;
	cout << "input ageing interval: ";
	cin >> interval;
	
	initialize();
	
	int size = sizeof proc - sizeof proc[0];
	priorityScheduling(proc, proc.size());
	return 0;
}
