// syscall 334
#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage void sys_my_get_time(unsigned long *sec, unsigned long *nsec) {
    struct timespec t;
    getnstimeofday(&t);
    *sec = t.tv_sec;
    *nsec = t.tv_nsec;
}