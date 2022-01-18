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
	bool completed;

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
			temp.completed = false;
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
	int clock = 0; // clock tick
	int completed = 0;
	int prev = 0;
	process temp; // temporary process
	process temp2; // temporary process
	vector<process> readyQueue;
	/* debug
	for(int i = 0; i < n; i++){
		temp = proc[i];
		cout << temp.id << " " << temp.arrival << endl;
	}
	*/
	map<int, vector<process>, greater<int>> priorityMap;
	while(completed < n){
			temp = proc.front();
				
			if(temp.arrival <= clock){
				//run through all at current tick
				proc.erase(proc.begin());
				priorityMap[temp.priority].push_back(temp);
				//completed++;
				//cout << completed << endl;
				//cout << temp.id << endl;
			}

			if(priorityMap.size() > 0){	//its not really sorting them by priority yet
				temp2 = priorityMap.begin()->second.front();
				priorityMap.begin()->second.erase(priorityMap.begin()->second.begin());
				if(priorityMap.begin()->second.empty()){
					priorityMap.erase(priorityMap.begin()->first);
				}
				cout << temp2.id << " " << temp2.priority << " " << temp2.arrival << endl;
			}
			//cout << priorityMap.begin()->second.front().id;
			//priorityMap.begin()->second.erase(proc.begin());
			//run

	
			/*
			for(map<int, vector<process>, greater<int>>::iterator it = priorityMap.begin(); it != priorityMap.end(); ++it) {
				cout << "priority: " << it->first << endl;
				cout << "Values: ";
				for(int j = 0; j < it->second.size(); j++){
					cout << it->second.at(j).id << " ";
				}
			cout << endl;
			}
			*/
		
			clock++;
			//cout << clock << endl;
		
	}
		
	/*
	for(int i = 0; i < n; i++){
		while(!proc.empty() || !priorityMap.empty()){
			temp = proc[i];
			proc.erase(proc.begin() + i);
  				//std::cout << "Value: " << it->second << std::endl();
			//if(temp.priority < )
				if((temp.arrival == clock && temp.burstLeft > 0)){
					for(int i = 0; i < quantum; i++){
						temp.burstLeft--;
						clock++;
						if(temp.burstLeft < 0){
							temp.burstLeft = 0;
						}
					}
					if(temp.burstLeft > 0){
						priorityMap[temp.priority].push_back(temp);
					}
					else{
						temp.completed = true;
						completed++;
					}
							
				}
		}
	}
	*/
			
			

	

/* print priority queue
for(auto ii=priorityMap.begin(); ii!=priorityMap.end(); ++ii){
	vector <process> inVect = (*ii).second;
	cout << (*ii).first << ": ";
	for (unsigned j=0; j<inVect.size(); j++){
        cout << inVect[j].id << " ";
    }
    cout << endl;
}
*/


	//add processes to a priority map
/*for printing the map of vectors/ The key is their priority, their values are the PID*/
/*
	for(auto ii=priorityMap.begin(); ii!=priorityMap.end(); ++ii){
	   vector <process> inVect = (*ii).second;
		
			while(inVect.size() > 0){
				if(!inVect.empty()){
					temp = inVect.back();
					inVect.pop_back();
					for(int i = 0; i < quantum; i++){
						if(temp.burst > 0){
							temp.burst--;
							clock++;
							inVect.push_back(temp);
							cout << "clock tick: " << clock << " pid: " << temp.id << " burst: " << temp.burst << " priority: " << temp.priority <<  endl;
						}
						else if(temp.burst == 0 || temp.burst < 0){
							clock++;
							if(i == quantum-1){
								completed++;
							}
						}
					}
					//break;
				}
			}	
		}	
*/		

//cout << endl << completed << endl;
	findavgTime(proc, n);
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
