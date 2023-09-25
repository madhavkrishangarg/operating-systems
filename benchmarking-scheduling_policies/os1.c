#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <sched.h>

#define BILLION 1000000000L

void *countA()
{
    struct sched_param param;
    struct timespec start, end;
    double time;
    long long count = 0;
    param.sched_priority =0;
    if(!pthread_setschedparam(pthread_self(), SCHED_OTHER, &param))
    {
        clock_gettime(CLOCK_REALTIME, &start);
        for (long long i = 0; i < 4294967296; i++)
        {
            count++;
            // printf("countA\n");
        }
        clock_gettime(CLOCK_REALTIME, &end);
    }
    else
    {
        printf("Error!");
    }
    time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/(double)BILLION;
    printf("time other: %lf\n", (double)time);
    // return NULL;
}

void *countB()
{
    struct sched_param param;
    struct timespec start, end;
    double time;
    long long count = 0;
    param.sched_priority = 40;
    if (!pthread_setschedparam(pthread_self(), SCHED_RR, &param))
    {
        clock_gettime(CLOCK_REALTIME, &start);
        for (long long i = 0; i < 4294967296; i++)
        {
            count++;
            // printf("countB\n");
        }
        clock_gettime(CLOCK_REALTIME, &end);
    }
    else
    {
        printf("Error!");
    }
    time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/(double)BILLION;
    printf("time rr : %lf\n", (double)time);
    // return NULL;
}

void *countC()
{
    struct sched_param param;
    struct timespec start, end;
    double time;
    long long count = 0;
    param.sched_priority = 45;
    if (!pthread_setschedparam(pthread_self(), SCHED_FIFO, &param))
    {
        clock_gettime(CLOCK_REALTIME, &start);
        for (long long i = 0; i < 4294967296; i++)
        {
            count++;
            // printf("countC\n");
        }
        clock_gettime(CLOCK_REALTIME, &end);
    }
    else
    {
        printf("Error!");
    }
    time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/(double)BILLION;
    printf("time fifo : %lf\n", (double)time);
    // return NULL;
}

int main()
{
    pthread_t p1, p2, p3;

    pthread_create(&p1, NULL, countA, NULL);
    pthread_create(&p2, NULL, countB, NULL);
    pthread_create(&p3, NULL, countC, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);

}
