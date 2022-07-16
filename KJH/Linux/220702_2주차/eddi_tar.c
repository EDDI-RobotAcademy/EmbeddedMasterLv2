#include <stdlib.h>
#include <string.h>
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

int file_size(int fd)
{
	int size, old;
	old = lseek(fd, 0, SEEK_CUR);
	size = lseek(fd, 0, SEEK_END);
	lseek(fd, old, SEEK_SET);

	return size;
}

int main(int argc, char **argv)
{
	tar_info info;
	int i, src, dst, ret;
	char buf[BUDDY_PAGE_SIZE];

	if (argc < 3)
	{
		printf("사용법: 묶음파일명 묶을것들1 ... \n");
		exit(-1);
	}

	if ((dst = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
	{
		exit(1);
	}

	for (i = 0; i < argc - 2; i++)
	{
		if ((src = open(argv[i + 1], O_RDONLY)) == -1)
	    {
	        exit(1);
	    }

		strcpy(info.name, argv[i + 1]);
		info.size = file_size(src);

		write(dst, &info, sizeof(info));

		while (ret = read(src, buf, sizeof(buf)))
		{
			write(dst, buf, ret);
		}

		close(src);
	}

    close(dst);

	return 0;
}
