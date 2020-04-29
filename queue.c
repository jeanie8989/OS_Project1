#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int isfull(struct Queue *q) {
    return ((q->back + 1) % Q_CAP == q->front);
}

int isempty(struct Queue *q) {
    return (q->front == q->back);
}

void push(struct Queue *q, int x) {
    if (isfull(q)) {
        fprintf(stderr, "Queue is full\n");
        exit(-1);
    }
    q->back = (q->back + 1) % Q_CAP;
    q->array[q->back] = x;
}

int pop(struct Queue *q) {
    if (isempty(q))
        return -1;
    else {
        q->front = (q->front + 1) % Q_CAP;
        return q->array[q->front];
    }
}