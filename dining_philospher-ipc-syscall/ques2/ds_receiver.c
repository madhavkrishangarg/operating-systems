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
int idx;

struct packet
{
    char text[9];
    int index;
} packets[5];
int done;

void receiver()
{
    if (recv(s2, packets, sizeof(packets), 0) == -1)
    {
        perror("receive");
        exit(1);
    }
    for (int j = 0; j < 5; j++)
    {
        printf("%d\t:\t%s\n", packets[j].index, packets[j].text);
    }
    done = 1;
    // idx = packets[4].index;
}

void sender()
{
    if (!done)
    {
        printf("sending index : %d", idx);
        if (send(s2, packets[4].index, 2048, 0) == -1)
        {
            perror("send");
            exit(1);
        }
    }
}

int main()
{
    int len;
    unsigned t;
    struct sockaddr_un local, remote;

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket: ");
        exit(1);
    }
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    bind(s, (struct sockaddr *)&local, len);
    listen(s, 5);

    for (int i = 0; i < 50; i += 5)
    {
        printf("waiting for connection");
        t = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1)
        {
            perror("accept : ");
            exit(1);
        }
        done = 0;
        if (recv(s2, packets, sizeof(packets), 0) == -1)
        {
            perror("receive");
            exit(1);
        }
        for (int j = 0; j < 5; j++)
        {
            printf("%d\t:\t%s\n", packets[j].index, packets[j].text);
        }
        done = 1;
        if (!done)
        {
            printf("sending index : %d", idx);
            if (send(s2, packets[4].index, 2048, 0) == -1)
            {
                perror("send");
                exit(1);
            }
        }
        close(s2);
    }

    return 0;
}