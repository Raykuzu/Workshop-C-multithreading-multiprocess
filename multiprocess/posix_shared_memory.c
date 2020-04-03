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
#include <sys/mman.h>

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

void *create_shared_memory(char *name, size_t size) {
    int fd = shm_open(name, O_RDWR | O_CREAT, 0666); // create shared memory
    ftruncate(fd, size); // set size

    return (mmap(0, size, PROT_WRITE, MAP_SHARED, fd, 0)); // get the pointer of the shared memory
}

void *open_shared_memory(char *name, size_t size) {
    int fd = shm_open(name, O_RDONLY, 0666); // open shared memory

    return (mmap(0, size, PROT_READ, MAP_SHARED, fd, 0));
}

void destroy_shared_memory(char *name) {
    shm_unlink(name);
}

size_t write_in_shared_memory_from_stdin(void *ptr) {
    char input[128] = {0};

    fgets(input, 128, stdin);
    printf("Process %d send a message: %s", getpid(), input);
    sprintf(ptr, "%s", input); // Write message in shared memory
    return (strlen(input));
}

size_t read_and_print_shared_memory(void *ptr) {
    char *msg = (char *)ptr;
    if (*msg != '\0') {
        printf("Process %d received a message: %s", getpid(), msg);
    }
    return (strlen(msg));
}

// Care, read (in fgets) is in blocking mode, if after send sigint it doesn't finish just write another
// message in stdin to unblock it and it will finish properly.

int main(void)
{
    char *shm_name = "posix_sm_example";
    void *shm_ptr = NULL;
    size_t shm_size = getpagesize();
    size_t len = 0;

    signal(SIGINT, &signhandler);
    pid_t pid = fork();

    printf("Hello I'm process with id %d\n", getpid());

    switch (pid) {
        case -1:
            fprintf(stderr, "%s\n", strerror(errno));
            break;
        case 0:
            printf("Hello I'm the child\n");
            flow_control.child_pid = getpid();
            sleep(1); // wait parent create shm before open it
            shm_ptr = open_shared_memory(shm_name, shm_size);
            while (flow_control.child_run == 1) {
                len = read_and_print_shared_memory(shm_ptr);
                shm_ptr += len; // Increment pointer with len of message
                sleep(1);
            };
            break;
        default:
            printf("I'm the parent\n");
            flow_control.parent_pid = getpid();
            flow_control.child_pid = pid;
            shm_ptr = create_shared_memory(shm_name, shm_size);
            while(flow_control.parent_run == 1) {
                len = write_in_shared_memory_from_stdin(shm_ptr);
                shm_ptr += len; // Increment pointer with len of message
                sleep(1);
            };
            destroy_shared_memory(shm_name);
            break;
    }
    return (0);
}