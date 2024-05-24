#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int i = 0;
int ud = 1;

void sig_int(int signo) {
    char c[256];
    printf("종료할까요? [y/n]\n");
    scanf("%255s", c); // Limiting scanf to prevent buffer overflow
    if (strcmp(c, "y") == 0 || strcmp(c, "Y") == 0) { 
        exit(1); 
    } else if (strcmp(c, "n") == 0 || strcmp(c, "N") == 0) { 
        signal(SIGINT, sig_int); // Reset signal handler to handle SIGINT again
    } else { 
        signal(SIGINT, sig_int); // Reset signal handler to handle SIGINT again
    }
}

void sig_stp(int signo) {
    printf("숫자를 초기화 합니다\n");
    i = 0;
    signal(SIGTSTP, sig_stp); // Reset signal handler to handle SIGTSTP again
}

void sig_usr(int signo) {
    printf("증감 변환 요청이 들어왔습니다. 잠시 뒤 변환됩니다.\n");
    ud *= -1;
    signal(SIGUSR1, sig_usr); // Reset signal handler to handle SIGUSR1 again
}

int main() {
    printf("pid: %d\n", getpid());

    signal(SIGINT, sig_int);
    signal(SIGTSTP, sig_stp);
    signal(SIGUSR1, sig_usr);

    while(1) {
        printf("%d\n", i);
        sleep(3);
        i += ud;
    }
    return 0;
}

