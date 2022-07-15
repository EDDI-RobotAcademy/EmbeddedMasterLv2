#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define SLAB_CACHE			64
#define BUDDY_PAGE_SIZE		4096

typedef struct
{
	char name[SLAB_CACHE];
	int size;
}
tar_info;

#define min(x, y)			(((x) < (y)) ? (x) : (y))

int network_read(int fd, void *buf, int len)
{
	char *tmp = (char *)buf;
	int checksum = len;
	int ret;

	while (checksum > 0)
	{
		if ((ret = read(fd, tmp, checksum)) == 0)
		{
			return 0;
		}

		checksum -= ret;
		tmp += ret;
	}

	return len;
}

int main(int argc, char **argv)
{
	tar_info info;
	int src, dst, len, ret;
	char buf[BUDDY_PAGE_SIZE] = {'\0'};

	if (argc != 2)
	{
		printf("사용법: 묶인파일명\n");
		exit(-1);
	}

	if ((src = open(argv[1], O_RDONLY)) == -1)
	{
		exit(1);
	}

	printf("src fd = %d\n", src);

	while (network_read(src, &info, sizeof(info)))
	{
		printf("info.name = %s, size = %d\n", info.name, info.size);

		if ((dst = open(info.name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
		{
			exit(1);
		}

		printf("dst fd = %d\n", dst);

		while (info.size > 0)
		{
			len = min(sizeof(buf), info.size);
			printf("len = %d\n", len);

			ret = read(src, buf, len);
			printf("read byte = %d\n", ret);

			write(dst, buf, ret);
			info.size -= ret;
			printf("remaining info.size = %d\n", info.size);

			sleep(1);
		}

		close(dst);
	}

    close(src);

	return 0;
}
