#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void sig_hand(int sig) {}

sigset_t save_mask;

void print_log(char* message) {
    printf("proc[%d]: %s\n", getpid(), message);
}

void child_process(pid_t parent_pid) {
    while(1) {
        sigsuspend(&save_mask);
        print_log("ping received");

        sleep(1);
        kill(parent_pid, SIGUSR1);
        print_log("pong sent");
    }
}

void parent_process(pid_t child_pid) {
    while (1) {
        sleep(1);
        kill(child_pid, SIGUSR1);
        print_log("ping sent");

        sigsuspend(&save_mask);
        print_log("pong received");
    }
}

int main() {
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = sig_hand;
    sigaction(SIGUSR1, &sa, NULL);

    pid_t child_pid = fork();
    if (child_pid == 0) {
        child_process(getppid());
    } else {
        parent_process(child_pid);
    }

    return 0;
}
