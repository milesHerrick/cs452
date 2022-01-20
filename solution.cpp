#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <map>



using namespace std;

class job {
	public:
		int id;
		int burst;
		int arrival;
		int priority;
		int ageindex;
		int currentTime;
		int ageInterval;
		int age; //keeps track of age to check if it needs a promotion
		bool checkPromotion() {  //Increases age as well as checks if it needs to be promoted
			age++;
			if(age == ageInterval){
				age = 0;
				return true;
			}else{
				return false;
			}
    	}
		void increasePriority(int t){ //Increases priority if age interval has been reached
			priority = priority + ageindex;
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
//Comparison for sorting
bool compareArrival(job j1, job j2){
	return (j1.arrival < j2.arrival);
}
//Compares priorities
bool comparePriority(int pri1, int pri2){
	return (pri1 > pri2);
}

//Find turnaround time for a finished process
int calcTurnaround(job process, int time){
	//calculate both and insert those into arrays to calculate the averages at the end
	int turnaround = time - process.arrival;
	return turnaround;
}

//Find wait time for a finished process
int calcWait(job process, int turnaround){
	int wait = turnaround - process.burst;
	return wait;
}

//Adds a process to a queue and the map
map<int, queue<job>> addProcess(std::map<int, queue<job>> &pmap, job process){
	if(pmap.find(process.priority) != pmap.end()){ //Checks if a queue for that priority exists already
		//puts process into existing queue
		pmap.find(process.priority)->second.push(process);
	}
	else{ //Creates a queue for that priority if needed
		queue<job> jobq;
		jobq.push(process);
		pmap.insert(pair<int, queue<job>>(process.priority, jobq)); //inserts queue into the tree using the priority as a key to sort it
	}
	return pmap; //Returns updated map
}

map<int, queue<job>> ageUp(map<int, queue<job>> &pmap, int t){ //Ages every job in the map
	map<int, queue<job>>::iterator it;
	map<int, queue<job>> newMap;
	job tempJ;

	for (it = pmap.begin(); it != pmap.end(); it++){ //Parses through each queue in the map
		int qsize = it->second.size(); //stops size from shrinking while loop is running
		for(int i = 0; i < qsize; i++){ //Parses through each element in the queue
			tempJ = it->second.front();
			it->second.pop();
			if (tempJ.checkPromotion()){  //increase age and checks if it needs a promotion
				tempJ.increasePriority(t); //increases priority
				addProcess(pmap, tempJ); //pushes it back into the queue with the updated priority
			} else{
				it->second.push(tempJ); //gets put back into same queue in the new map since priority doesn't change but age increases
			}  
		}
		
	}
	return pmap; 
}


//Start of main function ----------------------------------------------------------------------------------------------------------------------------------
int main(){
	//The values that are read in through an input file
	int id;
	int burst;
	int arrival;
	int priority;
	int ageindex;
    int ageInterval;
	int timeQuantum;

	std::map<int, queue<job>> pmap;
	
	vector<job> jobs;
	//Takes in input
    cout << "Enter desired age interval: ";
    cin >> ageInterval;
    
    cout << "Enter desired time quantum:";
	cin >> timeQuantum;
	cout << '\n';
    

	string line;
	ifstream myFile;
	int i = 0;
	int number;
	//Takes in input from file
	myFile.open("500Kprocesses.txt"); //Change this to name of file/file path of the text file you are inputting from
	
	getline(myFile,line);
	vector <int> turnaroundSet;
	vector <int> waitSet;
	job tempJob;
	tempJob.age = 0;
	tempJob.ageInterval = ageInterval;

	//read in input

		while(myFile >> id >> burst >> arrival >> priority >> ageindex){
			if(burst > 0 && arrival >= 0 && priority > 0 && priority < 100 && ageindex > 0){
				tempJob.id = id;
				tempJob.burst = burst;
				tempJob.arrival = arrival;
				tempJob.priority = priority;
				tempJob.ageindex = ageindex;
				
				jobs.push_back(tempJob);
			}
		}
    
	myFile.close();

	//sorts the input with lowest arrival time first
	sort(jobs.begin(), jobs.end(), compareArrival);
                                                              
	bool isRunning = true;
	bool inBur = false;
	int time = 0;
	int a = 0;
	int highestpriority;
	int tempTurnaround;
	int tempWait;
	int t = 0;
	job burstProcess;
	int numDone = 0;
	int counter = jobs.size();
	int quantumTimer = 0; //separate timer for the time quantum
	cout << "Clock tick: 0" << '\n';
	
	while(isRunning){ //Each loop is a tick (Make sure to change the upper range when not testing lower numbers of processes)
		quantumTimer++;
        
		
			while(a < jobs.size() && jobs.at(a).arrival == t){ //check if a job arrives, loop in case several arrive at the same tick
				//put it into a queue
				pmap = addProcess(pmap, jobs.at(a));
				//start to check the next one in the list since it is sorted
				a++;
			}
		
		
		if(inBur) { //Checks if a process is currently being ran
			time--;
			if (time == 0) {
				inBur = false;
				quantumTimer = 0; //Resets the time quantum timer
				cout << "Clock tick: " << t << '\n';
				cout << "Pid: " << burstProcess.id << " terminates" << '\n';
				numDone++;

				//Calculates the different times
				tempTurnaround = calcTurnaround(burstProcess, t);
				turnaroundSet.push_back(tempTurnaround);

				tempWait = calcWait(burstProcess, tempTurnaround);
				waitSet.push_back(tempWait);
				
			} else if(quantumTimer == timeQuantum){ //Checks if the time quantum has been reached while a process is running
				inBur = false;
				burstProcess.burst = time;
				burstProcess.demotion(timeQuantum); //Demotes priority of running process
                cout << "Clock tick: " << t << '\n';
				cout << "Pid: " << burstProcess.id << " expired, demoted to queue with priority " << burstProcess.priority << '\n';
				pmap = addProcess(pmap, burstProcess);
				time = 0; //Resets burst timer
				quantumTimer = 0; // Resets time quantum timer
			}
		}
		if(a == numDone){
            isRunning = false;
			inBur = true;
        }
		if(!inBur) { //Checks if a process is not being ran
			if(!pmap.empty()){
				highestpriority = pmap.rbegin()->first; //Grabs the highest key from the map, which is sorted by priority of processes in a queue
			
				queue<job> value = pmap.find(highestpriority)->second;

				//makes sure the next process has a lower ID
				job tempJ = value.front();
				value.pop();
				int qsize = value.size();
				job next;
				for(int m = 0; m < qsize; m++){
					next = value.front();
					value.pop();
					if(next.id < tempJ.id){
						value.push(tempJ);
						tempJ = next;
					} else{
						value.push(next);
					}
				}
				burstProcess = tempJ;
				burstProcess.age = 0;

				pmap.find(highestpriority)->second = value;
				if(pmap.find(highestpriority)->second.empty()){
					pmap.erase(highestpriority);
				}
				cout << "Pid: " << burstProcess.id << " runs" << '\n';
				cout << '\n';
				inBur = true;
				time = burstProcess.burst;
				quantumTimer = 0;
			} else{
				time = 0;
			}
			
		}
		pmap = ageUp(pmap, t); //Ages every process standing by in the map
		t++;
	}
	
    int averageWait = 0;
    int averageTurnaround = 0;

    for(int i = 0; i < jobs.size(); i++){
        averageWait = waitSet.at(i) + averageWait;
        averageTurnaround = turnaroundSet.at(i) + averageTurnaround;
    }
    averageWait = averageWait / waitSet.size();
    averageTurnaround = averageTurnaround / turnaroundSet.size();
    cout << "Number of process scheduled = " << jobs.size() << '\n';
    cout << "Average Waiting Time = " << averageWait << '\n';
    cout << "Average Turnaround Time = " << averageTurnaround << '\n';
}
