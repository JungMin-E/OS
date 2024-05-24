#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

// 메세지를 보내는 부분 함수
int send_signal (int pid)
{
  int ret;
  ret = kill(pid, SIGUSR1);
  printf("ret : %d",ret);
}

// pid로 SIGQUIT 신호를 발생하는 함수
void sig_stp(int singo)
{
  int pid = 0;
  printf("신호를 보냅니다\npid: ");
  scanf("%d", &pid);
  send_signal(pid);
}

int main()
{
  // 시그널 등록
  signal(SIGTSTP, (void*)sig_stp);
  
  // 무한루프하며 대기
  while(1)
  {
    sleep(1);
  }
} 
