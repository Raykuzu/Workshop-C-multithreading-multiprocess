//
// Created by raykuzu on 25/03/2020.
//

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

int check_err(int ret, int waited_ret) {
    if (ret != waited_ret) {
        fprintf(stderr, "%s\n", strerror(errno));
        return (-1);
    }
    return (0);
}

struct random_data_bis {
    char something_bis;
    pthread_mutex_t mutex;
};

struct random_data {
    int something;
    pthread_mutex_t mutex;
    struct random_data_bis random_data_bis;
};



void *routine(void *data) {

    struct random_data *random_data = (struct random_data *)data;

    pthread_mutex_lock(&random_data->mutex);
    pthread_mutex_lock(&random_data->random_data_bis.mutex);
    random_data->something += 1;
    printf("Start counting (number %d)\n", random_data->something);
    for (size_t i = 0; i < 10000000; i++);
    printf("I'm coming (number %d)\n", random_data->something);
    pthread_mutex_lock(&random_data->random_data_bis.mutex);
    pthread_mutex_unlock(&random_data->mutex);
    return (NULL);
}

void *routine2(void *data) {

    struct random_data *random_data = (struct random_data *)data;

    pthread_mutex_lock(&random_data->random_data_bis.mutex);
    pthread_mutex_lock(&random_data->mutex);
    random_data->something += 1;
    printf("Start counting (number %d)\n", random_data->something);
    for (size_t i = 0; i < 10000000; i++);
    printf("I'm coming (number %d)\n", random_data->something);
    pthread_mutex_unlock(&random_data->mutex);
    pthread_mutex_lock(&random_data->random_data_bis.mutex);
    return (NULL);
}

/* What happened ?
 * thread1 acquire the lock
 * thread2 arrives at lock -> it's locked he wait
 * thread1 arrives at unlock -> release lock
 * thread1 finish his task
 * thread2 acquire the lock
 * thread2 arrives at unlock -> release lock
 * thread2 finish
*/
int main(void) {
    int err = 0;
    pthread_t threads[2] = {0};
    struct random_data random_data = {
            .something = 0,
            .mutex = PTHREAD_MUTEX_INITIALIZER,
            .random_data_bis.something_bis = 0,
            .random_data_bis.mutex = PTHREAD_MUTEX_INITIALIZER,
    };

    printf("Hide and seek start\n");

    err = pthread_create(&threads[0], NULL, routine, &random_data);
    if (check_err(err, 0))
        return (-1);
    err = pthread_create(&threads[1], NULL, routine2, &random_data);
    if (check_err(err, 0))
        return (-1);
    for (size_t i = 0; i < 2; i++) {
        err = pthread_join(threads[i], NULL);
        if (check_err(err, 0))
            return (-1);
    }
    return (0);
}