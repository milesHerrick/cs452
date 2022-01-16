
// C++ implementation of the Priority Based(Preemptive) algorithm
#include <cstdlib>
#include <iostream>
#include <queue>
using namespace std;
class process {
public:
    
    time_t AT = 0,
           BT_left = 0, 
           BT = 0, 
           temp_BT = 0;
    int priority = 0;
  

    // Overload operator '<' w.r.t arrival
    // time because arrival time is the
    // first priority even greater than
    // priority of process and priority_queue
    // pops out the greatest value first
    // so we need to replace '<' with '>' inorder
    // to pop out smallest value
    friend bool operator<(const process& a, const process& b)
    {
        return a.AT > b.AT;
    }
};
  
// Function to implement pop_index()
process pop_index(priority_queue<process>* main_queue,
                  int index)
{
    priority_queue<process> rm_index;
    int i;
    process p;
  
    switch (index) {
    case 0:
        p = (*main_queue).top();
        (*main_queue).pop();
        break;
    default:
        for (i = 0; i < index; i++) {
            rm_index.push((*main_queue).top());
            (*main_queue).pop();
        }
        p = (*main_queue).top();
        (*main_queue).pop();
  
        while (!(*main_queue).empty()) {
            rm_index.push((*main_queue).top());
            (*main_queue).pop();
        }
        (*main_queue) = rm_index;
        break;
    }
    return p;
}
  
// Function to implement maximum priority w.r.t
//priority and also 2nd argument has boolean
//variable because we need to specify
// True=highest number as highest priority
// False=lowest number as highest priority
int max_priority(priority_queue<process> main_priority_queue,
                 int limit, bool high)
{
    int max = -1;
    if (high == 1) {
        while (!main_priority_queue.empty()
               && main_priority_queue.top().AT <= limit) {
            if (main_priority_queue.top().priority > max)
                max = main_priority_queue.top().priority;
            main_priority_queue.pop();
        }
    }
    else {
        while (!main_priority_queue.empty()
               && main_priority_queue.top().AT <= limit) {
            if (max == -1 || main_priority_queue.top().priority < max)
                max = main_priority_queue.top().priority;
            main_priority_queue.pop();
        }
    }
    return max;
}
  
// Function to implement maximum priority index
int max_priority_index(priority_queue<process> main_queue, int limit, bool high)
{
    int max = -1, i = 0, index = 0;
    if (high == 1) {
        while (!main_queue.empty() && main_queue.top().AT <= limit) {
            if (main_queue.top().priority > max) {
                max = main_queue.top().priority;
                index = i;
            }
            main_queue.pop();
            i++;
        }
    }
    else {
        while (!main_queue.empty()
               && main_queue.top().AT <= limit) {
            if (max == -1 || main_queue.top().priority < max) {
                max = main_queue.top().priority;
                index = i;
            }
            main_queue.pop();
            i++;
        }
    }
    return index;
}
  
// Function to implement priority based Preemptive scheduling
priority_queue<process> Priority_P_run(priority_queue<process> ready_queue, queue<process>* gantt, bool high)
{
    int temp;
    priority_queue<process> completion_queue;
    process p;
    time_t clock = 0;
    if (high == 1) {
        while (!ready_queue.empty()) {
            while (clock < ready_queue.top().AT) {
                p.temp_BT++;
                clock++;
            }
            
            p = pop_index(&ready_queue,
                          max_priority_index(ready_queue, clock, high));
            while (p.BT_left > 0 && (ready_queue.empty() || clock < ready_queue.top().AT)) {
                p.temp_BT++;
                p.BT_left--;
                clock++;
            }
            if (p.BT_left == 0) {
                p.temp_BT = 0;
                completion_queue.push(p);
            }
            else {
                p.AT = clock;
                p.temp_BT = 0;
                ready_queue.push(p);
            }
        };
    }
    else {
        while (!ready_queue.empty()) {
            while (clock < ready_queue.top().AT) {
                p.temp_BT++;
                clock++;
            }
            p = pop_index(&ready_queue,
                          max_priority_index(ready_queue,clock, high));
            temp = max_priority(ready_queue, clock, high);
  
            while (p.BT_left > 0 && (ready_queue.empty()
                                     || clock < ready_queue.top().AT
                                     || p.priority <= max_priority(ready_queue, clock, high))) {
                p.temp_BT++;
                p.BT_left--;
                clock++;
            }
            if (p.BT_left == 0) {
                p.temp_BT = 0;
                completion_queue.push(p);
            }
            else {
                p.AT = clock;
                p.temp_BT = 0;
                ready_queue.push(p);
            }
        }
    }
  
    return completion_queue;
}
  
