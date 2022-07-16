#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define STD_IN 0
#define STD_OUT 1
#define ERR 2

#define ONE_BYTE 1

/*
   1. 파일 포인터와 라인수를 입력 받는다
   2. 파일 커서를 파일의 맨끝부터 맨 앞쪽으로 1byte씩 이동
   *예외 : NULL 문자를 고려하여 offset을 0이 아닌 1로 설정
   3. 파일을 1byte씩 읽는다
   *예외 : 실제 파일의 라인수가 입력받은 라인수 보다 작은경우
	       파일 사이즈가 0이면 루프를 강제 탈출
   4. 개행 문자를 만나면  라인 버퍼 1증가
   5. 라인버퍼 값이 입력받은 라인수 보다 작거나 같을 때 까지 반복
*/
void mv_end_to_nrow(int fd, int n)
{
	int i, line, cur_size;
	char ch;

	line = 0;
	i = (int)ONE_BYTE;
	lseek(fd, (off_t)0, SEEK_END);

	while(line <= n)
	{
		cur_size = lseek(fd, (off_t)-i, SEEK_END);
		if(cur_size == 0)
			break;
		read(fd, &ch, ONE_BYTE);
		if(ch == '\n')
			line++;
		i++;
	}
}

/*
   1. 파일 디스크립터 값과 라인수를 입력받는다
   2. 표준 출력으로 읽은 문자를 1byte씩 출력한다
   3. 개행문자를 만나면 라인 버퍼 1증가
   4. 읽은 파일 값이 0이 될 때까지(NULL 문자까지) 반복
   5. 라인버퍼와 입력 받은 라인 수가 같으면 루프 종료
*/
void display_file_nrow(int fd, int n)
{
	int line;
	char ch;

	while (read(fd, &ch, ONE_BYTE))
	{
		if (ch == '\n')
			line++;

		write(STD_OUT, &ch, ONE_BYTE);

		if (line == n)
			break;
	}
}

/*
   파일의 맨 끝에서 부터 10줄 읽는다
   *리눅스 tail 명령어의 default와 동일
   1. 파일 포인터를 원하는 위치로 옮기기
   2. 옮겨진 파일포인터부터 파일의 마지막까지 읽기
*/
int main(int argc, char **argv)
{
	int fd;
	int nrow;

	if (argc != 2)
	{
		printf("사용법: 파일명\n");
		exit(-1);
	}

	if ((fd = open(argv[1], O_RDONLY)) == -1)
    {
        exit(1);
    }

	mv_end_to_nrow(fd, 50);
	display_file_nrow(fd, 50);

	close(fd);

	return 0;
}
