#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#define SOCK_PATH "mysock"
#define BILLION 1000000000L
char arr_str[50][9];
int s, s2;

struct packet
{
    char text[9];
    int index;
} packets[5];

int idx;

void receiver()
{
    int a;
    if (recv(s, a, sizeof(packets), 0) == -1)
    {
        perror("receive");
        exit(1);
    }
    printf("\nreceived index : %d\n", a);
}

void sender(int i)
{
    for (int j = 0; j < 5; j++)
    {
        packets[j].index = i + j;
        strcpy(packets[j].text, arr_str[i + j]);
    }
    if (send(s, packets, sizeof(packets), 0) == -1)
    {
        perror("send");
        exit(1);
    }
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

    int s, t, len;
    struct sockaddr_un remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    printf("connecting...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    clock_gettime(CLOCK_REALTIME, &start);
    if (connect(s, (struct sockaddr *)&remote, len) == -1)
    {
        perror("connect: ");
        exit(1);
    }
    printf("connected.\n");

    for (int i = 0; i < 50; i += 5)
    {
        for (int j = 0; j < 5; j++)
        {
            packets[j].index = i + j;
            strcpy(packets[j].text, arr_str[i + j]);
        }
        if (send(s, packets, sizeof(packets), 0) == -1)
        {
            perror("send");
            exit(1);
        }
        int a;
        if (recv(s, a, sizeof(packets), 0) == -1)
        {
            perror("receive");
            exit(1);
        }
        printf("\nreceived index : %d\n", a);
    }

    close(s);
    clock_gettime(CLOCK_REALTIME, &end);
    double time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / (double)BILLION;
    printf("time elapsed : %lf s\n", (double)time);
    return 0;
}