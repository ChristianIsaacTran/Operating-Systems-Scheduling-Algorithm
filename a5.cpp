/*
# =====================================================================
# Title             : a5.cpp
# Description       : This program peforms multiple process schedulign
algorithms and writes the output to different .out files. # Author            :
Christian Tran (R11641653) # Date              : 3/30/2024 # Usage             :
With the Makefile for compiling and sbatch command in HPCC. Has to include #
Notes             : requires HPCC and sbatch command, and requires the input
file argument
=====================================================================
*/

// Library imports
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <deque>
#include <iterator>

using namespace std; // Import namespace to not have to call std every time.

//Process Object Class
class ProcessObj{
  public:
    string pName; //Process name
    int arrTime; //Process arrival time
    int total_serTime; //Total time the process needs to run
    int timeRan; //Keeps track of how long this process has run so far
    int timeRemain; //Keeps track of remaining time it needs to complete
    int been_waiting; //Keeps track of how long the process has been waiting 
    double ProRatio; //The process's ratio
};


// Global Variables
//const int max_args = 2; //Max number of arguments allowed from command line
deque<ProcessObj> proDQ; //deque of processes
int max_args = 2; // max number of input arguments

// use ofstream to create the output files for each category

ofstream fcfs("fcfs.out");
ofstream rr_10("rr_10.out");
ofstream rr_40("rr_40.out");
ofstream spn("spn.out");
ofstream srt("srt.out");
ofstream hrrn("hrrn.out");
ofstream fb("feedback.out");


/* Function name: printlist()
   Purpose: utility function that prints out the main input deque to check if it is correct
   Return type: Void
   Input Params: nothing 
   Works?: Confirmed Yes 3/30/2024
*/
void printlist (deque<ProcessObj> inputDQ){
  for(int i = 0; i < inputDQ.size(); i++){
    cout << inputDQ.at(i).pName << "\t" << inputDQ.at(i).arrTime << "\t" << inputDQ.at(i).total_serTime << "\t" << inputDQ.at(i).timeRan << "\t" << inputDQ.at(i).timeRemain << "\t" << inputDQ.at(i).been_waiting << "\t" << inputDQ.at(i).ProRatio << endl; 
  }
}

//Overloaded function for string deques
void printlist (deque<string> inputDQ){
  for(int i = 0; i < inputDQ.size(); i++){
    cout << inputDQ.at(i) << endl; 
  }
}

/* Function name: first_come_first_served()
   Purpose: Performs the FCFS scheduling algorithm and writes output to fcfs.out
   Return type: Void
   Input Params: nothing 
   Works?: Confirmed Yes 3/30/2024
*/
void first_come_first_served() {
  deque<ProcessObj> tempDQ = proDQ; //deque for operations

  while(!tempDQ.empty()){ //While copyProDQ still has processes inside it

    tempDQ.front().timeRan = tempDQ.front().timeRan + 10; //Update timeRan on every iteration
    
    if(tempDQ.front().timeRan <= tempDQ.front().total_serTime){ //if the process has not reached it's total time needed, print process name to fcfs.out file
      fcfs << tempDQ.front().pName << endl;
    }
    else{ //If the process has reached/exceeded it's required time, pop the process out of the queue and move on to next process
      tempDQ.pop_front();
    }
  }
}

