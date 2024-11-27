#ifndef ROUND_ROBIN_H_
#define ROUND_ROBIN_H_

#define DEBUG_PRINTF_ENABLED 0U

#define NODE struct node
#define PROCESS struct process

#define IS_NEW(i)       (state[(i)]==0)
#define IS_READY(i)     (state[(i)]==1)
#define IS_RUNNING(i)   (state[(i)]==2)
#define IS_BLOCKED(i)   (state[(i)]==3)
#define IS_FINISHED(i)  (state[(i)]==4)

#define t_CPU(i)        (processes[(i)].arr[1])
#define t_IO(i)         (processes[(i)].arr[2])
#define t_arrival(i)    (processes[(i)].arr[3])

#define READ_FROM_FILE()  \
do { \
    while(!feof(fptr)){ /* didn't reach end of file*/ \
            fscanf(fptr,"%s",temp_string);\
            processes[nProcess].arr[0]=(int)temp_string[1]-48;\
            for(i=1;i<4;i++)\
                fscanf(fptr,"%d",&processes[nProcess].arr[i]);\
            processes=(PROCESS*)realloc(processes,(nProcess+2)*sizeof(PROCESS));\
            nProcess++;\
    }\
}while(0);

#define APPEND_TO_NEW_FILE() \
do{\
    for(i=0;i<nProcess;i++){\
        fprintf(newfptr , "%c%d ",'p',processes[i].arr[0]);\
        for(j=1;j<4;j++){\
            fprintf(newfptr,"%d ",processes[i].arr[j]);\
        }\
        fprintf(newfptr,"%c",'\n');\
    }\
    fprintf(newfptr,"%s %d%c","\nRound Robin with time quantum=",t_q,'\n');\
}while(0);

#define APPEND_CURRENT_STATES()   \
do{\
    fprintf(newfptr,"\nt=%d: ",t);\
    for(i=0;i<nProcess;i++){\
        if(state[i]==1) /* if process is in ready q */\
            fprintf(newfptr,"%c%d %s ",'P',i,"READY");\
        else if(state[i]==2)/* if process is running*/\
            fprintf(newfptr,"%c%d %s ",'P',i,"RUNNING");\
        else if(state[i]==3)/* if process is blocked*/\
            fprintf(newfptr,"%c%d %s ",'P',i,"BLOCKED");\
    }\
}while(0);

#define APPEND_STATISTICS() \
do{\
    fprintf(newfptr, "%c%s %d %c", '\n', "Finish time= ", t-2, '\n');\
    fprintf(newfptr, "%s %.9f %c", "CPU Utilization = ", (float)t_run/(t-1), '\n');\
    for(i=0; i<nProcess; i++){\
        fprintf(newfptr, "%s%d %c %d %c", "Turnaround Time for p", i, '=', t_finish[i]-t_ARRIVAL[i], '\n');\
    }\
}while(0);


PROCESS{
    int arr[4]; /*  //arr[0]: process ID
                    //arr[1]: CPU time
                    //arr[2]: I/O time
                    //arr[3]: arrival time */
};
NODE{
    int id;
    NODE *next ;
};

NODE*   create_node(void);
void    enqueue(NODE* front,int id);
int     dequeue(NODE ** front);
void    print_q(NODE * front);

#endif /*ROUND_ROBIN_H_*/
