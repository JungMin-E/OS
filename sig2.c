#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int send_signal(int pid) {
    int ret = kill(pid, SIGUSR1);
    if (ret == -1) {
        perror("kill");
        return -1; // Error occurred
    }
    return 0; // Success
}

void sig_stp(int signo) {
    int pid;
    printf("프로그램에 시그널을 보냅니다\npid: ");
    scanf("%d", &pid);
    send_signal(pid);
}

void sig_usr(int signo) {
    printf("터미널 1에서 시그널이 도착했습니다.\n");
}

int main() {
    signal(SIGTSTP, sig_stp);
    signal(SIGUSR1, sig_usr);
    
    while(1) {
        sleep(1);
    }
    return 0;
}