/* Function name: round_robin_10()
   Purpose: Performs the round robin scheduling algorithm at time quantum 10 and writes output to rr_10.out
   Return type: Void
   Input Params: nothing 
   Works?: Confirmed Yes 3/30/2024
*/
void round_robin_10() {
  deque<ProcessObj> tempDQ = proDQ; //deque for operations
  deque<ProcessObj> runningProcesses; //deque for arrived and running processes in round robin
  deque<string> runProcessNames; //deque for names to end while loop
  ProcessObj tempObj; //temporary object holder

  //Fill runProcessNames with the names of the processes
  for(int k = 0; k < tempDQ.size(); k++){
    runProcessNames.push_back(tempDQ.at(k).pName);
  }
  
  int current_time = 0; //current system time
  bool tempObjFlag = false; //To check if tempObj exists

  while(!runProcessNames.empty()){ //While runProcessNames still has processes in it
    //Add the new arrivals first, and then move the popped process to back of list
    for(int i = 0; i < tempDQ.size(); i++){ //Checks if any processes arrive at this current time, and add it to the queue
      if(tempDQ.at(i).arrTime == current_time){  //also checks if add_one is off
        runningProcesses.push_back(tempDQ.at(i));
      }
    }

    //Add the stored tempObj after adding new arrivals to queue
    if(tempObjFlag){
      runningProcesses.push_back(tempObj);
    }

    //note: only run one process per time quantum, run the front process of the queue
    //Add 10 ms to the front process
    runningProcesses.front().timeRan = runningProcesses.front().timeRan + 10; 
    //write the ran process to rr_10
    rr_10 << runningProcesses.front().pName << endl;

    if(runningProcesses.front().timeRan >= runningProcesses.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
      for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
        if(runningProcesses.front().pName == runProcessNames.at(p)){
          runProcessNames.erase(runProcessNames.begin()+p);
        }
      }
      //Turn off tempObjFlag
      tempObjFlag = false;

      //Then removes it from the runningProcesses deque
      runningProcesses.pop_front();
    }
    else{ //If front process has NOT finished running, then move it to the back of the queue
      //Then move front process to back of queue
      tempObj = runningProcesses.front(); //Store the front in the temporary object variable
      tempObjFlag = true; //Mark that the temp object exists
      runningProcesses.pop_front(); //Remove the front process in the queue
    }
    current_time = current_time + 10; //updates the current_time by 10 ms
  }
}

/* Function name: round_robin_40()
   Purpose: Performs the round robin scheduling algorithm at time quantum 40 and writes output to rr_40.out
   Return type: Void
   Input Params: nothing 
   Works?: Confirmed Yes 3/31/2024
*/
void round_robin_40() {
  deque<ProcessObj> tempDQ = proDQ; //deque for operations
  deque<ProcessObj> runningProcesses; //deque for arrived and running processes in round robin
  deque<string> runProcessNames; //deque for names to end while loop
  ProcessObj tempObj; //temporary object holder

  //Fill runProcessNames with the names of the processes
  for(int k = 0; k < tempDQ.size(); k++){
    runProcessNames.push_back(tempDQ.at(k).pName);
  }

  int current_time = 0; //current system time
  bool tempObjFlag = false; //To check if tempObj exists
  int quantum_limit = 0; //Counter to check if the process ran 40ms quantum
  
  while(!runProcessNames.empty()){ //While runProcessNames still has processes in it
    //Add the new arrivals first, and then move the popped process to back of list
    for(int i = 0; i < tempDQ.size(); i++){ //Checks if any processes arrive at this current time, and add it to the queue
      if(tempDQ.at(i).arrTime == current_time){  //also checks if add_one is off
        runningProcesses.push_back(tempDQ.at(i));
      }
    }

    //Add the stored tempObj after adding new arrivals to queue
    if(tempObjFlag){
      runningProcesses.push_back(tempObj);
    }
    
    //note: only run one process per time quantum, run the front process of the queue
    //Add 10 ms to the front process
    runningProcesses.front().timeRan = runningProcesses.front().timeRan + 10; 
    quantum_limit = quantum_limit + 10; //Add 10 to quantum limit to keep track of how many times we stayed on this process
    //write the ran process to rr_40
  
    rr_40 << runningProcesses.front().pName << endl;

    if(runningProcesses.front().timeRan >= runningProcesses.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
      for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
        if(runningProcesses.front().pName == runProcessNames.at(p)){
          runProcessNames.erase(runProcessNames.begin()+p);
        }
      }
      //Turn off tempObjFlag
      tempObjFlag = false;

      //Then removes it from the runningProcesses deque
      runningProcesses.pop_front();

      //reset quantum_limit
      quantum_limit = 0;
    }
    else if(quantum_limit == 40 && runningProcesses.size() >= 2){ //If front process has NOT finished running, then move it to the back of the queue
      //Then move front process to back of queue
      tempObj = runningProcesses.front(); //Store the front in the temporary object variable
      tempObjFlag = true; //Mark that the temp object exists
      runningProcesses.pop_front(); //Remove the front process in the queue

      //reset quantum_limit
      quantum_limit = 0;
    }
    current_time = current_time + 10; //updates the current_time by 10 ms
  }
}

