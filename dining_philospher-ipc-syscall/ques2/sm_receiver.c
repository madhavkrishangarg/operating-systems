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

int idx;

struct packet
{
    char text[9];
    int index;
};

void sender()
{
    int shmid = shmget((key_t)12345, 4196, 0666 | IPC_CREAT);
    int *id = (int *)shmat(shmid, (void *)0, 0);
    id = idx;
    printf("\nsending : %d\n\n", id);
    shmdt(id);
}

void receiver()
{
    int shmid = shmget((key_t)(12345), 10 * sizeof(struct packet), 0666 | IPC_CREAT);
    struct packet *packets = (struct packet *)shmat(shmid, (void *)0, 0);
    for (int j = 0; j < 5; j++)
    {
        printf("%d\t:\t%s\n", packets[j].index, packets[j].text);
    }
    idx = packets[4].index;
    printf("\n");
    shmdt(packets);
    //  if (packets[4].index < 50)
    //      return 1;
    //  return 0;
}

int main()
{
    for (int i = 0; i < 50; i += 5)
    {
        receiver();
        sender();
        sleep(1);
        // sender();
    }
}