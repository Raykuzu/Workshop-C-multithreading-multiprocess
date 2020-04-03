//
// Created by raykuzu on 25/03/2020.
//

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>

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

void signhandler(__attribute__((unused)) int sig) {
    printf("The process %d received a sigint\n", getpid());
    // here is the place to general cleanup
    if (getpid() == flow_control.parent_pid) {
        printf("parent cleanup\n");
        flow_control.parent_run = 0;
    } else if (getpid() == flow_control.child_pid) {
        printf("child cleanup\n");
        flow_control.child_run = 0;
    } else {
        fprintf(stderr, "I'm process %d, and there is nothing to handle my cleanup\n", getpid());
    }
}

void write_in_fifo_from_stdin(char *fifo) {
    int fd = open(fifo, O_WRONLY);
    char input[128] = {0};

    fgets(input, 128, stdin);
    printf("Process %d send a message: %s", getpid(), input);
    write(fd, input, strlen(input) + 1);
    close(fd);
}

void read_in_fifo_and_print(char *fifo) {
    int fd = open(fifo, O_RDONLY);
    char output[128] = {0};

    read(fd, output, 128);
    close(fd);
    printf("Process %d received a message: %s", getpid(), output);
}

// Care, read (in read_in_fifo and in fgets) is in blocking mode, if after send sigint it doesn't finish just write another
// message in stdin to unblock it and it will finish properly.

int main(void)
{
    char *fifo = "myfifo";

    signal(SIGINT, &signhandler);
    mkfifo(fifo, 0666); // create the pipe
    pid_t pid = fork();

    printf("Hello I'm process with id %d\n", getpid());

    switch (pid) {
        case -1:
            fprintf(stderr, "%s\n", strerror(errno));
            break;
        case 0:
            printf("Hello I'm the child\n");
            flow_control.child_pid = getpid();
            while (flow_control.child_run == 1) {
                read_in_fifo_and_print(fifo);
                write_in_fifo_from_stdin(fifo);
                sleep(1);
            };
            break;
        default:
            printf("I'm the parent\n");
            flow_control.parent_pid = getpid();
            flow_control.child_pid = pid;
            while(flow_control.parent_run == 1) {
                write_in_fifo_from_stdin(fifo);
                read_in_fifo_and_print(fifo);
                sleep(1);
            };
            unlink(fifo);
            break;
    }
    return (0);
}
