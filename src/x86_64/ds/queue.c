#include "queue.h"

void q_initialize (Queue_T* q)
{
    q->front = 0;
    q->rear = 0;
    for (int i = 0; i < QUEUE_MAX_SIZE; i++)
    {
        q->items[i] = 0;
    }
}

bool q_isEmpty (Queue_T* q)
{
    return (q->front == q->rear);
}

bool q_isFull (Queue_T* q)
{
    return ((q->rear + 1) % QUEUE_MAX_SIZE == q->front);
}

int q_enqueue (Queue_T* q, int value)
{
    if (q_isFull(q)) {
        return -1;
    }
    
    q->items[q->rear] = value;
    q->rear = (q->rear + 1) % QUEUE_MAX_SIZE;

    return value;
}

int q_dequeue (Queue_T* q)
{
    if (q_isEmpty(q)) {
        return -1;
    }

    int value = q->items[q->front];
    q->items[q->front] = 0;
    q->front = (q->front + 1) % QUEUE_MAX_SIZE;

    return value;
}

int q_peek (Queue_T* q)
{
    if (q_isEmpty(q)) {
        return -1;
    }

    return q->items[q->front];
}

void q_clear (Queue_T* q)
{
    while (!q_isEmpty(q)) {
        q->items[q->front] = 0;
        q->front = (q->front + 1) % QUEUE_MAX_SIZE;
    }

    q->front = 0;
    q->rear = 0;
}
