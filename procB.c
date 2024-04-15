#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<signal.h>

struct msgbuf
{
	  long mtype;
    char data[30];
};

int id; 

void clean_up()
{
    msgctl(id, IPC_RMID, NULL);
    exit(0); 
}

int main()
{
    char s[30];
    struct msgbuf v1;
    int pid;

    id=msgget(4,IPC_CREAT|0644);
    if(id<0)
    {
        perror("msgget");
        return 0;
    }

    pid = fork(); 
    if(pid < 0)
    {
        perror("fork");
        return 0;
    }
    else if(pid > 0) 
    {
        signal(SIGINT, clean_up); 
        while(1)
        {
            msgrcv(id,&v1,sizeof(v1),2,0);
            printf("\rProcess A: %s\n",v1.data);
            printf("Process B: ");
            fflush(stdout);
            if(strcmp(v1.data, "exit") == 0) 
            {
                kill(pid, SIGINT);
                break;
            }
        }
    }
    else
    {
        signal(SIGINT, clean_up);
        while(1)
        {
            v1.mtype=3;
            printf("\rProcess B: ");
            scanf(" %[^\n]s",s);
            strcpy(v1.data,s);
            msgsnd(id,&v1,strlen(v1.data)+1,0); 
            if(strcmp(s, "exit") == 0) 
            {
                kill(getppid(), SIGINT);
                break;
            }
        }
    }

    clean_up();
    return 0;
}

