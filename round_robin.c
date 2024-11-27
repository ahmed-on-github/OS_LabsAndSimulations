#include <stdio.h>
#include <stdlib.h>
#include "round_robin.h"


NODE * create_node(void){
    NODE *front=(NODE*)calloc(1,sizeof(NODE)); /*fornt->next = NULL initially*/
    front->id = -1; /*Initially no process stored in the new node*/
    return front;
}

/* enqueue: insertion at end */
void enqueue(NODE* front,int id){
     /* front place is only here in the queue & is empty
     (Place the process id only with no need to allocate new node) */
     if(front && front->next == NULL && front->id == -1){
        front->id = id;
        return;
     }
     /*Allocate new node*/
     NODE * newnode = create_node();
     if( newnode != NULL){
         newnode->id= id;
         NODE * temp = front;
         if(front){ /*Or if(temp)*/
            while(temp->next){
                temp=temp->next;
            }
            temp->next = newnode;
         }
     }
}

/* dequeue : deletion from front. This modifies the queue accessed by the "front" pointer */
int dequeue(NODE ** front){
    if( front && (*front) ){ /*If queue is already present/allocated*/
        if( (*front)->next!= NULL){ /*queue front is not the only node*/
            NODE * temp = *(front);
            int id = temp->id;
            (*front)=(*front)->next;
            free(temp);
            return id; /*should not be -1*/
        }
        else{ /* only head is in the queue & is to be dequeued */
            if((*front)->id==-1) //empty
                return -1;
            else{
                int id_=(*front)->id;
                (*front)->id=-1;
                return id_;
            }
        }
    }
    return -1; /*Unreachable code*/
}

void print_q(NODE * front){
#if (DEBUG_PRINTF_ENABLED != 0)
    while(front && front->id!=-1){
        printf("%d  ",front->id);
        front=front->next;
    }
    printf("\n");
#endif
}
