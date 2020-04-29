#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include "scheduler.h"
#include "queue.h"

int compare(const void *a, const void *b) {
    struct Process *p1, *p2;
    p1 = (struct Process *)a;
    p2 = (struct Process *)b;
    return p1->t_start - p2->t_start;
}

int main(){
    char buf[50];   // scheduling policy
    int num_proc;   // number of proc

    int policy;

    scanf("%s", buf);
    scanf("%d", &num_proc);

    struct Process *proc;
    proc = (struct Process *)malloc(num_proc * sizeof(struct Process));

    for (int i = 0; i < num_proc; i++){
        scanf("%s%d%d", proc[i].name, &proc[i].t_start, &proc[i].t_exec);
        proc[i].pid = -1;   // not ready
    }

    qsort(proc, num_proc, sizeof(struct Process), compare);

    if (strcmp(buf, "FIFO") == 0) {
        policy = FIFO;
    }
    else if (strcmp(buf, "RR") == 0) {
        policy = RR;
    }
    else if (strcmp(buf, "SJF") == 0) {
        policy = SJF;
    }
    else if (strcmp(buf, "PSJF") == 0) {
        policy = PSJF;
    }
    else {
        fprintf(stderr, "illegal policy\n");
        exit(-1);
    }
#ifdef DEBUG
    fprintf(stderr, "main: policy = %s, number of process = %d\n", buf, num_proc);
    fprintf(stderr, "mainpid = %d\n", getpid());
#endif
    scheduler(policy, proc, num_proc);

}