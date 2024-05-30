include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

pthread_mutex_t rw_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int read_count = 0;
int accountMoney = 1000;

void* reader(void* arg) {
    long id = (long)arg;
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&mutex);
        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&rw_mutex);
        }
        pthread_mutex_unlock(&mutex);

        printf("Mutex Reader %ld: accountMoney: %d\n", id, accountMoney);

        pthread_mutex_lock(&mutex);
        read_count--;
        if (read_count == 0) {
            pthread_mutex_unlock(&rw_mutex);
        }
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
    return NULL;
}

void* writer(void* arg) {
    long id = (long)arg;
    for (int i = 1; i <= 5; i++) {
        pthread_mutex_lock(&rw_mutex);
        accountMoney += 100;
        printf("Mutex Writer %ld: New balance is %d\n", id, accountMoney);
        pthread_mutex_unlock(&rw_mutex);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];

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
