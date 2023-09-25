#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define BILLION 1000000000L

char arr_str[50][9];
int id;

struct packet
{
    char text[9];
    int index;
};
// packets[5];

void receiver()
{
    int shmid = shmget((key_t)12345, 4196, 0666);
    int *index = (int *)shmat(shmid, 0, 0);
    //printf("received index : \n", index);
    int i=index;
    shmdt(index);
    // if (0 <= index < 50)
    // {
    //     return 0;
    // }
    // return 1;
    printf("received index : %d\n", id);
}

void sender(int i)
{
    int shmid = shmget((key_t)(12345), 10 * sizeof(struct packet), 0666 | IPC_CREAT);
    struct packet(*packets) = (struct packet *)shmat(shmid, (void *)0, 0);
    for (int j = 0; j < 5; j++)
    {
        packets[j].index = i + j;
        id = i + j;
        strcpy(packets[j].text, arr_str[i + j]);
    }
    shmdt(packets);
}

int main()
{
    struct timespec start, end;

    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            int a = rand() % 25;
            char c = 'a' + a;
            arr_str[i][j] = c;
        }
        arr_str[i][8] = '\0';
        printf("%d\t-\t%s\n", i + 1, arr_str[i]);
    }
    printf("\n");
    sleep(2);
    clock_gettime(CLOCK_REALTIME, &start);
    for (int i = 0; i < 50; i += 5)
    {
        sender(i);
        sleep(1);
        receiver();
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (double)BILLION;

    printf("time elapsed : %lf s\n", (double)time);

    return 0;
}