#include<iostream>
#include<fstream>
#include<list>
#include<sstream>
#include<string>
using namespace std;

//process nodes
struct node{
	string processName;
	int priority;
	int burst;
	int arrival;
	int turnaround_time=0;
	int wait_time=0;
	bool alive=true;
	struct node *next;
};
//returns the node of the runstate
node* findRunState(string run,node* q){
	node* temp=q;
	node* answer=new node();
	while(temp->next!=NULL){
		if(temp->next->processName==run&&temp->next->alive){ //if its running state and alive
			answer=temp->next;
		}
		temp=temp->next;
	}
	return answer;
}
	
//finds the process name of the highest priority in the ready queue
string findHighestPriority(string run,node *q){
	node* temp=q;
	node* highestPriority=new node();
	while(temp->next!=NULL){
		//if current nodes priority is greater and processname is not same as before and alive
		if(temp->next->priority>highestPriority->priority&&temp->next->processName!=run&&temp->next->alive){
			highestPriority=temp->next;
		}
		temp=temp->next;
	}
	return highestPriority->processName;
}	

//increments and decrements appropriate data
void incrementDecrement(string run,node* q){
	node* temp=q;
	while(temp->next!=NULL){
		if(temp->next->processName==run&&temp->next->alive){ //if it is runstate and alive
			temp->next->burst--;
			temp->next->turnaround_time++;
		}
		else if(temp->next->processName!=run&&temp->next->alive){ //if its waiting and alive
			temp->next->wait_time++;
			temp->next->turnaround_time++;
		}
		temp=temp->next;
	}
}

//returns true if a process with same priority exists in the ready queue
bool roundRobin(string run,node* q){
	node* temp=q;
	while(temp->next!=NULL){
		if(temp->next->priority==(findRunState(run,q)->priority)&&temp->next->alive){
			if(temp->next->processName!=findRunState(run,q)->processName)
				return true;
		}
		temp=temp->next;
	}
	return false;
}

//print statement
void print(node* q){
	node* temp=q;
	while(temp->next!=NULL){
		cout<<temp->next->processName<<" : "<<temp->next->processName<<", ";
		cout<<temp->next->turnaround_time<<", ";
		cout<<temp->next->wait_time<<endl;
		temp=temp->next;
	}
}

//returns true if all of the processes have finished 
bool finished(node*q){
	node* temp=q;
	while(temp->next!=NULL){
		if(temp->next->alive) return false;
		temp=temp->next;
	}
	return true;
}
			
int main(){
	ifstream theFile("process.txt");

	string processName;
	int priority;
	int burst;
	int arrival;
	int turnaround_time;
	int wait_time;

	node *readyQueue=new node();	//singly linked list ready queue
	node *rqPointer=readyQueue;		//pointer to the ready queue to help add nodes into ready queue
	string runState="";				//runstate
	int t=0;						//counter for while loop
	int counter=0;					//counter for quantom
	
	while(t<=96){	
		while(theFile>>processName>>priority>>burst>>arrival){
			if(arrival==t){
				node* newNode=new node();
				newNode->processName=processName;
				newNode->priority=priority;
				newNode->burst=burst;
				newNode->arrival=arrival;
				
				//store
				rqPointer->next=newNode;
				rqPointer=rqPointer->next;
				//assign runstate
				if(runState==""){
					runState=newNode->processName;
					cout<<"TIME: "<<t<< " P_n: "<<runState<<" P_r: "<<"Empty"<<endl;
				}
				//if newly arrived process priority is greater than current running states priority then swap
				if(newNode->priority>findRunState(runState,readyQueue)->priority){
					string current=runState;
					//if runstate has finished assign new run state
					if(findRunState(runState,readyQueue)->burst==0){
						findRunState(runState,readyQueue)->alive=false;
						runState=findHighestPriority(runState,readyQueue);
						counter=0;
						cout<<"TIME: "<<t<< " P_n: "<<runState<<" P_r: "<<current<<endl;
					}
					else{
						runState=newNode->processName;
						counter=0;
						cout<<"TIME: "<<t<< " P_n: "<<runState<<" P_r: "<<current<<endl;
					}
				}
			}
		}
		//if it finished executing then kill the process and assign a new runstate
		if(findRunState(runState,readyQueue)->burst==0){
			string current=runState;
			findRunState(runState,readyQueue)->alive=false; //kill the process
			//checking if is finished
			if((finished(readyQueue))) break;
			runState=findHighestPriority(runState,readyQueue);  //assign new run state
			counter=0;											//reset counter for quantom
			cout<<"TIME: "<<t<< " P_n: "<<runState<<" P_r: "<<current<<endl;
		}
		//if counter has reached quantom
		else if(counter==10){
			if(roundRobin(runState,readyQueue)){	//if roundrobin exists
				string current=runState;
				runState=findHighestPriority(runState,readyQueue);	//assign new run state
				cout<<"TIME: "<<t<< " P_n: "<<runState<<" P_r: "<<current<<endl;
			}
			counter=0;	//reset counter for quantom
		}
		
		//increment and decrement
		incrementDecrement(runState,readyQueue);
		
		//increment t and counter for quantom
		counter++;
		t++;
		
		//reset file
		theFile.clear();
		theFile.seekg(0);
		
	}
	//print statements 
	print(readyQueue);
}
