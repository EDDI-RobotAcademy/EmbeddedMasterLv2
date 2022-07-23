/*

과제 : 
tail 도 해보기
적을때 말씀하시나요?
전체를 스캔하고 버퍼로 갖고있으면 어렵진않지만
효율적으로 만들기위해서 고민을 해봐야한다.
slap size를 256정도로 잡고
tail 만들기
파일 내용이 되도록이면 좀 그걸볼려면 1000자이상으로 해주는게 좋을것 같다.
그다음에 tail 기능을 구현
(비효율 방식과 효율방식)

버디와 슬랩을 같이쓰면 좋지만 슬랩을 256정도로만해서 이문제를 풀어보면 좋을것같다.
sleek 쓰기 딱좋은 예제이다.

*/

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

// for open dir
#include <stdlib.h>
#include <fcntl.h>

#define TAB_SIZE	8
#define BUDDY_PAGE_SIZE			4096
#define SLAB_PAGE_SIZE			64

char buf[BUDDY_PAGE_SIZE] = "";
char out_buf[BUDDY_PAGE_SIZE] = "";

int main(int argc, char **argv)
{
	struct stat stat_buf;
	
	int fd;
	int file_size;
	int tail_size;
	int count_newline = 0;

	if(argc > 2)
	{
		stat(argv[1], &stat_buf);

		fd = open(argv[1], O_RDWR);

		file_size = lseek(fd, -(BUDDY_PAGE_SIZE), SEEK_END);
		
		read(fd, buf, BUDDY_PAGE_SIZE-1);

		tail_size = atoi(argv[2]);

		for(int i = BUDDY_PAGE_SIZE-1 ; i > 0 ; i--)
		{
			if(buf[i] == '\n')
			{
					count_newline++;
			}

			if(count_newline >= tail_size)
			{
				lseek(fd, -(BUDDY_PAGE_SIZE)+i, SEEK_END);
				break;
			}

			if(i == 0)
			{
				// 
			}
		}

		printf("result\n====================================================\n");
		read(fd, out_buf, BUDDY_PAGE_SIZE);
		printf("%s\n", out_buf);
		
		close(fd);
	}
	else
	{
		printf("사용법 : 읽어올 긴글, 읽어올 tail 줄수");
		return -1;
	}

	return 0;
}