#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// SIGINT(Ctrl+C) 핸들러 함수
void sigint_handler(int signum) {
    char response;
    printf("\n종료할까요? (y/n): ");
    scanf(" %c", &response);
    if (response == 'y' || response == 'Y') {
        printf("프로그램을 종료합니다.\n");
        exit(0);
    }
}

// SIGTSTP(Ctrl+Z) 핸들러 함수
void sigtstp_handler(int signum) {
    printf("\nCTRL + Z 를 눌렀습니다. 다른 시그널 핸들링을 하는 프로그램을 실행합니다.\n");
    // 여기에 다른 시그널 핸들링을 하는 프로그램을 실행하는 코드를 추가하면 됩니다.
}

int main() {
    // SIGINT 핸들러 등록
    signal(SIGINT, sigint_handler);

    // SIGTSTP 핸들러 등록
    signal(SIGTSTP, sigtstp_handler);

    // 메인 루프
    while (1) {
        // 프로그램이 실행 중일 때의 동작을 여기에 구현합니다.
    }

    return 0;
}
