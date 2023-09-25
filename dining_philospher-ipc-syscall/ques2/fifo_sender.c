#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#define BILLION 1000000000L

char arr_str[50][9];

struct packet
{
	char text[9];
	int index;
} packets[5];

void receiver()
{
	int fd = open("ques2", O_RDONLY);
	int id;
	read(fd, &id, sizeof(int));
	close(fd);
	printf("\nreceived index : %d\n\n", id);
}

void sender(int i)
{
	int fd = open("ques2", O_WRONLY);
	for (int j = 0; j < 5; j++)
	{
		packets[j].index = i + j;
		strcpy(packets[j].text, arr_str[i + j]);
	}
	write(fd, &packets, sizeof(packets));
	close(fd);
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
	sleep(1);

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