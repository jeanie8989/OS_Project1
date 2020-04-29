#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sched.h>
#include "scheduler.h"
#include "queue.h"

static int now_time;
static int running_proc;
static int num_finish_proc;
static int last_switch_time;
static int num_create_proc;
struct Queue *queue;   // for RR
static int PSJF_flag;   // for PSJF, create process or not

int find_next(int policy, struct Process *proc, int num_proc) {

    if (running_proc != -1 && (policy == SJF || policy == FIFO))
        return running_proc;

    int ret = -1;

    if (policy == FIFO) {
        if (proc[num_finish_proc].pid != -1)    // next process is created
            ret = num_finish_proc;
    }

    else if ((policy == SJF && running_proc == -1) || (policy == PSJF && (PSJF_flag == 1 || running_proc == -1))) {     // need to check context switch
        for (int i = 0; i < num_proc; i++) {
            if (proc[i].t_exec != 0 && proc[i].pid != -1) {     // created and not finish
                if (ret == -1 || proc[i].t_exec < proc[ret].t_exec)
                    ret = i; 
            }
        }
    }

    else if (policy == PSJF && PSJF_flag == 0)
        ret = running_proc;

    else if (policy == RR) {
        if (running_proc == -1) 
            ret = pop(queue);
        else if ((now_time - last_switch_time) % 500 == 0) {     // context switch
            push(queue, running_proc);
            ret = pop(queue);
        }
        else
            ret = running_proc;
    }

    return ret;
}

void scheduler(int policy, struct Process *proc, int num_proc) {

    cpu_assign(getpid(), SCHEDULER_CPU);
    proc_run(getpid());

    now_time = 0;
    running_proc = -1;  // no one running
    num_finish_proc = 0;    // no one finish
    num_create_proc = 0;    // no one create

    // queue for RR
    queue = (struct Queue *)malloc(sizeof(struct Queue)); 
    queue->front = 0;
    queue->back = 0;

#ifdef DEBUG
    fprintf(stderr, "scheduler set enviroment sucess\n");
#endif

    while(1) {

        // process finish
        if (running_proc != -1 && proc[running_proc].t_exec == 0) {

#ifdef DEBUG
            fprintf(stderr, "process %s finish at time %d\n", proc[running_proc].name, now_time);
#endif
            waitpid(proc[running_proc].pid, NULL, 0);
            printf("%s %d\n", proc[running_proc].name, proc[running_proc].pid);
            running_proc = -1;
            num_finish_proc++;

            // all finish
            if (num_finish_proc == num_proc) 
                break;
        }
        
        // find ready process
        PSJF_flag = 0;
        for (int i = num_create_proc; i < num_proc && proc[i].t_start <= now_time; i++) {
            if (proc[i].t_start == now_time) {
#ifdef DEBUG
                fprintf(stderr, "process %s ready at time %d\n", proc[i].name, now_time);
#endif
                proc[i].pid = proc_exec(proc[i]);
                proc_stop(proc[i].pid);
                PSJF_flag = 1;
                num_create_proc++;
                
                if (policy == RR) {
                    push(queue, i);
                }
            }
        }

        // find next process
        int next_proc = find_next(policy, proc, num_proc);
        if (next_proc != running_proc) {    // context switch
#ifdef DEBUG
            if (next_proc != -1)
                fprintf(stderr, "context switch to %s at time %d\n", proc[next_proc].name, now_time);
#endif
            proc_run(proc[next_proc].pid);
            proc_stop(proc[running_proc].pid);
            running_proc = next_proc;
            last_switch_time = now_time;    // for RR
        }

        // do iteration
        t_unit();
        now_time++;
        if (running_proc != -1)
            proc[running_proc].t_exec--;
    }

}