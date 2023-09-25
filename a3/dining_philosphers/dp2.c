#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define true 1

sem_t forks[5];
sem_t sauce;

void eat(int i, int a, int b)
{
    printf("Philospher %d is eating using %d and %d forks with sauce bowl.\n", i + 1, a + 1, b + 1);
}

void think(int i)
{
    printf("Philospher %d is thinking.\n", i + 1);
}

void *dining_philospher(int i)
{
    while (true)
    {
        // sleep(1);
        int a = i % 5;
        int b = (i + 1) % 5;
        sem_wait(&sauce);
        if (i % 2)
        {
            sem_wait(&forks[a]);
            // sleep(2);
            sem_wait(&forks[b]);
        }
        else
        {
            sem_wait(&forks[b]);
            // sleep(2);
            sem_wait(&forks[a]);
        }
        eat(i, a, b);
        // sleep(1);
        sem_post(&sauce);
        sem_post(&forks[a]);
        sem_post(&forks[b]);
        think(i);
    }
}

int main()
{
    pthread_t philospher[5];
    for (int i = 0; i < 5; i++)
        sem_init(&forks[i], 0, 1);
    sem_init(&sauce, 0, 2);

    for (int i = 0; i < 5; i++)
        pthread_create(&philospher[i], NULL, (void *)dining_philospher, (int *)i);
    for (int i = 0; i < 5; i++)
        pthread_join(philospher[i], NULL);

    return 0;
}