/* Function name: shortest_process_next()
   Purpose: performs the SPN algorithm and writes output to spn.out
   Return type: Void
   Input Params: nothing 
   Works?: Confirmed yes 3/31/2024
*/
void shortest_process_next() {
  deque<ProcessObj> tempDQ = proDQ; //deque for operations
  deque<ProcessObj> runningProcesses; //deque for arrived and running processes
  deque<string> runProcessNames; //deque for names to end while loop

  ProcessObj tempObj; //temporary object holder
  
  //Fill runProcessNames with the names of the processes
  for(int k = 0; k < tempDQ.size(); k++){
    runProcessNames.push_back(tempDQ.at(k).pName);
  }
  
  int current_time = 0; //current system time
  int min_pos = 0; //Keeps track of the current position of the minimum process
  int min_time = 0; //Keeps track of the current minimum runtime 
  bool first_flag = true; //Checks the initial iteration once

  while(!runProcessNames.empty()){ //While runProcessNames still has processes in it
    for(int i = 0; i < tempDQ.size(); i++){ //Checks if any processes arrive at this current time, and add it to the queue
      if(tempDQ.at(i).arrTime == current_time){  
        runningProcesses.push_back(tempDQ.at(i));
      }
    }

    if(first_flag){ //This is the initial check for the shortest process
      //set initial minimum to the front element for comparison
      min_time = runningProcesses.front().total_serTime;
      min_pos = 0;

      //Compare all required processes and get the minimum process to run 
      if(!runningProcesses.empty()){ //As long as runningProcesses is not empty, check for minimum
        for(int n = 0; n < runningProcesses.size(); n++){
          if(runningProcesses.at(n).total_serTime < min_time){ //Check if any other process in the list has a shorter run time
            min_time = runningProcesses.at(n).total_serTime;
            min_pos = n;

          }
        }
        //move the minimum process found to the front of the queue
        tempObj = runningProcesses.at(min_pos); //Temporarily store minimum process into tempObj
        runningProcesses.erase(runningProcesses.begin()+min_pos); //Delete the minimum process
        runningProcesses.push_front(tempObj); //Push the minimum found process to the front of the queue
      }

      //Set first_flag to false.
      first_flag = false;
    }
    
    //note: only run one process per time quantum, run the front process of the queue
    //Add 10 ms to the front process
    runningProcesses.front().timeRan = runningProcesses.front().timeRan + 10; 
    //write the ran process to spn
    spn << runningProcesses.front().pName << endl;
    
    

    if(runningProcesses.front().timeRan >= runningProcesses.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
      for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
        if(runningProcesses.front().pName == runProcessNames.at(p)){
          runProcessNames.erase(runProcessNames.begin()+p);
        }
      }
      
      //Then removes it from the runningProcesses deque
      runningProcesses.pop_front();

      //set initial minimum to the front element for comparison
      min_time = runningProcesses.front().total_serTime;
      min_pos = 0;

      //Compare all required processes and get the minimum process to run 
      if(!runningProcesses.empty()){ //As long as runningProcesses is not empty, check for minimum
        for(int n = 0; n < runningProcesses.size(); n++){
          if(runningProcesses.at(n).total_serTime < min_time){ //Check if any other process in the list has a shorter run time
            min_time = runningProcesses.at(n).total_serTime;
            min_pos = n;
            
          }
        }
        //move the minimum process found to the front of the queue
        tempObj = runningProcesses.at(min_pos); //Temporarily store minimum process into tempObj
        runningProcesses.erase(runningProcesses.begin()+min_pos); //Delete the minimum process
        runningProcesses.push_front(tempObj); //Push the minimum found process to the front of the queue
      }
    }
    current_time = current_time + 10; //updates the current_time by 10 ms

  }
}


