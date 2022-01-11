#include <iostream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;
#define N 100001
int k, n, q;

int now = 0;
struct process
{
	int id; //number
	int pos; //posi after arrange
	int arrive; //arrivetime
	int work; //worktime
	int begin; //begintime
	int end; //endtime
	int turnaround; //turnaroundtime
	int wait; //waittime
	bool in; //in(out)preplist?
	bool finish; //(un)finished?
	int rest; //restofworktime
	int block_p; //recentblockpoint
	int block_t; //blocktime
} proc[N];
queue <process> ready; //readyQ
queue <process> block; //BlockQ
queue <process> block_new; //BlockQ

bool cmp_FCFS(process a, process b)
{
	if (a.arrive != b.arrive)
		return a.arrive < b.arrive;
	else
		return a.id < b.id;
}

bool cmp_id(process a, process b)
{
	return a.id < b.id;
}

void init()
{
	cout << "input the number of process :";
	cin >> n;
	cout << "input the info: " <<
		endl; //in order
	for (int i = 1; i <= n; i++)
	{
		cout << "process " << i << ": ";
		cin >> proc[i].arrive >> proc[i].work;
		//initialize
		proc[i].id = i;
		proc[i].in = 0;
		proc[i].begin = -1; //not start yet
		proc[i].finish = 0;
		proc[i].block_t = 0;
		proc[i].rest = proc[i].work;
	}
	sort(proc, proc + n + 1, cmp_FCFS);
	for (int i = 1; i <= n; i++)
	{
		proc[i].pos = i;
	}
}

void block_check() //check if a process is invoked from expiration
{
	while (!block.empty()) //look back on the expire Q
	{
		process t2 = block.front();
		block.pop();
		if (t2.block_p != now) 
		{
			if (rand() % 2) 
			{
				t2.block_t += (now - t2.block_p); 
				cout << "clock tick: " << now << ", process" << t2.id << " continue running" << endl;
				ready.push(t2);
			}
			else 
			{
				block_new.push(t2);
			}
		}
		else 
		{
			block_new.push(t2);
		}
	}
	block = block_new; 
	while (!block_new.empty()) 
	{
		block_new.pop();
	}
}

void FCFS()
{
	srand((unsigned int)(time(0))); 
	int left = n; 
	int once_max; 
	int once; 
	while (left > 0) 
	{
		int min_arrive = 0x3f3f3f; 
		int in_flag = 0; 
		for (int i = 1; i <= n; i++)
		{
			if (!proc[i].in && proc[i].arrive < min_arrive)
			{
				min_arrive = proc[i].arrive;
				in_flag = 1;
			}
		}
		if (min_arrive >= now && in_flag)
		{
			
			if (min_arrive > now)
				cout << "clock tick: " << now << " no process running" << endl;
			now = min_arrive;
			for (int i = 1; i <= n; i++)
			{
				if (proc[i].arrive == min_arrive)
				{
					proc[i].in = 1;
					ready.push(proc[i]);
				}
			}
		}
		if (!ready.empty())
		{
			process t = ready.front(); 
			ready.pop();
			if (t.begin == -1) 
			{
				t.begin = now; 
			}
			if (k == 0) //FCFS
			{
				once_max = t.rest; 
			}
			else //RR
			{
				once_max = min(q, t.rest);
				
			}
			once = 0;
			while (once < once_max) 
			{
				now++; 
				once++; 
				t.rest--; 
				for (int i = 1; i <= n; i++)
				{
					if (!proc[i].in && proc[i].arrive == now) 
					{
						proc[i].in = 1; 
						ready.push(proc[i]); 
					}
				}
				block_check(); 
				if (rand() % 10 == 0 && t.rest != 0)	
				{
					t.block_p = now;
					block.push(t);
					break;
				}
			}
			if (t.rest > 0) 
			{
				cout << "clock tick: " << now - once << " PID " << t.id << " runs";
				if (t.block_p == now) //
				{
					cout << ", then expired ";
				}
				else 
				{
					ready.push(t);
				}
				cout << endl;
			}
			else 
			{
				cout << "clock tick: " << now << " PID " << t.id << " terminates" << endl;
				t.end = now; 
				t.finish = 1; 
				t.turnaround = t.end - t.arrive; 
				t.wait = t.turnaround - t.work - t.block_t; 
				proc[t.pos] = t; 
				left--; 
			}
		}
		else 
		{
			int in_block = 0;
			while (1)
			{
				block_check();
				if (left == block.size() && left > 0) 
				{
					in_block++; 
					now++;
				}
				else
				{
					break; 
				}
			}
			if (in_block > 0) 
			{
				cout << "clock tick: " << now - in_block << " - " << now  << " no process running" << endl;
			}
		}
	}
}

void display()
{
	int sum_work = 0, sum_turnaround = 0, sum_wait = 0, sum_block = 0;
	for (int i = 1; i <= n; i++)
	{
		sum_work += proc[i].work;
		sum_turnaround += proc[i].turnaround;
		sum_block += proc[i].block_t;
		sum_wait += proc[i].wait;
		//cout << left << setw(10) << proc[i].id << setw(10) << proc[i].arrive << setw(10) << proc[i].work << setw(10) << proc[i].begin << setw(10) << proc[i].end << setw(10) << proc[i].turnaround << setw(10) << proc[i].wait << setw(10) << proc[i].block_t << endl;
	}
	cout << "Average Turnaround Time: " << (double)sum_turnaround / n << endl;
	cout << "Average Wait Time: " << (double)sum_wait / n << endl;
	cout << "Number of Process Scheduled: " << n << endl;
	return;
}

int main()
{
	init();
	FCFS();
	display();
	return 0;
}
