//
// Created by raykuzu on 23/03/2020.
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
    return (NULL);
}


int main(void)
{
    int err = 0;
    pthread_t thread = 0;
    size_t number = 1000000000;

    printf("Hide and seek start\n");

    // Create the thread
    // arg 1: addr of the thread (pthread_t aka unsigned int),
    // arg2: attribute of the thread (NULL == default),
    // arg3: routine(function the thread will execute aka (void *)(func *)(void *)),
    // arg4: argument of the routine
    err = pthread_create(&thread, NULL, routine_count_to_number, (void *)number);
    if (check_err(err,0))
        return (-1);

    // Wait the thread finish his task (if you don't do that -> SEGV)
    err = pthread_join(thread, NULL);
    if (check_err(err,0))
        return (-1);
    return (0);
}