#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>f
#include <sys/shm.h> 

#define BUFSZ 255
const char ON = 'n', OFF = 'f', KILL = 'k', N = '\0';

void make_writer(void* shmem_w, void* shmem_r)
{
    int f = fork();

    if (f == 0)
    {
        char buf[BUFSZ] = "";
        while (1)
        {
            int size = 0;
            if ((size = read(0, buf, BUFSZ - 1)) > 0)
            {
                if (size < BUFSZ - 1)
                {
                    buf[size - 1] = '\0';
                }

                memcpy(shmem_w, buf, BUFSZ);
                
                if (strcmp(buf, "exit") != 0)
                {
                    memcpy(shmem_w + BUFSZ, &ON, 1);
                }
                else
                {
                    memcpy(shmem_r + BUFSZ, &KILL, 1);
                    memcpy(shmem_w + BUFSZ, &KILL, 1);
                }
            }
        }
    }
}

void make_reader(void* shmem_r)
{
    int f = fork();

    if (f == 0)
    {
        // char buf[BUFSZ] = "";
        while (1)
        {
            if (memcmp(shmem_r + BUFSZ, &KILL, 1) == 0)
            {
                killpg(getpgrp(), SIGKILL);
            }
            // if (memcmp(shmem, buf, BUFSZ) != 0)
            if (memcmp(shmem_r + BUFSZ, &ON, 1) == 0)
            {
                printf("C: %s\n", (char*)shmem_r);
                // memcpy(buf, shmem, BUFSZ);
                memcpy(shmem_r + BUFSZ, &OFF, 1);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    key_t key[2]; 
    int shmflg = IPC_CREAT | 0666; 
    int shmid[2];  
    int size = BUFSZ + 1;
    const int CODE1 = 437823, CODE2 = 162523;

    if (argv[1][0] == 'a')
    {
        key[0] = CODE1;
        key[1] = CODE2;
    }
    else if (argv[1][0] == 'b')
    {
        key[0] = CODE2;
        key[1] = CODE1;
    }

    if ((shmid[0] = shmget(key[0], size, shmflg)) == -1 || (shmid[1] = shmget(key[1], size, shmflg)) == -1) 
    {
        perror("shmget: shmget failed"); 
        exit(1); 
    }
    
    void* shmem_r = shmat(shmid[0], NULL, 0);
    void* shmem_w = shmat(shmid[1], NULL, 0);

    memcpy(shmem_r, &N, size);
    memcpy(shmem_w, &N, size);

    printf("Start Check: %s\n", (char*)shmem_w);
    printf("Start Check: %s\n", (char*)shmem_r);
    
    make_reader(shmem_r);
    make_writer(shmem_w, shmem_r);

    wait(NULL);
}
