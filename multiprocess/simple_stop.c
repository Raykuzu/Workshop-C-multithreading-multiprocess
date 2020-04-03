//
// Created by raykuzu on 25/03/2020.
//

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int main(void)
{
    pid_t pid = fork();

    printf("Hello I'm process with id %d\n", getpid());

    switch (pid) {
        case -1:
            fprintf(stderr, "%s\n", strerror(errno));
            break;
        case 0:
            printf("Hello I'm the child\n");
            // Some task
            while (42) {
                sleep(1);
            };
            break;
        default:
            printf("I'm the parent\n");
            sleep(3);
            kill(pid, SIGTERM); // kill the child after 3 seconds
            break;
    }
    return (0);
}