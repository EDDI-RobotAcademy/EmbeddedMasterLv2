#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

#define SLAB_CACHE			64

int main(void)
{
	int read_fd, write_fd, ret;
	char read_buf[SLAB_CACHE];
	char write_buf[SLAB_CACHE];
	pid_t pid;

	if ((read_fd = open("writefifo", O_RDWR)) == -1)
    {
        printf("open error!\n");
        exit(1);
    }

	if ((write_fd = open("readfifo", O_RDWR)) == -1)
    {
        printf("open error!\n");
        exit(1);
    }

	if ((pid = fork()) > 0)
	{
		for (;;)
		{
			ret = read(0, write_buf, sizeof(write_buf));
			write_buf[ret] = 0;
			write(write_fd, write_buf, ret);
		}
	}
	else if (pid == 0)
	{
		for (;;)
		{
			ret = read(read_fd, read_buf, sizeof(read_buf));
			read_buf[ret] = 0;
			printf("Someone: %s\n", read_buf);
		}
	}
	else
	{
		perror("fork() ");
		exit(-1);
	}

	close(read_fd);
	close(write_fd);

	return 0;
}
