// ***************************************************************
// Names:   Pelagio, Trisha Gail P.
//          Ramirez, Bryce Anthony V.
// Group:   Group 4
// Section: S17
// ***************************************************************


#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int processId;
  int arrivalTime;
  int executionTime;
  int ioBurst;
  int ioFrequency;
  int originalIndex;
  int index;
  int queue;
  int timeAvail;
  int currBurst;
  int IOrun;
  int status;
  int oldQueue;
}Process;

typedef struct {
  int queueId;
  int priority;
  int timeQuantum;
  int n;
  int endProcess;
  int currProcess;
  Process queue[100];
}Queue;

int main(void) {
  // Clear the Screen
  char fileName[1000];
  int i,j;
  int validInput = 1;
  // FILE HANDLING

  printf("Enter file name: ");
  scanf("%s", fileName);

  FILE *in_file = fopen(fileName, "r");
  if(in_file == NULL){
    printf("File not found.\n\n");
    exit(-1);
  }

  // GET X,Y,S VALUES
  int x,y,s;
  fscanf(in_file, "%d", &x);
  fscanf(in_file, "%d", &y);
  fscanf(in_file, "%d", &s);
  // printf("x: %d \ny: %d \ns: %d\n\n",x,y,s);

  int history[y][9999][3];
  int historyNum[y];

  int priority[y];
  int endIter = 0;

  int minQueue = 999999999;
  int curQueue;
  int curIndex;
  int time = 0;
  int start = 0;
  int endIndex;
  int tempIndex;
  int boostReady = 0;
  // INITIALIZE QUEUES
  Queue queues[x];
  for(i=0;i<x;i++){
    if(fscanf(in_file, "%d", &queues[i].queueId) == EOF) {
      validInput = 0;
    }
    if(fscanf(in_file, "%d", &queues[i].priority) == EOF) {
      validInput = 0;
    }
    if(fscanf(in_file, "%d", &queues[i].timeQuantum) == EOF) {
      validInput = 0;
    }
    queues[i].n = 0;
    queues[i].currProcess = 0;
    queues[i].endProcess = -1;
  }

  // INITIALIZE PROCESSES
  Process processes[y];
  for(i = 0; i < y; i++) {

    if(fscanf(in_file, "%d", &processes[i].processId) == EOF){
      validInput = 0;
    }
    if(fscanf(in_file, "%d", &processes[i].arrivalTime) == EOF){
      validInput = 0;
    }
    if(fscanf(in_file, "%d", &processes[i].executionTime) == EOF){
      validInput = 0;
    }
    if(fscanf(in_file, "%d", &processes[i].ioFrequency) == EOF){
      validInput = 0;
    }
    if(fscanf(in_file, "%d", &processes[i].ioBurst) == EOF){
      validInput = 0;
    }
    processes[i].index = 0;
    processes[i].queue = 0;
    processes[i].IOrun = 0;
    processes[i].originalIndex = i;
    processes[i].timeAvail = processes[i].arrivalTime;
    processes[i].currBurst = processes[i].executionTime;
    processes[i].status = 0;
  }

  // ERROR CHECKING
  int w;
  if(fscanf(in_file, "%d", &w)!=EOF) {
    validInput = 0;
  }

  if(x < 2 || x > 5) {
    validInput = 0;
  }
  if(y < 3 || y > 100) {
    validInput = 0;
  }
  if(s < 0){
    validInput = 0;
  }

  if(validInput == 0) {
    printf("The input contains some errors. Please fix these based on the specifications.\n");
  }
  else {
    //SORT PROCESSES BY ARRIVAL TIME
  Process temp;
  for(i = 0; i < y-1; i++){
      for(j = 0; j < y-i-1; j++) {
          if(processes[j].arrivalTime > processes[j+1].arrivalTime) {
              temp =  processes[j];
              processes[j] = processes[j+1];
              processes[j+1] = temp;
          }
      }
  }

  for(i = 0; i < y; i++) {
    processes[i].originalIndex = i;
  }

  // SORT QUEUES BY PRIORITY BY PRIORITY (LOWER INDEX HIGHER PRIORITY)
  Queue queueTemp;
  for(i = 0; i < x-1; i++){
      for(j = 0; j < x-i-1; j++) {
          if(queues[j].priority > queues[j+1].priority) {
              queueTemp = queues[j];
              queues[j] = queues[j+1];
              queues[j+1] = queueTemp;
          }
      }
  }

  printf("\nPrinting Processes: \n");
  for(i = 0; i < y; i++) {
    printf("%d %d %d %d %d %d\n", processes[i].processId, processes[i].arrivalTime, processes[i].executionTime, processes[i].ioBurst, processes[i].ioFrequency, processes[i].originalIndex);
  }

  printf("Printing Queues: \n");
  for(i = 0; i < x; i++) {
    printf("%d %d %d\n", queues[i].queueId, queues[i].priority, queues[i].timeQuantum);
  }
  // STORE PROCESSES TO FIRST QUEUE
  // for(i = 0; i < y; i++) {
  //   queues[0].queue[i] = processes[i];
  //   queues[0].queue[i].index = i;
  //   queues[0].n++;
  //   //printf("Pro %d ",queues[0].queue[i].originalIndex)
  // }
  // printf("Printing Queue 1: \n");
  // for(i = 0; i < queues[0].n; i++) {
  //   printf("%d %d %d %d %d %d \n", queues[0].queue[i].processId, queues[0].queue[i].arrivalTime, queues[0].queue[i].executionTime, queues[0].queue[i].ioBurst, queues[0].queue[i].ioFrequency, queues[0].queue[i].currBurst);
  // }
  // queues[0].endProcess = queues[0].n - 1;

  int boostTrack=0;
  int timeQuanTrack=0;
  int IOTrack = 0;
  int found = 0;
  int boost = 0;
  int yy = y;
  while(endIter<y){
    //search through queues for valid
    //exit loop once found
    //endIter++;
    // check for process that will be inserted to queue here
    for(i = 0; i < y; i++) {
      if(processes[i].arrivalTime==time){
        //printf("yes\n");
        endIndex = queues[0].endProcess;
        queues[0].queue[(endIndex+1) % y] = processes[i];
        queues[0].endProcess = (endIndex+1)%y;
        queues[0].queue[(endIndex+1) % y].index = (endIndex+1) % y;
        queues[0].n = queues[0].n+1;
      }
      //printf("Pro %d ",queues[0].queue[i].originalIndex)
    }
    //printf("\nPrinting Processes: \n");
    // for(i = 0; i < y; i++) {
    //   printf("p%d q%d %d %d %d %d\n", processes[i].processId, processes[i].queue, processes[i].executionTime, processes[i].ioBurst, processes[i].ioFrequency, processes[i].originalIndex);
    // }
    //check for io that will be reinserted to queue here
    for(i = 0; i < y; i++) {
      //printf("p%d q%d %d %d %d %d\n", processes[i].processId, processes[i].queue, processes[i].executionTime, processes[i].ioBurst, processes[i].ioFrequency, processes[i].originalIndex);
      if(processes[i].timeAvail==time && processes[i].ioBurst!=0&&(processes[i].IOrun!=0)){
        int index = processes[i].queue;
        //printf("insert io %d %d %d\n", processes[i].queue, processes[i].processId, processes[i].oldQueue);
        endIndex = queues[index].endProcess;
        queues[index].queue[(endIndex+1) % y] = processes[i];
        queues[index].queue[(endIndex+1) % y].IOrun = processes[i].oldQueue;
        queues[index].endProcess = (endIndex+1)%y;
        queues[index].queue[(endIndex+1) % y].index = (endIndex+1) % y;
        processes[i].index =  (endIndex+1) % y;
        queues[index].n = queues[index].n+1;
        IOTrack = 0;
        if(processes[i].IOrun==processes[i].ioBurst)
          processes[i].IOrun=0;
      }
      //printf("Pro %d ",queues[0].queue[i].originalIndex)
      //printf("\n\n");
    }
    // for(i = 0; i < x; i++) {
    //   for(j = 0; j < y; j++) {
    //     int index = (queues[i].currProcess+j)%y;
    //     if((queues[i].queue[j].ioBurst!=0)&&(queues[i].queue[j].timeAvail==time)&&(queues[i].queue[j].IOrun==queues[i].queue[j].ioBurst)){
    //       printf("insert io\n");
    //       endIndex = queues[i].endProcess;
    //       queues[i].queue[(endIndex+1) % y] = queues[i].queue[j];
    //       queues[i].endProcess = (endIndex+1)%y;
    //       queues[i].queue[(endIndex+1) % y].index = (endIndex+1) % y;
    //       //queues[i].n++;
    //       IOTrack = 0;
    //     }
    //   }
    //   printf("\n\n");
    // }

    int old_index = curIndex;
    int old_queue = curQueue;
    int afterIO;
    //printf("Time: %d\n", time);
    i = 0;
    for(i=0;(i<x) && (found!=1);i++){
      Process current = queues[i].queue[queues[i].currProcess];
      //printf("q %d p%d n:%d timeavail:%d\n", current.queue, queues[i].currProcess, queues[i].n, current.timeAvail);
      if(current.timeAvail<=time && queues[i].n!=0){
        // printf("Current Process: %d Time: %d TimeAvail: %d\n", queues[i].currProcess,time, current.timeAvail);
        curQueue = current.queue;
        curIndex = current.index;
        //printf("ival: %d Cur Q: %d\n", i, curQueue);
        found = 1;
        //printf("found %d %d\n", curQueue, curIndex);
      }
    }
    //printf("timeQuant: %d %d\n", timeQuanTrack, queues[curQueue].timeQuantum);
    
    if(found){
      //printf("q %d p%d currburst %d boostTrack %d TimeQuantTrack %d\n", curQueue ,queues[curQueue].queue[curIndex].processId ,queues[curQueue].queue[curIndex].currBurst, boostTrack, timeQuanTrack);
      // If process got preempted
      if(curQueue!=old_queue && timeQuanTrack!=0 && queues[old_queue].queue[old_index].ioBurst==0){
        // Move old element to end of queue
        endIndex = queues[old_queue].endProcess;
        queues[old_queue].currProcess = (queues[old_queue].currProcess+1)%y;
        queues[old_queue].queue[(endIndex+1) % y] = queues[old_queue].queue[old_index];
        queues[old_queue].endProcess = (endIndex+1)%y;
        queues[old_queue].queue[(endIndex+1) % y].index = (endIndex+1) % y;

        tempIndex = queues[old_queue].queue[old_index].originalIndex;
        history[tempIndex][historyNum[tempIndex]][0] = old_queue;
        history[tempIndex][historyNum[tempIndex]][1] = start;
        history[tempIndex][historyNum[tempIndex]][2] = time;
        historyNum[tempIndex]++;
        timeQuanTrack = 0;
        start = time;
        processes[queues[curQueue].queue[curIndex].originalIndex].status = 0;
      }

      //printf("timeQuant: %d %d\n", timeQuanTrack, queues[curQueue].timeQuantum);
      // printf("boost: %d\n", boostTrack);
      // printf("curold index: %d %d\n", curIndex, old_index);
      boostTrack +=1;
      // If priority boost is ready and old process stopped running
      if(boostTrack == s){
        boostReady = 1;
        boostTrack = 0;
      }
      if(boostReady && old_index!=curIndex){
        // for(i = 0; i < y; i++) {
        //   printf("processio %d %d %d %d\n", processes[i].processId, processes[i].timeAvail, processes[i].ioBurst, processes[i].IOrun);
        //   if(processes[i].timeAvail==time+1 && processes[i].ioBurst!=0&&(processes[i].IOrun!=0)){
        //     int index = processes[i].queue;
        //     printf("insert io %d %d\n", index, processes[i].processId);
        //     endIndex = queues[index].endProcess;
        //     queues[index].queue[(endIndex+1) % y] = processes[i];
        //     queues[index].endProcess = (endIndex+1)%y;
        //     queues[index].queue[(endIndex+1) % y].index = (endIndex+1) % y;
        //     queues[index].n = queues[index].n+1;
        //     processes[i].IOrun=0;
        //     IOTrack = 0;
        //   }
        //   //printf("Pro %d ",queues[0].queue[i].originalIndex)
        //   printf("\n\n");
        // }
        //printf("boost\n");
        boostReady = 0;
        //Reset all processes to queue 1;
        for(i=1;i<x;i++){ //For each queue, except first queue
          for(j=0;j<queues[i].n;j++){//For each process in each queue
            int moveIndex = (queues[0].endProcess+1) % y;
            queues[0].queue[moveIndex] = queues[i].queue[(queues[i].currProcess+j)%y];
            queues[0].queue[moveIndex].queue = 0;
            processes[queues[i].queue[(queues[i].currProcess+j)%y].originalIndex].queue = 0;
            queues[0].queue[moveIndex].index = moveIndex;
            queues[0].endProcess = moveIndex;
            queues[0].queue[moveIndex].IOrun = 0;
            queues[0].n = queues[0].n+1;
            //printf("val:%d endprocess:%d \n", queues[0].queue[moveIndex].processId,queues[0].endProcess);
          }
          queues[i].n = 0;
          queues[i].currProcess = 0;
          queues[i].endProcess = -1;
        }
        // printf("Printing Queue 1: \n");
        // printf("%d %d %d \n", queues[0].currProcess, queues[0].endProcess, queues[0].queueId);
        // for(i = 0; i < queues[0].n; i++) {
        //   printf("%d %d %d %d %d %d \n", queues[0].queue[i].processId, queues[0].queue[i].arrivalTime, queues[0].queue[i].executionTime, queues[0].queue[i].ioBurst, queues[0].queue[i].ioFrequency, queues[0].queue[i].currBurst);
        // }
        found = 0;
        boostTrack = -1;
        timeQuanTrack = 0;
        continue;
      }
      time++;
      timeQuanTrack += 1;
      
      queues[curQueue].queue[curIndex].currBurst -= 1;
      processes[queues[curQueue].queue[curIndex].originalIndex].currBurst -= 1;
      if(queues[curQueue].queue[curIndex].ioBurst!=0){
        IOTrack++;
        //printf("increment %d %d %d %d\n", curQueue, curIndex, queues[curQueue].queue[curIndex].processId, queues[curQueue].queue[curIndex].IOrun);
        queues[curQueue].queue[curIndex].IOrun++;
        processes[queues[curQueue].queue[curIndex].originalIndex].IOrun++;
        processes[queues[curQueue].queue[curIndex].originalIndex].status = 1;
        // printf("IO RUN %d\n", queues[curQueue].queue[curIndex].IOrun);
      }
        
      // If process already finished
      if(queues[curQueue].queue[curIndex].currBurst==0){
        // printf("finished %d", queues[curQueue].queue[curIndex].processId);
        endIter++;
        timeQuanTrack = 0;
        IOTrack = 0;
        queues[curQueue].currProcess = (queues[curQueue].currProcess + 1) % y;
        queues[curQueue].n--;
        int tempIndex = queues[curQueue].queue[curIndex].originalIndex;
        history[tempIndex][historyNum[tempIndex]][0] = curQueue;
        history[tempIndex][historyNum[tempIndex]][1] = start;
        history[tempIndex][historyNum[tempIndex]][2] = time;
        historyNum[tempIndex]++;
        start = time;
      }
      else if(((timeQuanTrack == queues[curQueue].timeQuantum && old_index==curIndex) || (queues[curQueue].queue[curIndex].IOrun==queues[curQueue].timeQuantum&&queues[curQueue].queue[curIndex].ioBurst!=0&&processes[queues[curQueue].queue[curIndex].originalIndex].IOrun!=queues[curQueue].queue[curIndex].ioBurst))){
        //printf("whut\n");
        if(queues[curQueue].queue[curIndex].ioBurst!=0&&processes[queues[curQueue].queue[curIndex].originalIndex].IOrun!=queues[curQueue].queue[curIndex].ioBurst){
          timeQuanTrack = 0;
          //printf("\nhellokitty\n");
          //printf("io run %d %d\n",queues[curQueue].queue[curIndex].processId, queues[curQueue].queue[curIndex].IOrun);
          queues[curQueue].queue[curIndex].IOrun = 0;
          //printf("io run %d %d\n",queues[curQueue].queue[curIndex].processId, queues[curQueue].queue[curIndex].IOrun);
          // processes[queues[curQueue].queue[curIndex].originalIndex].IOrun = 0;
          //printf("IO");
          int tempIndex = queues[curQueue].queue[curIndex].originalIndex;
          history[tempIndex][historyNum[tempIndex]][0] = curQueue;
          history[tempIndex][historyNum[tempIndex]][1] = start;
          history[tempIndex][historyNum[tempIndex]][2] = time;
          historyNum[tempIndex]++;
          start = time;

          int move = curQueue+1;
          if(move==x){
            move = curQueue;
            //printf("im here");
          }
          endIndex = queues[move].endProcess;
          processes[queues[curQueue].queue[curIndex].originalIndex].queue = move;
          processes[queues[curQueue].queue[curIndex].originalIndex].index = (endIndex+1)%y;
          //Move process to end of queue
          queues[curQueue].currProcess = (queues[curQueue].currProcess+1)%y;
          queues[curQueue].n--;
          queues[move].queue[(endIndex+1) % y] = queues[curQueue].queue[curIndex];
          queues[move].n++;
          queues[move].endProcess = (endIndex+1)%y;
          queues[move].queue[(endIndex+1) % y].queue = move;
          queues[move].queue[(endIndex+1) % y].index = (endIndex+1)%y;
        }
        else{
            //Move current element to the last element of the next queue;
          //printf("down %d \n", queues[curQueue].queue[curIndex].processId);
          queues[curQueue].queue[curIndex].IOrun = 0;
          //printf("io run %d %d\n",queues[curQueue].queue[curIndex].processId, queues[curQueue].queue[curIndex].IOrun);
          if(IOTrack==queues[curQueue].queue[curIndex].ioBurst){
            processes[queues[curQueue].queue[curIndex].originalIndex].IOrun = 0;
          }
          IOTrack = 0;
          timeQuanTrack = 0;
          int moveQueue = curQueue+1;
          // If process is already at the last queue
          if(moveQueue==x){
            // printf("endQueue\n");
            moveQueue = curQueue;
            endIndex = queues[moveQueue].endProcess;
            // printf("endindex: %d\n", endIndex);
            queues[curQueue].currProcess = (queues[curQueue].currProcess+1)%y;
            queues[curQueue].queue[(endIndex+1) % y] = queues[curQueue].queue[curIndex];
            queues[curQueue].queue[(endIndex+1) % y].index=(endIndex+1) % y;
            queues[curQueue].endProcess = (endIndex+1)%y;
            processes[queues[curQueue].queue[curIndex].originalIndex].index = (endIndex+1)%y;
          }
          else{
            //printf("%d cur process\n", queues[moveQueue].endProcess);
            endIndex = queues[moveQueue].endProcess;
            
            queues[moveQueue].queue[(endIndex+1) % y] = queues[curQueue].queue[curIndex];
            queues[moveQueue].n++;
            queues[moveQueue].endProcess = (endIndex+1)%y;
            queues[moveQueue].queue[(endIndex+1) % y].queue = moveQueue;
            processes[queues[moveQueue].queue[(endIndex+1) % y].originalIndex].queue = moveQueue;
            // printf("Moved P%d to Queue %d",processes[1].queue,processes[queues[moveQueue].queue[(endIndex+1) % y].originalIndex].queue);
            queues[moveQueue].queue[(endIndex+1) % y].index = (endIndex+1)%y;
            processes[queues[curQueue].queue[curIndex].originalIndex].index = (endIndex+1)%y;
            queues[curQueue].n--;
            if(queues[curQueue].n==0){
              queues[curQueue].currProcess = 0;
              queues[curQueue].endProcess = -1;
            }
            else
              queues[curQueue].currProcess = ((queues[curQueue].currProcess) + 1) % y;
            // printf("endindex: %d\n", endIndex);
          }
          int tempIndex = queues[curQueue].queue[curIndex].originalIndex;
          history[tempIndex][historyNum[tempIndex]][0] = curQueue;
          history[tempIndex][historyNum[tempIndex]][1] = start;
          history[tempIndex][historyNum[tempIndex]][2] = time;
          historyNum[tempIndex]++;
          start = time;
          // printf("ioburst %d\n", queues[curQueue].queue[curIndex].ioBurst);
          if(queues[curQueue].queue[curIndex].ioBurst!=0){
            history[tempIndex][historyNum[tempIndex]][0] = -1;
            history[tempIndex][historyNum[tempIndex]][1] = start;
            history[tempIndex][historyNum[tempIndex]][2] = start+queues[curQueue].queue[curIndex].ioFrequency;
            historyNum[tempIndex]++;
            afterIO = queues[moveQueue].queue[(endIndex+1) % y].ioFrequency;
            queues[moveQueue].queue[(endIndex+1) % y].timeAvail = time+afterIO;
            // printf("time avail: %d\n", queues[moveQueue].queue[(endIndex+1) % y].timeAvail);
            processes[queues[moveQueue].queue[(endIndex+1) % y].originalIndex].timeAvail = time+afterIO;
            // printf("time avail2: %d\n", processes[queues[moveQueue].queue[(endIndex+1) % y].originalIndex].timeAvail);
          }
        }
      }
      else if(queues[curQueue].queue[curIndex].ioBurst!=0){ //If IO Burst
        // printf("IOBURST CHECK%d\n", IOTrack==queues[curQueue].queue[curIndex].ioBurst);
        if(processes[queues[curQueue].queue[curIndex].originalIndex].IOrun==queues[curQueue].queue[curIndex].ioBurst){
          //printf("hello im here %d %d %d\n\n", queues[curQueue].queue[curIndex].processId, queues[curQueue].queue[curIndex].IOrun, queues[curQueue].timeQuantum);
          if(queues[curQueue].queue[curIndex].IOrun>=queues[curQueue].timeQuantum){
            queues[curQueue].queue[curIndex].IOrun = 0;
            //processes[queues[curQueue].queue[curIndex].originalIndex].IOrun = 0;
            //printf("hello im here %d %d %d\n\n", queues[curQueue].queue[curIndex].processId, queues[curQueue].queue[curIndex].IOrun, queues[curQueue].timeQuantum);
            int move3 = curQueue+1;
            if(move3==x)
              move3 = curQueue;
            processes[queues[curQueue].queue[curIndex].originalIndex].queue = move3;
            queues[curQueue].queue[curIndex].queue=move3;
            processes[queues[curQueue].queue[curIndex].originalIndex].oldQueue = 0;
          }
          else{
            processes[queues[curQueue].queue[curIndex].originalIndex].oldQueue = queues[curQueue].queue[curIndex].IOrun;
            //printf("not down %d %d %d \n", time,queues[curQueue].queue[curIndex].processId ,processes[queues[curQueue].queue[curIndex].originalIndex].oldQueue);
          }
          //queues[curQueue].queue[curIndex].IOrun = 0;
          //processes[queues[curQueue].queue[curIndex].originalIndex].IOrun = 0;
          // printf("IO");
          int tempIndex = queues[curQueue].queue[curIndex].originalIndex;
          history[tempIndex][historyNum[tempIndex]][0] = curQueue;
          history[tempIndex][historyNum[tempIndex]][1] = start;
          history[tempIndex][historyNum[tempIndex]][2] = time;
          historyNum[tempIndex]++;
          start = time;
          history[tempIndex][historyNum[tempIndex]][0] = -1;
          history[tempIndex][historyNum[tempIndex]][1] = start;
          history[tempIndex][historyNum[tempIndex]][2] = start+queues[curQueue].queue[curIndex].ioFrequency;
          historyNum[tempIndex]++;
          IOTrack=0;
          timeQuanTrack = 0;
          //Set Time Avail to time after IO
          afterIO = queues[curQueue].queue[curIndex].ioFrequency;
          endIndex = queues[curQueue].endProcess;
          queues[curQueue].queue[curIndex].timeAvail = time+afterIO;
          // printf("time avail: %d\n", queues[curQueue].queue[curIndex].timeAvail);
          processes[queues[curQueue].queue[curIndex].originalIndex].timeAvail = time+afterIO;
          //processes[queues[curQueue].queue[curIndex].originalIndex].queue = curQueue;
          //Move process to end of queue
          queues[curQueue].currProcess = (queues[curQueue].currProcess+1)%y;
          // queues[curQueue].queue[(endIndex+1) % y] = queues[curQueue].queue[curIndex];
          // queues[curQueue].queue[(endIndex+1) % y].index = (endIndex+1) % y;
          // queues[curQueue].endProcess = (queues[curQueue].endProcess+1) %y;
          // printf("endindex: %d", endIndex);
          queues[curQueue].n--;
        }
        found = 0;
      }
      found = 0;
        
      // for(i = 0; i < x; i++) {
      //   printf("Queue %d N=%d Time Quant=%d\n",queues[i].queueId, queues[i].n, queues[i].timeQuantum);
      //   for(j = 0; j < y; j++) {
      //     int index = (queues[i].currProcess+j)%y;
      //     if(queues[i].queue[j].processId!=-1)
      //       printf("P%d %d | ", queues[i].queue[j].processId, queues[i].queue[j].currBurst);
      //     else
      //       printf("[IO] %d | ", queues[i].queue[j].currBurst);
      //   }
      //   printf("\n\n");
      // }
    }
    else{
      time++;
      boostTrack++;
      start = time;
    }

  }
  // Print answers
      printf("********************************\n");
      float ave = 0;
      for(i=0;i<y;i++){
        printf("P[%d]\n", processes[i].processId);
        int waiting = 0;
        for(j=0;j<historyNum[i];j++){
          if(history[i][j][0]!=-1)
            printf("Q[%d] Start: %d End: %d\n", queues[history[i][j][0]].queueId ,history[i][j][1],history[i][j][2]);
          else
            printf("[IO] Start: %d End: %d\n",history[i][j][1],history[i][j][2]);
          if(history[i][j][0]!=-1){
            if(j==0)
              waiting+=history[i][j][1]-processes[i].arrivalTime;
            else
              waiting+=history[i][j][1]-history[i][j-1][2];
          }
        }
        printf("Waiting time: %d\n", waiting);
        ave+=waiting;
        printf("Turnaround time: %d\n", history[i][historyNum[i]-1][2]-processes[i].arrivalTime);
        printf("********************************\n");
      }
      printf("Average waiting time: %f", (ave/y));
  }
 
  
  
  return 0;
}