// Set data on the basis of given table
priority_queue<process> set_sample_data()
{
    priority_queue<process> ready_queue;
    process temp;
    temp.AT = 0;
    temp.BT = 4;
    temp.priority = 2;
    ready_queue.push(temp);
    temp.AT = 1;
    temp.BT = 2;
    temp.priority = 4;
    ready_queue.push(temp);
    temp.AT = 2;
    temp.BT = 3;
    temp.priority = 6;
    ready_queue.push(temp);
    temp.AT = 3;
    temp.BT = 5;
    temp.priority = 10;
    ready_queue.push(temp);
    temp.AT = 4;
    temp.BT = 1;
    temp.priority = 8;
    ready_queue.push(temp);
    temp.AT = 5;
    temp.BT = 4;
    temp.priority = 12;
    ready_queue.push(temp);
    temp.AT = 6;
    temp.BT = 6;
    temp.priority = 9;
    ready_queue.push(temp);
    return ready_queue;
}
  
// Function to get total Waiting Time

  
// Function to get total Turn Around Time

// Function to get total Completion Time

  
// Function to get total Response Time

  
// Function to display main queue
void disp(priority_queue<process> main_queue, bool high)
{
    int i = 0, temp, size = main_queue.size();
    priority_queue<process> tempq = main_queue;
    double temp1;

        cout << endl;

    cout << "| Burst Time | Completion Time ";
    cout << "| Turnaround Time | Waiting Time | Response Time |";

        cout << " Priority |" << endl;

    while (!main_queue.empty()) {
        //temp = to_string(main_queue.top().p_no).length();
        //cout << '|' << string(6 - temp / 2 - temp % 2, ' ')
        //     << main_queue.top().p_no << string(7 - temp / 2, ' ');
        //temp = to_string(main_queue.top().start_AT).length();
        //cout << '|' << string(7 - temp / 2 - temp % 2, ' ')
        //     << main_queue.top().start_AT << string(7 - temp / 2, ' ');
        //temp = to_string(main_queue.top().BT).length();
        //cout << '|' << string(6 - temp / 2 - temp % 2, ' ')
        //     << main_queue.top().BT << string(6 - temp / 2, ' ');
        //temp = to_string(main_queue.top().CT).length();
        //cout << '|' << string(8 - temp / 2 - temp % 2, ' ')
        //     << main_queue.top().CT << string(9 - temp / 2, ' ');
        //temp = to_string(main_queue.top().TAT).length();
        //cout << '|' << string(8 - temp / 2 - temp % 2, ' ')
        //     << main_queue.top().TAT << string(9 - temp / 2, ' ');
        //temp = to_string(main_queue.top().WT).length();
        //cout << '|' << string(7 - temp / 2 - temp % 2, ' ')
        //     << main_queue.top().WT << string(7 - temp / 2, ' ');
        //temp = to_string(main_queue.top().RT).length();
        //cout << '|' << string(7 - temp / 2 - temp % 2, ' ')
        //     << main_queue.top().RT << string(8 - temp / 2, ' ');
        //if (high == true) {
        //    temp = to_string(main_queue.top().priority).length();
        //    cout << '|' << string(5 - temp / 2 - temp % 2, ' ')
        //         << main_queue.top().priority << string(5 - temp / 2, ' ');
        //}
        //cout << "|\n";
        main_queue.pop();
    }
    cout << endl;
    //emp1 = get_total_WT(tempq);
    cout << "Number of process scheduled: " << size << endl;
    cout << "Average waiting time :- " << temp1 / size << endl;
    //temp1 = get_total_TAT(tempq);
    cout << "Average turnaround time :- " << temp1 / size << endl;

}
  

// Driver Code
int main()
{
    // Initialise Ready and Completion Queue
    priority_queue<process> ready_queue, completion_queue;
  
    // queue for Gantt Chart
    queue<process> gantt;
    ready_queue = set_sample_data();
  
    // Function call to find completion data
//3rd argument has true passed becuase we have set
//highest number = highest priority
    completion_queue = Priority_P_run(ready_queue, &gantt, true);
  
    // Display Completion Queue as true in
//2nd argument to display priority
    disp(completion_queue, true);

    return 0;
}