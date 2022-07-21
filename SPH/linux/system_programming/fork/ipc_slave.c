#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#define SLABSIZE 64

int main(void)
{
	int fd_m, fd_s, ret;
	char buf[SLABSIZE];
	pid_t pid;

	mkfifo("fifo_slave", 0755);

	fd_m = open("fifo_master", O_RDONLY, 0644);
	fd_s = open("fifo_slave", O_WRONLY, 0644);

	if(!fd_m)
	{
		printf("master fifo open fail\n");
		exit(1);
	}
	else if(!fd_s)
	{
		printf("slave fifo open fail\n");
		exit(2);
	}

	if(pid = fork() > 0)
	{
		while(1)
		{
			ret = read(0, buf, sizeof(buf));
			buf[ret] = 0;
			printf("me : %s\n", buf);
			write(fd_s, buf, ret);

			memset(buf, 0, sizeof(buf));
		}
	}
	else if(pid == 0)
	{
		while(1)
		{
			ret = read(fd_m, buf, sizeof(buf));
			buf[ret] = 0;
			printf("opponent : %s\n", buf);

			memset(buf, 0, sizeof(buf));
		}
	}
	else
	{
		printf("fork() error\n");
		exit(-1);
	}
#if 0
	while(1)
	{
		ret = read(0, buf, sizeof(buf));
		buf[ret] = 0;
		printf("me : %s\n", buf);
		write(fd_s, buf, ret);

		ret = read(fd_m, buf, sizeof(buf));
		buf[ret] = 0;
		printf("opponent : %s\n", buf);
	}
#endif

	close(fd_m);
	close(fd_s);

	return 0;
}
