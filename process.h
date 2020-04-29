#define SCHEDULER_CPU 0
#define PROC_CPU 1

struct Process{
    char name[30];
    int t_start;
    int t_exec;
    pid_t pid;
};

void cpu_assign(pid_t pid, int cpu);
int proc_exec(struct Process proc);
int proc_run(pid_t pid);
int proc_stop(pid_t pid);

#define t_unit() { volatile unsigned long i; for(i=0;i<1000000UL;i++); }

void cpu_assign(pid_t pid, int cpu);
int proc_exec(struct Process proc);
int proc_run(pid_t pid);
int proc_stop(pid_t pid);