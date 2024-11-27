#include <stdio.h>
#include <stdlib.h>

#include "round_robin.h"


int main(){
    int nProcess =0;
    char filename [256]; /* enough filename  */
    printf("Enter filename:  ");
    scanf("%s",filename);
    FILE *fptr=fopen(filename,"r"); /* open for reading */

    /* open a new output file to overwrite it */
    FILE *newfptr = fopen("op_file.txt","w");

    /* Decide which Scheduling algoritm */
    printf("Enter Scheduling Algorithm ( 0 for FCFS , 1 for ROUND ROBIN ) ");
    int sch_alg;
    scanf("%d",&sch_alg);

    if(sch_alg==1){ /* round robin */

        PROCESS * processes = (PROCESS*)malloc(sizeof(PROCESS)); /* Dynamic array of processes */

        /*Reading inputs (working)*/
        int i,j;
        char temp_string[3];/*String holding 4 chars "p0\0" ---> "p9\0" */
        READ_FROM_FILE();

        int *state=(int*)calloc(1,nProcess*sizeof(int)); /* initialized as zeros */
        /*  //if state[i]=0 : new or didn't arrive process
            //if state[i]=1 : In ready Q
            //if state[i]=2 : running process(CPU TIME)
            //if state[i]=3 : blocked process (IO TIME)
            //if state[i]=4 : finished process */
        printf("# processes=%d\n",nProcess);

        int *rem=(int*)calloc(1,nProcess*sizeof(int)); /*remaining running time till finish for each process*/
        for(i=0;i<nProcess;i++)
            rem[i]=2*processes[i].arr[1]; /*remaining running time = 2*CPU_time initially */
        /* //Debugging Prints
        for(i=0;i<nProcess;i++)
            printf("%d ",rem[i]);
        printf("\n"); */

        int *blocked_time = (int*)calloc(1,nProcess*sizeof(int));   /*blocking time for each process, initially zeros */
        int *t_finish = (int*)calloc(1,nProcess*sizeof(int));       /*Finish time for each process, initially zeros */

        unsigned int t_q ,\
        t=0,\
        t_temp=0,\
        t_run=0,\
        nFinished=0,\
        CPU_taken=0;

        printf("Enter time quantum: ");
        scanf("%ud",&t_q);
        APPEND_TO_NEW_FILE();
        NODE * ready_q_head = create_node();

        for(t=0; nFinished<nProcess ;t++){ /*Run till all processes are finished*/
            CPU_taken = 0;
            for(i=0;i<nProcess;i++){
                /*1- checking process arrival*/
                if(processes[i].arr[3] == t && IS_NEW(i)){
                    /* process has cpu time !=0 ( not IO bound )*/
                    /* make it ready */
                    if(processes[i].arr[1]!=0){
                        enqueue(ready_q_head,i);
                        state[i] = 1;
                    }
                    else{ /* process is IO bound , block it immediately */
                        state[i]=3;
                        blocked_time[i]++;
                    }
                }
                /*2- checking process blocking */
                else if(IS_BLOCKED(i) && rem[i] == processes[i].arr[1]){
                    /* if finished IO request */
                    if(blocked_time[i]==processes[i].arr[2]){
                        /*  // if not IO bound
                            // move it to ready queue */
                        if(processes[i].arr[1]!=0){ /*CPU_time[i] > 0*/
                            enqueue(ready_q_head,i);
                            state[i]=1;
                        }
                        /*  // if IO bound
                            // finish it */
                        else{ /*CPU_time[i] = 0*/
                            state[i]=4;
                            nFinished++;
                            t_finish[i] = t - 1; /* record that its finish cycle is the last cycle*/
                        }
                    }
                    else {/* still in IO request time */
                        blocked_time[i]++;
                    }
                }
                /*3- checking for a running process */
                else if( IS_RUNNING(i) ){
                    /*  //still running :
                        //Didn't finish its quantum time
                        //and didn't finish execution nor came to the IO request */
                    if( t_temp < t_q && (rem[i]!=0 && rem[i] != processes[i].arr[1])){
                        CPU_taken=1;
                        rem[i]--;
                        t_temp++;
                        t_run++;
                    }
                    else{
                        /*process finished*/
                        if(rem[i]==0){
                            state[i]=4;
                            nFinished++;
                            t_finish[i] = t-1 ; /* record that its finish cycle is the past cycle*/
                        }
                        /*process came to the IO request, Block it !*/
                        else if(rem[i] == processes[i].arr[1]){
                            state[i]=3;
                            blocked_time[i]++;
                        }
                        /*  // process finished its time quantum
                            // move it the ready q */
                        else if(t_temp == t_q){
                            enqueue(ready_q_head,processes[i].arr[0]);
                            state[i]=1;
                        }
                        else{}
                    }
                }
                /*if process is finished */
                else if(IS_FINISHED(i))
                    continue;
                else{}
            }

            //Coming to the ready Q:
            #if (DEBUG_PRINTF_ENABLED != 0)
            print_q(ready_q_head);
            #endif
            /*  // if CPU is not taken by any process
                //dequeue a process to it */
            if(!CPU_taken){
                int running = dequeue(&ready_q_head); // dequeue returns -1 on empty queue

                /*  // if ready queue is not empty
                    // move the front process to the CPU (Run it !)
                */
                if(running >= 0){ /* running != -1 (ready q was not empty) */
                    /*initialize its running state */
                    state[running] = 2;
                    t_temp = 0;
                    /*run it for this cycle */
                    rem[running]--;
                    t_temp++;
                    t_run++;
                }
            }
            /*Appending current process states to the o/p file */
            if(nFinished<nProcess){
                APPEND_CURRENT_STATES();
            }
        }
        /*Append statistics before closing output file*/
        APPEND_STATISTICS();
    }


    /*Close files*/
    fclose(fptr);
    fclose(newfptr);
    return 0;
}
