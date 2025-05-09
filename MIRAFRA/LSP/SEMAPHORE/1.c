#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SEM_NAME "/my_binary_semaphore"

int main() {
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1); // Create semaphore with initial value 1
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    printf("Process 1: Waiting for semaphore...\n");
    sem_wait(sem);  // Lock semaphore (P operation)

    printf("Process 1: Entering critical section\n");
    sleep(5); // Simulate work in the critical section
    printf("Process 1: Leaving critical section\n");

    sem_post(sem);  // Unlock semaphore (V operation)

    sem_close(sem);
    return 0;
}

