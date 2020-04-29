#include "process.h"

#define FIFO 1
#define RR 2
#define SJF 3
#define PSJF 4

void scheduler(int policy, struct Process *proc, int num_proc);
