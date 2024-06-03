// MutexLock.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 10
#define NUM_WRITERS 2
#define NUM_ITERATIONS 1000000

pthread_mutex_t mutex;
int accountMoney = 1000;

void* reader(void* arg) {
    long id = (long)arg;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        // Simulate reading operation
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* writer(void* arg) {
    long id = (long)arg;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        pthread_mutex_lock(&mutex);
        accountMoney += 100;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    pthread_mutex_init(&mutex, NULL);

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

    pthread_mutex_destroy(&mutex);
    return 0;
}
