#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define MAX_THREADS 16 //스레드의 개수 16개로 제한 

//thread_data 구조체 정의 A~B사이의 수의 소수를 찾기위해 
struct thread_data {
    int start; //시작할 수 (범위 지정)
    int end; //마지막 수 (범위 지정)
    int prime_count; //찾을 소수의 개수
};

//매개변수에 들어가는 숫자가 소수인지 아닌지 구하는 함수
int is_prime(int num) {
    if(num <= 1) {
        return 0;
    }
    //2부터 시작해서 몇번 반복할지 for문 설정
    for(int i = 2; i * i <= num; ++i) {
        if(num % i == 0) {
            return 0;
        }
    }
    return 1;
}

//각 스레드가 실행할 함수
void *find_primes(void *arg) {
    struct thread_data *data = (struct thread_data *)arg;
    data->prime_count = 0; //초기 구할 소수의 개수를 0으로 초기화

    for(int i = data->start; i <= data->end; ++i) {
        if(is_prime(i)) {
            data->prime_count++;
        }
    }
    pthread_exit(NULL); //스레드 종료
}

//argc(Argument Count)로 
int main(int argc, char *argv[]) {
    if(argc != 3) {
        fprintf(stderr, "%s <start> <end>\n", argv[0]);
        return 1;
    }

    int start = atoi(argv[1]); //시작범위 숫자
    int end = atoi(argv[2]); //끝 범위 숫자

    struct timeval start_time, end_time; //시간의 차이를 구하기위해서 timeval구조체 선언

    gettimeofday(&start_time, NULL); //단일 스레드로 소수찾는 시간 기록 시작
    int prime_count = 0; //초기 소수개수 0으로 초기화
    for(int i = start; i <= end; ++i) {
        if(is_prime(i)) {
            prime_count++;
        }
    }

    gettimeofday(&end_time, NULL); //단일 스레드로 소수 찾는 시간 종료

    double single_thread_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0; //단일 스레드로 소수 찾는데 걸리는 시간 계산 (종료시간 - 시작시간)

    gettimeofday(&start_time, NULL); //멀티 스레드로 소수찾는 시간 기록 시작
    pthread_t threads[MAX_THREADS]; //스레드 배열을 선언 스레드의 개수를 16개로 지정
    struct thread_data thread_data_array[MAX_THREADS]; //스레드 배열에 선언한 스레드들의 각 데이터 배열 구조체를 선언

    int thread_range = (end - start + 1) / MAX_THREADS; //스레드가 처리할 범위를 계산
    //각 스레드에게 할당할 범위 설정 
    for(int i = 0; i < MAX_THREADS; ++i) {
        thread_data_array[i].start = start + i * thread_range; //각 스레드에 할당된 숫자를 계산하게 
        thread_data_array[i].end = (i == MAX_THREADS - 1) ? end : thread_data_array[i].start + thread_range - 1; //각 스레드에 처리할 범위의 끝에 해당 하는 숫자를 계산
        pthread_create(&threads[i], NULL, find_primes, (void *)&thread_data_array[i]); //스레드 생성 
    }

    int multi_thread_count = 0;
    //모든 스레드의 작업이 종료될 때까지 대기하고 결과를 합산
    for(int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(threads[i], NULL);
        multi_thread_count += thread_data_array[i].prime_count;
    }

    gettimeofday(&end_time, NULL); //멀티 스레드 소수 찾는 시간 기록 종료
    //멀티 스레드로 소수 찾는 시간 계산
    double multi_thread_time = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    printf("Single Thread\n");
    printf("Processing Time: %.4f 초\n", single_thread_time);

    printf("Multi Thread\n");
    printf("Processing Time: %.4f 초\n", multi_thread_time);

    printf("Time Comparison:\n");
    printf("Multi Thread %.2f배 than Single Thread.\n", single_thread_time/ multi_thread_time);

}