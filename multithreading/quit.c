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
    printf("Canceled\n"); // call to write -> cancellation point
    printf("I'm coming !\n"); // We won't see that
    return ((void *)1);
}

void *exit_routine_count_to_number(void *data) {
    size_t number = (size_t)data;
    size_t middle = number / 2;

    printf("I'm starting to count\n");
    while (number-- > 0) {
        if (number == middle) {
            printf("I'm a cheater I'm coming after counting half\n");
            pthread_exit((void *) 2);
        }
    };
    printf("I'm coming !\n");
    return ((void *)1);
}


int main(void)
{
    int err = 0;
    pthread_t exit_thread = 0;
    pthread_t cancel_thread = 0;
    size_t number = 1000000000;
    size_t middle = 500000000;
    void *ret = NULL;

    printf("Hide and seek start\n");

    err = pthread_create(&exit_thread, NULL, exit_routine_count_to_number, (void *) number);
    if (check_err(err, 0))
        return (-1);
    err = pthread_create(&cancel_thread, NULL, routine_count_to_number, (void *) number);
    if (check_err(err, 0))
        return (-1);
    while (middle-- > 0);
    // The cancel will be applied at the next cancellation point after thread received it
    // Cancellation point are most of system functions (see pthreads 7)
    err = pthread_cancel(cancel_thread);
    if (check_err(err,0))
        return (-1);
    err = pthread_join(exit_thread, &ret);
    if (check_err(err,0))
        return (-1);
    printf("exit_thread return: %d\n", (int)ret);
    err = pthread_join(cancel_thread, NULL);
    if (check_err(err,0))
        return (-1);
    return (0);
}