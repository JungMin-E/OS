#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "RWLock.h"

#define NUM_READERS 5
#define NUM_WRITERS 2

RWLock rwlock;
int accountMoney = 1000;

void* reader(void* arg) {
    long id = (long)arg;
    for (int i = 1; i <= 5; i++) {
        RWLock_EnterReadLock(&rwlock);
        printf("Reader %ld: accountMoney: %d\n", id, accountMoney);
        RWLock_LeaveReadLock(&rwlock);
        sleep(1);
    }
    return NULL;
}

void* writer(void* arg) {
    long id = (long)arg;
    for (int i = 1; i <= 5; i++) {
        RWLock_EnterWriteLock(&rwlock);
        accountMoney += 100;
        printf("Writer %ld: New balance is %d\n", id, accountMoney);
        RWLock_LeaveWriteLock(&rwlock);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    RWLock_Init(&rwlock);

    for (long i = 0; i < NUM_READERS; i++) { // Start from index 0
        pthread_create(&readers[i], NULL, reader, (void*)(i + 1));
    }
    for (long i = 0; i < NUM_WRITERS; i++) { // Start from index 0
        pthread_create(&writers[i], NULL, writer, (void*)(i + 1));
    }

    for (int i = 0; i < NUM_READERS; i++) { // Start from index 0
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) { // Start from index 0
        pthread_join(writers[i], NULL);
    }

    RWLock_Destroy(&rwlock);
    return 0;
}

