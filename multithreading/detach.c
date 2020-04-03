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

void *joinable_routine_count_to_number(void *data) {
    size_t number = (size_t)data;

    printf("Im the joinable thread, you need to join me before end of program but you can get my return value\n");
    printf("I'm starting to count\n");
    while (number-- > 0);
    printf("I'm coming !\n");
    return ((void *)1);
}

void *detached_routine_count_to_number(void *data) {
    size_t number = (size_t)data;

    printf("Im the detached thread, I'll cleanup myself but you can't get my return value\n");
    printf("I'm starting to count\n");
    while (number-- > 0);
    printf("I'm coming\n");
    printf("I cleanup myself\n");
    return ((void *)1);
}


int main(void)
{
    int err = 0;
    pthread_t joinable_thread = 0;
    pthread_t detached_thread = 0;
    size_t number = 1000000000;
    void *ret = NULL;

    printf("Hide and seek start\n");

    err = pthread_create(&joinable_thread, NULL, joinable_routine_count_to_number, (void *)number);
    if (check_err(err,0))
        return (-1);
    err = pthread_create(&detached_thread, NULL, detached_routine_count_to_number, (void *)number);
    if (check_err(err,0))
        return (-1);
    err = pthread_detach(detached_thread);
    if (check_err(err,0))
        return (-1);
    printf("You wont see the print of the detached thread after we detached it\n");
    err = pthread_join(joinable_thread, &ret);
    if (check_err(err,0))
        return (-1);
    printf("The joinable thread return: %d\n", (int)ret);
    return (0);
}