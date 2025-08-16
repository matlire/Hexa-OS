#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#define QUEUE_MAX_SIZE 128

typedef struct
{
    int items[QUEUE_MAX_SIZE];
    int front;
    int rear;
} Queue_T;

void q_initialize (Queue_T* q);

bool q_isEmpty (Queue_T* q);
bool q_isFull  (Queue_T* q);

int  q_enqueue (Queue_T* q, int value);
int  q_dequeue (Queue_T* q);
int  q_peek    (Queue_T* q);
void q_clear   (Queue_T* q);

#endif
