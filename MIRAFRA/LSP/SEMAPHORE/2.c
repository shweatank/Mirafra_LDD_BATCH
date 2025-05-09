#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define SEM_NAME "/my_binary_semaphore"

int main() {
    sem_t *sem = sem_open(SEM_NAME, 0); // Open existing semaphore
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    printf("Process 2: Waiting for semaphore...\n");
    sem_wait(sem);  // Lock semaphore (P operation)

    printf("Process 2: Entering critical section\n");
    sleep(3); // Simulate work
    printf("Process 2: Leaving critical section\n");

    sem_post(sem);  // Unlock semaphore (V operation)

    sem_close(sem);
    return 0;
}

