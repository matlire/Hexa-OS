#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#define QUEUE_MAX_SIZE 128

typedef struct
{
    int items[QUEUE_MAX_SIZE];
    int front;
    int rear;
} Queue;

void q_initialize (Queue* q);

bool q_isEmpty (Queue* q);
bool q_isFull  (Queue* q);

int  q_enqueue (Queue* q, int value);
int  q_dequeue (Queue* q);
int  q_peek    (Queue* q);
void q_clear   (Queue* q);

#endif