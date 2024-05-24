#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int i = 0;
int ud = 1;

// 인터럽트 시그널 발생(CTRL+C)시 수행되는 종료 질문 루틴
void sig_int(int signo)
{
  char c[256];
  printf("종료할까요? [y/n]\n");
  scanf("%s", c);
  if (strcmp(c, "y") == 0 || strcmp(c, "Y") == 0) { exit(1); }
  else if (strcmp(c, "n") == 0 || strcmp(c, "N") == 0) { return; }
  else { sig_int(signo); }
}

// 일시중지 시그널 발생(CTRL+Z)시 수행되는 숫자 초기화 함수
void sig_stp(int singo)
{
  printf("숫자를 초기화 합니다\n");
  i = 0;
}

// 외부에서 들어올 QUIT 시그널에 대해 숫자 더하기/빼기 상태를 바꾸는 함수
void sig_usr(int singo)
{
  printf("증감 변환 요청이 들어왔습니다. 잠시 뒤 변환됩니다.\n");
  ud *= -1;
}

int main(int argc, char **argv)
{
  // 외부를 위해 pid 출력
  printf("pid: %d\n", getpid());

  // signal 등록
  signal(SIGINT, (void *)sig_int);
  signal(SIGTSTP, (void*)sig_stp);
  signal(SIGUSR1, (void*)sig_usr);

  // 무한루프하며 숫자를 더하거나 뺌
  while(1)
  {
    printf("%d\n", i);
    sleep(3);
    i += ud;
  }
}
