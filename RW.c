// RWLockTest.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "RWLock.h"

#define NUM_READERS 10
#define NUM_WRITERS 2
#define NUM_ITERATIONS 1000000

RWLock rwlock;
int accountMoney = 1000;

void* reader(void* arg) {
    long id = (long)arg;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        RWLock_EnterReadLock(&rwlock);
        // Simulate reading operation
        RWLock_LeaveReadLock(&rwlock);
    }
    return NULL;
}

void* writer(void* arg) {
    long id = (long)arg;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        RWLock_EnterWriteLock(&rwlock);
        accountMoney += 100;
        RWLock_LeaveWriteLock(&rwlock);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    RWLock_Init(&rwlock);

    for (long i = 0; i < NUM_READERS; i++) {
        pthread_create(&readers[i], NULL, reader, (void*)(i + 1));
    }

    for (long i = 0; i < NUM_WRITERS; i++) {
        pthread_create(&writers[i], NULL, writer, (void*)(i + 1));
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}
