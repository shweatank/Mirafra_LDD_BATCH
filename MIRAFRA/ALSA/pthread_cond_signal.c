#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int shared_data_ready = 0;

void* waiter_thread(void* arg) {
    pthread_mutex_lock(&lock);
    
    while (shared_data_ready == 0) {
        printf("Waiting for signal...\n");
        pthread_cond_wait(&cond, &lock);  // Releases lock and waits
    }

    printf("Signal received. Proceeding...\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* signaler_thread(void* arg) {
    pthread_mutex_lock(&lock);
    shared_data_ready = 1;
    pthread_cond_signal(&cond);  // Wakes up one waiting thread
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, waiter_thread, NULL);
    pthread_create(&t2, NULL, signaler_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}

