#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define true 1

int forks[5];

void eat(int i, int a, int b)
{
    printf("Philospher %d is eating using %d and %d forks.\n", i + 1, a + 1, b + 1);
}

void think(int i)
{
    printf("Philospher %d is thinking.\n", i + 1);
}

void *dining_philospher(int i)
{
    for (int j = 0;;)
    {
        // sleep(1);
        int a = i % 5;
        int b = (i + 1) % 5;
        if (i % 2)
        {
            while (!forks[a])
            {
                forks[a] = 1;
                while (!forks[b])
                {
                    forks[b] = 1;
                    eat(i, a, b);
                    // sleep(1);
                }
            }
            forks[a] = 0;
            forks[b] = 0;
            // sleep(2);
        }
        else
        {
            while (!forks[b])
            {
                forks[b] = 1;
                while (!forks[a])
                {
                    forks[a] = 1;
                    eat(i, a, b);
                    // sleep(1);
                }
            }
            forks[a] = 0;
            forks[b] = 0;
        }
        think(i);
    }
}

int main()
{
    pthread_t philospher[5];
    for (int i = 0; i < 5; i++)
        forks[i] = 0;

    for (int i = 0; i < 5; i++)
        pthread_create(&philospher[i], NULL, (void *)dining_philospher, (int *)i);
    for (int i = 0; i < 5; i++)
        pthread_join(philospher[i], NULL);
    // for(int i=0;i<5;i++)
    //     pthread_mutex_destroy(&forks[i]);

    return 0;
}