/* Function name: shortest_remaining_time()
   Purpose: performs the SRT algorithm and writes output to srt.out
   Return type: Void
   Input Params: nothing 
   Works?: Confirmed yes 3/31/2024
*/
void shortest_remaining_time() {
  deque<ProcessObj> tempDQ = proDQ; //deque for operations
  deque<ProcessObj> runningProcesses; //deque for arrived and running processes 
  deque<string> runProcessNames; //deque for names to end while loop

  ProcessObj tempObj; //temporary object holder

  //Fill runProcessNames with the names of the processes
  for(int k = 0; k < tempDQ.size(); k++){
    runProcessNames.push_back(tempDQ.at(k).pName);
  }

  int current_time = 0; //current system time
  int min_pos = 0; //Keeps track of the current position of the minimum process
  int min_time = 0; //Keeps track of the current minimum remaining time

  
  while(!runProcessNames.empty()){ //While runProcessNames still has processes in it
    //Add the new arrivals first, and then move the popped process to back of list
    for(int i = 0; i < tempDQ.size(); i++){ //Checks if any processes arrive at this current time, and add it to the queue
      if(tempDQ.at(i).arrTime == current_time){  //also checks if add_one is off
        runningProcesses.push_back(tempDQ.at(i));
      }
    }


    //set initial minimum to the front element for comparison
    min_time = runningProcesses.front().timeRemain;
    min_pos = 0;

    //Compare all required processes and get the minimum process to run 
    if(!runningProcesses.empty()){ //As long as runningProcesses is not empty, check for minimum
      for(int n = 0; n < runningProcesses.size(); n++){
        if(runningProcesses.at(n).timeRemain < min_time){ //Check if any other process in the list has a shorter run time
          min_time = runningProcesses.at(n).timeRemain;
          min_pos = n;
        }
      }
      //move the minimum process found to the front of the queue
      tempObj = runningProcesses.at(min_pos); //Temporarily store minimum process into tempObj
      runningProcesses.erase(runningProcesses.begin()+min_pos); //Delete the minimum process
      runningProcesses.push_front(tempObj); //Push the minimum found process to the front of the queue
    }



    
    
    //note: only run one process per time quantum, run the front process of the queue
    //Add 10 ms to the front process
    runningProcesses.front().timeRan = runningProcesses.front().timeRan + 10; 
    runningProcesses.front().timeRemain = runningProcesses.front().timeRemain - 10;
    //write the ran process to srt
    srt << runningProcesses.front().pName << endl;

    


    if(runningProcesses.front().timeRan >= runningProcesses.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
      for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
        if(runningProcesses.front().pName == runProcessNames.at(p)){
          runProcessNames.erase(runProcessNames.begin()+p);
        }
      }

      //Then removes it from the runningProcesses deque
      runningProcesses.pop_front();

      //set initial minimum to the front element for comparison
      min_time = runningProcesses.front().timeRemain;
      min_pos = 0;

      //Compare all required processes and get the minimum process to run 
      if(!runningProcesses.empty()){ //As long as runningProcesses is not empty, check for minimum
        for(int n = 0; n < runningProcesses.size(); n++){
          if(runningProcesses.at(n).timeRemain < min_time){ //Check if any other process in the list has a shorter run time
            min_time = runningProcesses.at(n).timeRemain;
            min_pos = n;
          }
        }
        //move the minimum process found to the front of the queue
        tempObj = runningProcesses.at(min_pos); //Temporarily store minimum process into tempObj
        runningProcesses.erase(runningProcesses.begin()+min_pos); //Delete the minimum process
        runningProcesses.push_front(tempObj); //Push the minimum found process to the front of the queue
      }
    }
    current_time = current_time + 10; //updates the current_time by 10 ms
  }
}



