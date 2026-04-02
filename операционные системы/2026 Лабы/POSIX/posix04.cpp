//----------- posix04 - синхронизация потоков через мьютекс
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int down=0;
pthread_mutex_t mutex;
void *thread(void *arg) {
    int num=*(int*)arg;
    printf("thread %d started\n", num);
    while(down==0){
        pthread_mutex_lock(&mutex);
        printf("%d a-a-a-a-a-a-a-a-a-a-a-a-a-a-a-a-a-a-a-a-a-a\n",num);
        printf("%d b-b-b-b-b-b-b-b-b-b-b-b-b-b-b-b-b-b-b-b-b-b\n",num);
        pthread_mutex_unlock(&mutex);
        }
    printf("thread %d finished\n", num);
    pthread_exit((void*)"!!!!!!!!!!!!!!!!!!!!!!!");
    }

int main(int argc, char *argv[]) {
    int res;
    pthread_t a_thread;
    void *thread_result;
    int num=1;
    int num2=2;
    pthread_mutex_init(&mutex, NULL);
    res = pthread_create(&a_thread, NULL, thread, (void*)&num);
    if (res != 0) { perror("error 1"); exit(EXIT_FAILURE); }
    res = pthread_create(&a_thread, NULL, thread, (void*)&num2);
    if (res != 0) { perror("error 2"); exit(EXIT_FAILURE); }
    sleep(10);
    down=1;
    sleep(5);
    pthread_mutex_destroy(&mutex);    
    exit(EXIT_SUCCESS);
    }
