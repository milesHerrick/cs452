#include <bits/stdc++.h>
using namespace std;
int quantum;
int interval;
//you can change how many processes you run by changing the txt file below VVV
ifstream infile("500Kprocesses.txt");
struct process{
	int id; //PID
	int burst; //burst value
	int arrival; //arrival time
	int priority; //priority value
	int ageIndex; //age index
	int age;
	bool checkPromotion() {  //Increases age as well as checks if it needs to be promoted
		age++;
		if(age == interval){
			age = 0;
			return true;
		}else{
			return false;
		}
	}
	void increasePriority(int t){ //Increases priority if age interval has been reached
		priority = priority + ageIndex;
		if(priority > 99){              
			priority = 99;
		}
		
	}
	void demotion(int timeQuantum){ //Demotes priority if time quantum has been used up
		//priority = priority - (timeQuantum / 4);
		priority = priority - 20;
		if(priority < 1){
			priority = 1;
		}
	}
};
vector <process> proc; //list of processes

void initialize(){
	string temp1;
	int id; //PID
	int burst; //default burst value
	int arrival; //default arrival time
	int priority; //default priority
	int ageIndex; //default age index
	process temp; //temp process
	process *tempPtr; //temp process pointer
	tempPtr = &temp;
	getline(infile, temp1); //parse line of title strings
	while(infile){ //while(hasNext)
		if(infile >> id >> burst >> arrival >> priority >> ageIndex){
			temp.id = id; //assigns PID to temp process 
			temp.burst = burst; //assigns burst value
			temp.arrival = arrival; //assigns arrival time
			temp.priority = priority; //assigns priority
			temp.ageIndex = ageIndex; //assigns age index
			if((burst > 0 && arrival >= 0 && 0 < priority && priority <= 100 && ageIndex > 0)){
				proc.insert(proc.end(), temp);
			}	
		}
	}	
}


bool compare(process &a, process &b)
{
	if (a.arrival != b.arrival)
		return a.arrival < b.arrival;
	else
		return a.priority > b.priority;
}

//Find turnaround time for a finished process
int calcTurnaround(process temp, int time){
	//calculate both and insert those into arrays to calculate the averages at the end
	int turnaround = time - temp.arrival;
	return turnaround;
}

//Find wait time for a finished process
int calcWait(process temp, int turnaround){
	int wait = turnaround - temp.burst;
	return wait;
}

map<int, queue<process>> addProcess(map<int, queue<process>> &priorityMap, process temp){
	if(priorityMap.find(temp.priority) != priorityMap.end()){
		priorityMap.find(temp.priority)->second.push(temp);
	}
	else{
		queue<process> newQ;
		newQ.push(temp);
		priorityMap.insert(pair<int, queue<process>>(temp.priority, newQ));
	}
	return priorityMap;
}

map<int, queue<process>> ageUp(map<int, queue<process>> &priorityMap, int t){ //Ages every process in the map
	map<int, queue<process>>::iterator it;
	map<int, queue<process>> newMap;
	process temp;

	for (it = priorityMap.begin(); it != priorityMap.end(); it++){ //Parses through each queue in the map
		int n = it->second.size(); //stops size from shrinking while loop is running
		for(int i = 0; i < n; i++){ //Parses through each element in the queue
			temp = it->second.front();
			it->second.pop();
			if (temp.checkPromotion()){  //increase age and checks if it needs a promotion
				temp.increasePriority(t); //increases priority
				addProcess(priorityMap, temp); //pushes it back into the queue with the updated priority
			} else{
				it->second.push(temp); //gets put back into same queue in the new map since priority doesn't change but age increases
			}  
		}
		
	}
	return priorityMap; 
}




void priorityScheduling(vector<process> proc, int n, int quantum, int interval){
    sort(proc.begin(), proc.end(), compare);

	vector <int> turnaroundSet;
	vector <int> waitSet;

	bool isRunning = true;
	bool burst = false;
	int burstCount = 0;
	int index = 0;
	int highestPriority;
	int tempTurnaround;
	int tempWait;
	int clock = 0;
	process current;
	int completed = 0;
	int quantumCount = 0; //separate timer for the time quantum
	map<int, queue<process>> priorityMap;
	cout << "clock tick: " << clock << endl;
	while(isRunning){
		quantumCount++;


		while(index < proc.size() && proc.at(index).arrival == clock){//adds process to queue at specified arrival time
			priorityMap = addProcess(priorityMap, proc.at(index));
			//cout << proc.at(index).id << endl;
			index++;
		}

		if(burst){
			burstCount--;
			if(burstCount == 0){
				burst = false;
				quantumCount = 0;
				cout << endl;
				cout << "clock tick: " << clock << endl;
				cout << "pid: " << current.id << " terminates " << burstCount << " " << quantumCount << " " << current.priority << endl;
				completed++;

			}  else if(quantumCount == quantum){ //Checks if the time quantum has been reached while a process is running
				burst = false;
				current.burst = burstCount;
				current.demotion(quantum); //Demotes priority of running process
				cout << endl;
                cout << "clock tick: " << clock << endl;
				cout << "pid: " << current.id << " expired, demoted to queue with priority " << current.priority << endl;
				priorityMap = addProcess(priorityMap, current);
				burstCount = 0; //Resets burst timer
				quantumCount = 0; // Resets time quantum timer
			}
		}

		if(!burst){
			if(!priorityMap.empty()){
				highestPriority = priorityMap.rbegin()->first;

				queue<process> tempq = priorityMap.find(highestPriority)->second;

				process temp = tempq.front();
				tempq.pop();
				int qsize = tempq.size();
				process next;
				for(int i = 0; i < qsize; i++){
					next = tempq.front();
					tempq.pop();
					if(next.id < temp.id){
						tempq.push(temp);
					}
					else{
						tempq.push(next);
					}
				}
				current = temp;
				current.age = 0;

				priorityMap.find(highestPriority)->second = tempq;
				if(priorityMap.find(highestPriority)->second.empty()){
					priorityMap.erase(highestPriority);
				}
				//cout << "clock tick: " << clock << endl;
				cout << "pid: " << current.id << " runs "  << burstCount << " " << quantumCount << " " << current.priority << endl;
				burst = true;
				burstCount = current.burst;
				quantumCount = 0;
			}
			else{
				burstCount = 0;
			}
		}
		priorityMap = ageUp(priorityMap, clock);
		clock++;
			
		//findavgTime(proc, n);
	}
}

int main(){
	cout << "input time quantum: ";
	cin >> quantum;
	cout << "input ageing interval: ";
	cin >> interval;
	cout << endl;
	
	initialize();
	priorityScheduling(proc, proc.size(), quantum, interval);

	return 0;
}
