#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

struct packet
{
    char text[9];
    int index;
} packets[5];

void sender()
{
    int fd = open("ques2", O_WRONLY);
    write(fd, &(packets[4].index), sizeof(int));
    close(fd);
}
void receiver()
{
    int fd = open("ques2", O_RDONLY);
    read(fd, &packets, sizeof(packets));
    close(fd);
    for (int j = 0; j < 5; j++)
    {
        printf("%d\t:\t%s\n", packets[j].index, packets[j].text);
    }
    printf("\n");
}

int main()
{
    for (int i = 0; i < 50;)
    {
        receiver();
        sender();
        i += 5;
    }
}