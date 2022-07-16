#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>

#define BUDDY_SIZE		4096

int main(int argc, char **argv)
{
	char ch;
	int fd, enter_cnt = 0, backword_cnt = 0;
	char buf[BUDDY_SIZE] = { 0 };

	if (argc != 2)
	{
		printf("사용법: 파일명\n");
		exit(-1);
	}

	fd = open(argv[1], O_RDONLY);
	lseek(fd, 0, SEEK_END);

	while (enter_cnt < 10)
	{
		lseek(fd, --backword_cnt, SEEK_END);
		read(fd, &ch, 1);

		if (ch == '\n')
		{
			enter_cnt++;
		}
	}

	lseek(fd, backword_cnt, SEEK_END);

	read(fd, buf, sizeof(buf));
	printf("%s", buf);

	return 0;
}
