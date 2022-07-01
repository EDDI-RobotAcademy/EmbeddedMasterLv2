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

int main(int argc, char **argv)
{
	int fd;
	int str1_len, str2_len;
	int f_tot_size, f_cur_size;
	int str1_size, str2_size;
	char buf[BUDDY_PAGE_SIZE] = "";
	char *str_buf1, *str_buf2, *str_buf3;
	int nread;

	if(argc < 4)
	{
		printf("명령어 인수가 부족합니다!\n");
		printf(".out 파일경로 문자열1 문자열2로 입력하세요\n");
	}
	else if(argc > 4)
	{
		printf("명령어 인수가 너무 많습니다!\n");
		printf(".out 파일경로 문자열1 문자열2로 입력하세요\n");
	}

	fd = open(argv[1], O_RDWR);

	if(fd == ERROR)
	{
		printf("File open error!\n");
		return 1;
	}

	str1_len = strlen(argv[2]);
	str2_len = strlen(argv[3]);

	//초기 파일 전체 사이즈를 계산한다
	f_tot_size = lseek(fd, (off_t)0, SEEK_END);

	for(int i = 0; i < f_tot_size; i++)
	{
		//문자자열1과 문자열2가 서로 사이즈가 다를 경우 파일의 크기가 바뀌므로 다시 계산한다
		f_tot_size = lseek(fd, (off_t)0, SEEK_END);
		//파일의 맨 앞부터 1바이트씩 seek_pointer의 위치를 변경
		f_cur_size = lseek(fd, (off_t)i, SEEK_SET);

		//문자열1의 크기 만큼 파일의 내용을 읽어 buf에 저장한다
		if(nread = read(fd, buf, (size_t)str1_len) < 0)
		{
			printf("read file error\n");
			return 1;
		}
		//입력받은 문자열과 파일의 문자열이 일치할 때
		if(!strcmp(argv[2], buf))
		{
			//문자열1의 다음 내용을 버퍼에 저장한다
			str2_size = f_tot_size - (int)lseek(fd, (off_t)0, SEEK_CUR);
			str_buf2 = (char *)malloc(sizeof(char) * str2_size);
			read(fd, str_buf2, str2_size);

			//문자열1을 문자열2로 교체한다
			lseek(fd, i, SEEK_SET);
			write(fd, argv[3], str2_len);
			str1_size = (int)lseek(fd, (off_t)0, SEEK_CUR);
			lseek(fd, (off_t) -str1_size, SEEK_CUR);
			str_buf1 = (char *)malloc(sizeof(char) * str1_size);
			read(fd, str_buf1, sizeof(char) * str1_size);

			//문자열2 다음에 저장한 문자열1의 다음 내용을 붙여 넣는다
			str_buf3 = (char *)malloc(sizeof(char) * (str1_size + str2_size));
			memcpy(str_buf3, str_buf1, str1_size);
			memcpy(str_buf3+str1_size, str_buf2, str2_size);

			//파일을 TRUNCATE모드로 다시 열어 수정된 내용을 쓰고 파일을 닫는다
			close(fd);
			fd = open(argv[1], O_RDWR|O_TRUNC);
			write(fd, str_buf3, (size_t)(str1_size+str2_size));
			close(fd);

			free(str_buf1);
			free(str_buf2);
			free(str_buf3);

			fd = open(argv[1], O_RDWR);
		}
	}

	lseek(fd, (off_t)0, SEEK_SET);
	read(fd, buf, BUDDY_PAGE_SIZE);
	printf("after change buf = %s\n", buf);

	close(fd);

	return 0;
}
