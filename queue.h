
#define Q_CAP 50

struct Queue {
    int array[Q_CAP];
    int front;
    int back;
};

int isfull(struct Queue *q);
int isempty(struct Queue *q);
void push(struct Queue *q, int x);
int pop(struct Queue *q);