/* Function name: highest_response_ratio_next()
   Purpose: performs the HRRN algorithm and writes output to hrrn.out
   Return type: Void
   Input Params: nothing 
   Works?: Confirmed yes 3/31/2024
*/
void highest_response_ratio_next() {
  deque<ProcessObj> tempDQ = proDQ; //deque for operations
  deque<ProcessObj> runningProcesses; //deque for arrived and running processes in round robin
  deque<string> runProcessNames; //deque for names to end while loop

  ProcessObj tempObj; //temporary object holder

  //Fill runProcessNames with the names of the processes
  for(int k = 0; k < tempDQ.size(); k++){
    runProcessNames.push_back(tempDQ.at(k).pName);
  }

  int current_time = 0; //current system time
  int max_pos = 0; //Keeps track of the current position of the highest ratio
  double max_ratio = 0; //Keeps track of the current maximum ratio

  while(!runProcessNames.empty()){ //While runProcessNames still has processes in it
    for(int i = 0; i < tempDQ.size(); i++){ //Checks if any processes arrive at this current time, and add it to the queue
      if(tempDQ.at(i).arrTime == current_time){  
        runningProcesses.push_back(tempDQ.at(i));
      }
    }

    //note: only run one process per time quantum, run the front process of the queue
    //Add 10 ms to the front process
    runningProcesses.front().timeRan = runningProcesses.front().timeRan + 10; 
    //write the ran process to spn
    hrrn << runningProcesses.front().pName << endl;
    

    //increase every other process's wait time by 10ms
    for(int f = 0; f < runningProcesses.size(); f++){
      if(runningProcesses.at(f).pName != runningProcesses.front().pName){
        runningProcesses.at(f).been_waiting = runningProcesses.at(f).been_waiting + 10;
      }
    }
    
    if(runningProcesses.front().timeRan >= runningProcesses.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
      for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
        if(runningProcesses.front().pName == runProcessNames.at(p)){
          runProcessNames.erase(runProcessNames.begin()+p);
        }
      }

      //Then removes it from the runningProcesses deque
      runningProcesses.pop_front();

      //Calculate the ratio for each process
      for(int y = 0; y < runningProcesses.size(); y++){
        runningProcesses.at(y).ProRatio = (double) (runningProcesses.at(y).been_waiting + runningProcesses.at(y).total_serTime) / runningProcesses.at(y).total_serTime;
      }

      //set initial maximum to the front element for comparison
      max_ratio = runningProcesses.front().ProRatio;
      max_pos = 0;

      //Compare all required processes and get the minimum process to run 
      if(!runningProcesses.empty()){ //As long as runningProcesses is not empty, check for minimum
        for(int n = 0; n < runningProcesses.size(); n++){
          if(runningProcesses.at(n).ProRatio > max_ratio){ //Check if any other process in the list has a shorter run time
            max_ratio = runningProcesses.at(n).ProRatio;
            max_pos = n;

          }
        }
        //move the maximum process found to the front of the queue
        tempObj = runningProcesses.at(max_pos); //Temporarily store minimum process into tempObj
        runningProcesses.erase(runningProcesses.begin()+max_pos); //Delete the minimum process
        runningProcesses.push_front(tempObj); //Push the minimum found process to the front of the queue
      }
    }
    current_time = current_time + 10; //updates the current_time by 10 ms
  }
}

