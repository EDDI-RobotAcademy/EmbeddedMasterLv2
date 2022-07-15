#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUDDY_PAGE_SIZE				4096

int main(int argc, char **argv)
{
	int src, dst, ret;
	char buf[BUDDY_PAGE_SIZE];

	if (argc != 3)
	{
		printf("사용법: 복사할파일명 복사된파일명\n");
		exit(-1);
	}

	src = open(argv[1], O_RDONLY);
	dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (src == -1 && dst == -1)
    {
        exit(1);
    }

	while (ret = read(src, buf, sizeof(buf)))
	{
		write(dst, buf, ret);
	}

    close(src);
    close(dst);

	return 0;
}
