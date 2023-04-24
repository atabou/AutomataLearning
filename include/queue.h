
#ifndef QUEUE_H

    #include "common.h"

    typedef struct queue_t {

        pair_t* arr;

        int capacity;
        int front;
        int rear;
        int size;

    } queue_t;

    void make_queue(queue_t* q);
    
    int enqueue(queue_t* q, pair_t p);
    int dequeue(queue_t* q, pair_t* p);
    int empty(queue_t* q);
    
    void free_queue(queue_t* q);



#endif