/* Function name: feedback()
   Purpose: Performs the feedback scheduling algorithm at time quantum 10 and writes output to feedback.out
   Return type: Void
   Input Params: nothing 
   Works?: 
*/
void feedback() {
  deque<ProcessObj> tempDQ = proDQ; //deque for operations
  deque<ProcessObj> RP1; //deque 1 - Highest priority to lowest priority
  deque<ProcessObj> RP2; //deque 2
  deque<ProcessObj> RP3; //deque 3
  deque<ProcessObj> RP4; //deque 4 
  deque<ProcessObj> RP5; //deque 5 
  deque<ProcessObj> RP6; //deque 6 -Lowest priority
  
  deque<string> runProcessNames; //deque for names to end while loop
  ProcessObj tempObj; //temporary object holder

  //Fill runProcessNames with the names of the processes
  for(int k = 0; k < tempDQ.size(); k++){
    runProcessNames.push_back(tempDQ.at(k).pName);
  }

  int current_time = 0; //current system time

  while(!runProcessNames.empty()){ //While runProcessNames still has processes in it
    //Add the new arrivals first, and then move the popped process to back of list
    for(int i = 0; i < tempDQ.size(); i++){ //Checks if any processes arrive at this current time, and add it to the queue
      if(tempDQ.at(i).arrTime == current_time){  //also checks if add_one is off
        RP1.push_back(tempDQ.at(i));
      }
    }

    //RP1 Processing
    if(!RP1.empty()){ //if RP1 has processes in it, run the front() process for this time slice
      RP1.front().timeRan = RP1.front().timeRan + 10;  //Run process for 10ms 
      fb << RP1.front().pName << endl; //write to feedback.out file
      
      //after running front process, check if the process is finished:
      if(RP1.front().timeRan >= RP1.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
        for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
          if(RP1.front().pName == runProcessNames.at(p)){
            runProcessNames.erase(runProcessNames.begin()+p);
          }
        }
        //Then removes it from the runningProcesses deque
        RP1.pop_front();
      }
      else{ //If the front process is NOT finished, then move it to the next lower priority queue and remove it from this queue
        RP2.push_back(RP1.front()); //Move process over to the next queue
        RP1.pop_front(); //Remove front process from this queue
      }
      current_time = current_time + 10; //updates the current_time by 10 ms
      continue; //move on to the next iteration
    }

    //RP2 Processing
    if(!RP2.empty()){ //if RP1 has processes in it, run the front() process for this time slice
      RP2.front().timeRan = RP2.front().timeRan + 10;  //Run process for 10ms 
      fb << RP2.front().pName << endl; //write to feedback.out file

      //after running front process, check if the process is finished:
      if(RP2.front().timeRan >= RP2.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
        for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
          if(RP2.front().pName == runProcessNames.at(p)){
            runProcessNames.erase(runProcessNames.begin()+p);
          }
        }
        //Then removes it from the runningProcesses deque
        RP2.pop_front();
      }
      else{ //If the front process is NOT finished, then move it to the next lower priority queue and remove it from this queue
        RP3.push_back(RP2.front()); //Move process over to the next queue
        RP2.pop_front(); //Remove front process from this queue
      }
      current_time = current_time + 10; //updates the current_time by 10 ms
      continue; //move on to the next iteration
    }

    //RP3 Processing
    if(!RP3.empty()){ //if RP1 has processes in it, run the front() process for this time slice
      RP3.front().timeRan = RP3.front().timeRan + 10;  //Run process for 10ms 
      fb << RP3.front().pName << endl; //write to feedback.out file
  
      //after running front process, check if the process is finished:
      if(RP3.front().timeRan >= RP3.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
        for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
          if(RP3.front().pName == runProcessNames.at(p)){
            runProcessNames.erase(runProcessNames.begin()+p);
          }
        }
        //Then removes it from the runningProcesses deque
        RP3.pop_front();
      }
      else{ //If the front process is NOT finished, then move it to the next lower priority queue and remove it from this queue
        RP4.push_back(RP3.front()); //Move process over to the next queue
        RP3.pop_front(); //Remove front process from this queue
      }
      current_time = current_time + 10; //updates the current_time by 10 ms
      continue; //move on to the next iteration
    }

    //RP4 Processing
    if(!RP4.empty()){ //if RP1 has processes in it, run the front() process for this time slice
      RP4.front().timeRan = RP4.front().timeRan + 10;  //Run process for 10ms 
      fb << RP4.front().pName << endl; //write to feedback.out file

      //after running front process, check if the process is finished:
      if(RP4.front().timeRan >= RP4.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
        for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
          if(RP4.front().pName == runProcessNames.at(p)){
            runProcessNames.erase(runProcessNames.begin()+p);
          }
        }
        //Then removes it from the runningProcesses deque
        RP4.pop_front();
      }
      else{ //If the front process is NOT finished, then move it to the next lower priority queue and remove it from this queue
        RP5.push_back(RP4.front()); //Move process over to the next queue
        RP4.pop_front(); //Remove front process from this queue
      }
      current_time = current_time + 10; //updates the current_time by 10 ms
      continue; //move on to the next iteration
    }

    //RP5 Processing
    if(!RP5.empty()){ //if RP1 has processes in it, run the front() process for this time slice
      RP5.front().timeRan = RP5.front().timeRan + 10;  //Run process for 10ms 
      fb << RP5.front().pName << endl; //write to feedback.out file

      //after running front process, check if the process is finished:
      if(RP5.front().timeRan >= RP5.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
        for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
          if(RP5.front().pName == runProcessNames.at(p)){
            runProcessNames.erase(runProcessNames.begin()+p);
          }
        }
        //Then removes it from the runningProcesses deque
        RP5.pop_front();
      }
      else{ //If the front process is NOT finished, then move it to the next lower priority queue and remove it from this queue
        RP6.push_back(RP5.front()); //Move process over to the next queue
        RP5.pop_front(); //Remove front process from this queue
      }
      current_time = current_time + 10; //updates the current_time by 10 ms
      continue; //move on to the next iteration
    }

    //RP6 Processing
    if(!RP6.empty()){ //if RP1 has processes in it, run the front() process for this time slice
      RP6.front().timeRan = RP6.front().timeRan + 10;  //Run process for 10ms 
      fb << RP6.front().pName << endl; //write to feedback.out file
      
      //after running front process, check if the process is finished:
      if(RP6.front().timeRan >= RP6.front().total_serTime){ //If the current front process is finished, remove it from the queue entirely
        for(int p = 0; p < runProcessNames.size(); p++){ //Finds the name of the process and removes it
          if(RP6.front().pName == runProcessNames.at(p)){
            runProcessNames.erase(runProcessNames.begin()+p);
          }
        }
        //Then removes it from the runningProcesses deque
        RP6.pop_front();
      }
      
      current_time = current_time + 10; //updates the current_time by 10 ms
      continue; //move on to the next iteration
    }
  }
}





