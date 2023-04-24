#include "queue.h"

#include <stdlib.h>
#include <stdio.h>

void make_queue(queue_t* q) {

    q->arr = (pair_t*) malloc(sizeof(pair_t));
    
    q->capacity = 1;
    
    q->front = 0;
    q->rear  = q->capacity - 1;
    q->size  = 0;

}

int enqueue(queue_t* q, pair_t p) {

    if(q->size == q->capacity) {
    
        q->arr = (pair_t*) realloc(q->arr, sizeof(pair_t) * q->capacity * 2);

        if(q->arr == NULL) {

            printf("Could not increase the capacity of the queue. Ran out of memory.\n");
            exit(1);

        }


        if(q->rear < q->front) {

            int ntransfer = q->rear;

            for(int i=0; i<ntransfer; i++) {

                q->arr[q->capacity + i] = q->arr[i];

            }

            q->rear = (q->capacity - 1) + ntransfer;
        
        }

        q->capacity = 2 * q->capacity;

    }

    q->rear = (q->rear + 1) % q->capacity;
    q->arr[q->rear] = p;
    q->size = q->size + 1;

    return 1;

}

int dequeue(queue_t* q, pair_t* p) {

    if(q->size == 0) {

        p->first = -1;
        p->second = -1;

        return 0;

    }

    *p = q->arr[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size = q->size - 1;

    return 1;

}

int   empty(queue_t* q) {

    return q->size == 0;

}

void free_queue(queue_t* q) {

    free(q->arr);
    q->arr = NULL;
    q->capacity = 0;
    q->front = -1;
    q->rear = -1;
    q->size = -1;

}


