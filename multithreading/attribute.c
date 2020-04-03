//
// Created by raykuzu on 24/03/2020.
//

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

int check_err(int ret, int waited_ret) {
    if (ret != waited_ret) {
        fprintf(stderr, "%s\n", strerror(errno));
        return (-1);
    }
    return (0);
}

void *routine_count_to_number(void *data) {
    size_t number = (size_t)data;

    printf("I'm starting to count\n");
    while (number-- > 0);
    printf("I'm coming !\n");
}


int main(void)
{
    int err = 0;
    pthread_t thread = 0;
    union pthread_attr_t attr;
    size_t number = 1000000000;

    printf("Hide and seek start\n");

    // Init attributes
    pthread_attr_init(&attr);

    err = pthread_create(&thread, &attr, routine_count_to_number, (void *)number);
    if (check_err(err,0))
        return (-1);

    err = pthread_join(thread, NULL);
    if (check_err(err,0))
        return (-1);
    // Destroy attributes
    pthread_attr_destroy(&attr);
    return (0);
}

/* All attributes have similar syntax and have 2 functions :

pthread_attr_set[attribute](args...)
pthread_attr_get[attribute](args...)

// setter example: pthread_attr_setstack(&attr, someaddr, somesize);
// getter exemple: pthread_attr_getstack(&attr, &someaddr, &somesize);

List of attributes

affinity_sp: set or exclude cpu's (on multiprocessor system) for the thread (default: system choice)
guardsize: set guardsize (zone who protect stack against overflow) for a thread (default: pagesize)
detachstate: set the detachstate (datached or joinable) set to detached is equal to pthread_create + pthread_detach (default: joinable)
inheritsched: set cpu scheduler (inherit or explicit) if explicit is specified, attributes schedpolicy, schedparam and scope will be considerate
              else these attributes will be equal to the parent thread (default: inherit)
schedpolicy: set cpu scheduler policy (fifo, rr or other) -> man 7 sched (default: other)
schedparam: set sched priority -> man 7 sched (default: system choice)
scope: set scope for the new thread (system or process) -> only system is supported on linux
stack: set the stack size and addr (default size: ulimit, default addr: system choice)
 */