int main(int argc, char *argv[]) {
  // error Check for max number of arguments with error message and termination
  // of program
 if (argc > max_args || argc == 1) {
    cout << "ERROR: Not enough arguments or too many arguments found. "
            "Terminating program..."
         << endl;
    return 0;
  }

  // pull file name from argv (arguments) list and pass to reader thread to open
  // and read file:
  char *fileN = argv[1];

  // Create fstream to read file argument and open it
  ifstream inputfile_arg;
  inputfile_arg.open(fileN);
  //inputfile_arg.open("input.in");

  // Check to see if file is open or not/if it exists
  if (inputfile_arg.is_open()) {
    //Temporary Input file variables
    string processName; // Name of process
    int arrive_time; // arrival time of process
    int serve_time;  // total service time the process needs

    while (inputfile_arg >> processName >> arrive_time >> serve_time) { //Gets every input line until EOF
      ProcessObj temp_process; //Create new process object

      //Set object variables to input file variables
      temp_process.pName = processName;
      temp_process.arrTime = arrive_time;
      temp_process.total_serTime = serve_time;
      temp_process.timeRan = 0; //Initially set all process's time ran to 0
      temp_process.timeRemain = serve_time; //Initially set all process's remaining time to total service time (no processes have run yet)
      temp_process.been_waiting = 0; //Initially set waiting time to 0 (for use in HRRN later)

      //Insert Process Object into process vector
      proDQ.push_back(temp_process);
    }
  }
  else { // error message and exit program if the file is not open, or not
           // found
    cout << "ERROR: File not found or invalid argument/filename. Terminating..." << endl;
      exit(0); // Exits program
  }

  //Display input list to ensure its correct
  //printlist(proDQ);
  
  //Execute the scheduling algorithms.
  first_come_first_served(); //FCFS algorithm
  round_robin_10(); //Round robin algorithm at time quantum 10
  round_robin_40(); //Round robin algorithm at time quantum 40
  shortest_process_next(); //SPN algorithm
  shortest_remaining_time(); //SRT algorithm
  highest_response_ratio_next(); //HRRN algorithm
  feedback(); //Feedback algorithm 
  
  return 0;
}