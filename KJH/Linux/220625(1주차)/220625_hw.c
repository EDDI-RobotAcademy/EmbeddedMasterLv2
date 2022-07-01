#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define BUDDY_PAGE_SIZE						4096
#define SLAB_CACHE							64
#define ERROR								-1

int main(void)
{
    off_t newpos;
	int fd, buf_len;
	ssize_t nread;
    ssize_t nwrite;
	char buf[BUDDY_PAGE_SIZE] = "";

    int buffer = 0;
	int cursor = 0;
	int fine_apple[200] = { 0, };
	int apple_cursor = 0;

    char *ptr;

	fd = open("test.txt", O_CREAT | O_RDWR/* | O_TRUNC*/, 0644);  // 권한을 설정하지않고 프로그램 재실행시 에러가 발생함 -> 권한이 랜덤으로 생성되기 때문인것으로 추정 (아닌가?)
    
	if (fd == ERROR)
	{
        printf("File open(%d) Error\n", fd);
		exit(1);
	}
    else
    {
        printf("File open success!!\n");
    }

	buf_len = strlen(buf);
	nwrite = write(fd, buf, buf_len);

	if (nwrite != buf_len)
	{
		printf("write error!\n");
		exit(2);
	}

	nread = read(fd, buf, sizeof(buf));
	printf("%s\n", buf);
    

    while ((nread = read(fd, buf, BUDDY_PAGE_SIZE)) > 0)
	{
		if (write(1, buf, nread) < nread)
		{
			close(fd);
		}
	}

	newpos = lseek(fd, (off_t)0, SEEK_SET);
    printf("file size (SEEK_SET) = %ld\n", newpos);

	newpos = lseek(fd, (off_t)0, SEEK_END);
	printf("file size (SEEK_END) = %ld\n", newpos);

	while(buffer+5 < newpos)
	{
		if(strncmp(buf, "apple", 5) == 0)
		{
			fine_apple[apple_cursor++] = buffer;
		}
		else
		{
			buffer++;
			lseek(fd, (off_t)buffer, SEEK_SET);
		}
	}

	for(int i = 0 ; i < apple_cursor ; i++)
	{
		printf("%d : %d\n", i+1, fine_apple[i]);

	}
    //strncmp()

    // nread = read(fd, buf, sizeof(buf));
    
    // ptr = strstr(buf, "apple");
    // while(ptr != NULL)
    // {
    //     printf("%s\n", ptr);
    //     ptr = strstr(ptr+1, "apple");
    // }
    // printf("%s\n", ptr);

    //while(buffer = strncmp(buf, "apple", 5) != ERROR)
    //{




	// buffer = strncmp(buf, "apple", 5);      // 버퍼에 apple 다섯글자가 있는지 확인 후 위치를 buffer 변수로 리턴받는다. -> 위치를 리턴 받지 않는다.
	// printf("buffer*%d\n", buffer);                // 위치 확인차 buffer 변수 출력

	// buffer += 5;                            // buf에 apple 이 있다면 다섯글자만큼 커서를 증가시킨다. (추후 scanf -> sizeof로 변경)
	// lseek(fd, (off_t)0+5, SEEK_SET);   // 커서의 포인터를 변경한다.

	// read(fd, buf, sizeof(buf));             // 변경된 포인터를 기준으로 파일을 다시 읽어온다. -> 4096이 나오는 이유 4kByte로 읽어서
	// printf("%s\n", buf);                      // 읽어온 파일 출력 (x) %s
	// printf("sizeof : %ld\n", sizeof(buf)/sizeof(char));						// -> %d 파일의 크기 출력

	// buffer = strncmp(buf, "apple", 5);      // -49 로 출력 -> 혹시 strncmp 하면서 lseek 로 한칸씩 움직여야 하나????
	// 										// 커서 포인터를 하나씩 증가시키면서 모든 파일을 탐색해야하나?? 너무 불합리하다.
	// 										// KMP 알고리즘을 구현해서 문자열을 찾도록하자.
	// printf("*%d\n", buffer);




		// while(buffer = strncmp(buf, "apple", 5) != 0)	// 일단 한칸씩 포인터를 이동하면서 apple이라는 글자를 찾는 탐색 알고리즘을 구현해 보았다.
		// {												// 
		// 	printf("*%d", buffer);

		// 	lseek(fd, (off_t)0 + (++cursor), SEEK_SET);
		// 	read(fd, buf, sizeof(buf));

		// 	printf("%s", buf);
		// }
    //}





	// if (nwrite = write(fd, buf, sizeof(buf)) == ERROR)
	// {
	// 	printf("write error!\n");
	// }

	// printf("fd = %d, buf = %s", fd, buf);

	// lseek(fd, (off_t)0, SEEK_SET);

	// while ((nread = read(fd, buf, BUDDY_PAGE_SIZE)) > 0)
	// {
	// 	printf("nread = %ld\n", nread);

	// 	if (write(1, buf, nread) < nread)
	// 	{
	// 		printf("print error\n");
	// 		close(fd);
	// 	}
	// }

	fd = close(fd);
    printf("File Close(%d)\n", fd);

	return 0;
}
