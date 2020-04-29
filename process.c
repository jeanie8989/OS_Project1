#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sched.h>
#include "process.h"
#define GET_TIME 334
#define PRINTK 335

void cpu_assign(pid_t pid, int cpu) {

    if (cpu > sizeof(cpu_set_t)) {
        fprintf(stderr, "cpu error\n");
        exit(-1);
    }

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    
    if (sched_setaffinity(pid, sizeof(mask), &mask) < 0) {
        fprintf(stderr, "sched_setaffinity failed\n");
        exit(-1);
    }
}

int proc_exec(struct Process proc) {
    
    int pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(-1);
    }
    else if (pid == 0) {    // child
        unsigned long start_sec, start_nsec, end_sec, end_nsec;
        syscall(GET_TIME, &start_sec, &start_nsec);

        for (int i = 0; i < proc.t_exec; i++)
            t_unit();
        
        syscall(GET_TIME, &end_sec, &end_nsec);
        syscall(PRINTK, getpid(), start_sec, start_nsec, end_sec, end_nsec);
        exit(0);
    }

    cpu_assign(pid, PROC_CPU);
    
    return pid;
}

int proc_run(pid_t pid) {
    struct sched_param param;
    param.sched_priority = 0;

    int ret = sched_setscheduler(pid, SCHED_OTHER, &param);
    if (ret < 0) {
        fprintf(stderr, "sched_setscheduler failed\n");
        exit(-1);
    }
    return ret;
}

int proc_stop(pid_t pid) {
    struct sched_param param;
    param.sched_priority = 0;

    int ret = sched_setscheduler(pid, SCHED_IDLE, &param);
    if (ret < 0) {
        fprintf(stderr, "sched_setscheduler failed\n");
        exit(-1);
    }
    return ret;
}

