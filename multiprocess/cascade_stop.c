//
// Created by raykuzu on 25/03/2020.
//

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

struct flow_control {
    pid_t parent_pid;
    pid_t child_pid;
    int parent_run;
    int child_run;
};

struct flow_control flow_control = {
        .parent_pid = 0,
        .child_pid = 0,
        .parent_run = 1,
        .child_run = 1,
};

// Global variables aren't really sexy, you can use static encapsulation too

void signhandler(__attribute__((unused)) int sig) {
    printf("The process %d received a sigint\n", getpid());
    // here is the place to general cleanup
    if (getpid() == flow_control.parent_pid) {
        printf("parent cleanup\n");
        flow_control.parent_run = 0;
        // here is the place to parent specific cleanup
    } else if (getpid() == flow_control.child_pid) {
        printf("child cleanup\n");
        flow_control.child_run = 0;
        // here is the place to child specific cleanup
    } else {
        fprintf(stderr, "I'm process %d, and there is nothing to handle my cleanup\n", getpid());
    }
}

int main(void)
{
    signal(SIGINT, &signhandler);
    pid_t pid = fork();

    // Care process don't share memory, if you modify the flow control after the fork
    // in the parent for example, the value won't be modified in the child

    printf("Hello I'm process with id %d\n", getpid());

    switch (pid) {
        case -1:
            fprintf(stderr, "%s\n", strerror(errno));
            break;
        case 0:
            printf("Hello I'm the child\n");
            flow_control.child_pid = getpid();
            // Some task
            while (flow_control.child_run == 1) {
                sleep(1);
            };
            break;
        default:
            printf("I'm the parent\n");
            flow_control.parent_pid = getpid();
            flow_control.child_pid = pid;
            // Some task
            while(flow_control.parent_run == 1) {
                sleep(1);
            };
            break;
    }
    return (0);
}