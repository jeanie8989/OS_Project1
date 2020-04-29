// syscall 335
#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void sys_my_print_kernel(int pid, unsigned long start_sec, unsigned long start_nsec, unsigned long end_sec, unsigned long end_nsec) {
    printk(KERN_INFO "[Project1] %d %ld.%09ld %ld.%09ld", pid, start_sec, start_nsec, end_sec, end_nsec);
}