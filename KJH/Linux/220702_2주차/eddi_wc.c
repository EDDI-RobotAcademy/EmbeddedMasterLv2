#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

static int line;
static int word;
static int cnt;

static bool flag = false;

void analysis_file(int fd, char *ch, int byte)
{
	while (read(fd, ch, byte))
	{
		cnt++;
		if (*ch == '\n')
		{
			line++;
		}
		else if (*ch != '\t' && *ch != ' ')
		{
			if (flag == false)
			{
				word++;
				flag = true;
			}
		}
		else
		{
			flag = false;
		}
	}
}

int main(int argc, char **argv)
{
	int fd;
	char ch;

	if (argc != 2)
	{
		printf("사용법: 분석할파일명\n");
		exit(-1);
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        exit(1);
    }

	analysis_file(fd, &ch, 1);

    close(fd);

	printf("단어: %d, 라인수: %d, 글자수: %d, 파일명: %s\n", word, line, cnt, argv[1]);

	return 0;
}
