#include "queue.h"

void q_initialize (Queue* q)
{
    q->front = 0;
    q->rear = 0;
    for (int i = 0; i < QUEUE_MAX_SIZE; i++)
    {
        q->items[i] = 0;
    }
}

bool q_isEmpty (Queue* q)
{
    return (q->front == q->rear);
}

bool q_isFull (Queue* q)
{
    return ((q->rear + 1) % QUEUE_MAX_SIZE == q->front);
}

int q_enqueue (Queue* q, int value)
{
    if (q_isFull(q)) {
        return -1;
    }
    
    q->items[q->rear] = value;
    q->rear = (q->rear + 1) % QUEUE_MAX_SIZE;

    return value;
}

int q_dequeue (Queue* q)
{
    if (q_isEmpty(q)) {
        return -1;
    }

    int value = q->items[q->front];
    q->items[q->front] = 0;
    q->front = (q->front + 1) % QUEUE_MAX_SIZE;

    return value;
}

int q_peek (Queue* q)
{
    if (q_isEmpty(q)) {
        return -1;
    }

    return q->items[q->front];
}

void q_clear (Queue* q)
{
    while (!q_isEmpty(q)) {
        q->items[q->front] = 0;
        q->front = (q->front + 1) % QUEUE_MAX_SIZE;
    }

    q->front = 0;
    q->rear = 0;
}