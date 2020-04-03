//
// Created by raykuzu on 25/03/2020.
//

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>

int main(void)
{
    pid_t pid = fork(); // create the child, now there is two executions sequence on the following lines

    // How to know who I am ?
    // Fork return the child pid in the parent and 0 in the child if the operation work

    switch (pid) {
        case -1:
            fprintf(stderr, "%s\n", strerror(errno));
            break;
        case 0:
            printf("Hello I'm the child\n");
            break;
        default:
            printf("I'm the parent\n");
            break;
    }
    printf("Hello I'm process with id %d\n", getpid());
    return (0);
}