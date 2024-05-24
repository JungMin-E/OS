#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "RWLock.h"

//은행 계좌 잔액을 확인하고 금액을 바꾸는 시나리오
#define NUM_READERS 5 //초기 Readers 5명으로 설정
#define NUM_WRITERS 2 //초기 Writers 2명으로 설정

RWLock rwlock;
int accountMoney = 1000; //초기 계좌 금액 1000으로 설정

void* reader(void* arg) {
    long id = (long)arg;
    for (int i = 1; i <= 5; i++) {
        RWLock_EnterReadLock(&rwlock); //Reader에게 ReadLock에 접근권한을 주고
        printf("Reader %ld: accountMoney: %d\n", id, accountMoney); //계좌금액을 출력하기위해서 
        RWLock_LeaveReadLock(&rwlock); //Reader에게 ReadLock에 접근권한을 해제
        sleep(1);
    }
    return NULL;
}

void* writer(void* arg) {
    long id = (long)arg;
    for (int i = 1; i <= 5; i++) {
        RWLock_EnterWriteLock(&rwlock); //Writer에게 WriteLock에 접근 권한을 부여
        accountMoney += 100; //계좌 금액을 100씩 증가
        printf("Writer %ld: New balance is %d\n", id, accountMoney);
        RWLock_LeaveWriteLock(&rwlock); //Writer에게 WriteLock권한을 해제
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS]; //Readers배열, Writers배열 선언
    RWLock_Init(&rwlock); //RWLock초기화 함수

    for (long i = 0; i < NUM_READERS; i++) { 
        //스레드를 생성하고 생성된 스레드는 reader함수 호출하여 실행
        pthread_create(&readers[i], NULL, reader, (void*)(i + 1));
    }

    for (long i = 0; i < NUM_WRITERS; i++) { 
        //스레드 생성하고 생성된 스레드 writer함수 호출하여 실행
        pthread_create(&writers[i], NULL, writer, (void*)(i + 1));
    }

    for (int i = 0; i < NUM_READERS; i++) { 
        //모든 스레드들이 종료될떄 까지 기다리기
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < NUM_WRITERS; i++) { 
        pthread_join(writers[i], NULL);
    }

    return 0;
}
