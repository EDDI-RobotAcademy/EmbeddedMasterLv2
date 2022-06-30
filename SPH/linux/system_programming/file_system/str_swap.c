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

#if 0
######################TODO#########################
#1. 커맨드라인 인수로 파일명, 문자열1, 문자열2를 입력
#2. 파일을 RDWR 권한으로 open
#3. 문자열1과 문자열2의 길이 계산
#4. 파일의 전체 사이즈를 계산한다
#5. seek_pointer를 파일의 맨 처음부터 1byte씩 옮긴다
#6. 문자열1의 길이 만큼 read를 통해 buf에 저장
#7. buf와 문자열1 비교
#8. 문자열1 다음의 내용을 버퍼에 저장
#9. seek_pointer를 문자열1의 시작 위치로 변경한다 
#10. 문자열1의 시작 위치에서 문자열2를 문자열2 사이즈 만큼 write한다
#11. seek_pointer가 문자열2 다음 위치로 변경 되었으므로 이어서 변경전 문자열1 다음 내용을 write한다
#12. 위 4~11을 file total 사이즈 만큼 반복한다
********************예외상황*********************** 
#1. 문자열1과 문자열2의 사이즈가 서로 일치하지 않을 때
###################################################
#endif
//#1
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

	//#2
	//fd = open(argv[1], O_RDWR|O_SYNC);
	fd = open(argv[1], O_RDWR);

	if(fd == ERROR)
	{
		printf("File open error!\n");
		return 1;
	}

	//#3
	str1_len = strlen(argv[2]);
	str2_len = strlen(argv[3]);

	//초기 파일 전체 사이즈를 계산한다
	f_tot_size = lseek(fd, (off_t)0, SEEK_END);

	//#12
	for(int i = 0; i < f_tot_size; i++)
	{
		//#4 : 문자열1과 문자열2가 서로 사이즈가 다를 경우 파일의 크기가 바뀌므로 다시 계산한다
		f_tot_size = lseek(fd, (off_t)0, SEEK_END);
		//#5. 파일의 맨 앞부터 1바이트씩 seek_pointer의 위치를 변경
		f_cur_size = lseek(fd, (off_t)i, SEEK_SET);

		//#6 : 문자열1의 크기 만큼 파일의 내용을 읽어 buf에 저장한다
		if(nread = read(fd, buf, (size_t)str1_len) < 0)
		{
			printf("read file error\n");
			return 1;
		}
		//#7 : 입력받은 문자열과 파일의 문자열이 일치할 때
		if(!strcmp(argv[2], buf))
		{
			printf("f_tot_size = %d\n", f_tot_size);
			printf("f_cur_size = %d\n", f_cur_size);

			//문자열1의 다음 내용을 버퍼에 저장한다
			str2_size = f_tot_size - (int)lseek(fd, (off_t)0, SEEK_CUR);
			printf("str2_size = %d\n", str2_size);
			str_buf2 = (char *)malloc(sizeof(char) * str2_size);
			read(fd, str_buf2, str2_size);

			//문자열1을 문자열2로 교체한다
			lseek(fd, i, SEEK_SET);
			write(fd, argv[3], str2_len);
			str1_size = (int)lseek(fd, (off_t)0, SEEK_CUR);
			printf("str1_size = %d\n", str1_size);
			lseek(fd, (off_t) -str1_size, SEEK_CUR);
			str_buf1 = (char *)malloc(sizeof(char) * str1_size);
			read(fd, str_buf1, sizeof(char) * str1_size);

			printf("str_buf1 = %s\n", str_buf1);
			printf("str_buf2 = %s\n", str_buf2);

			str_buf3 = (char *)malloc(sizeof(char) * (str1_size + str2_size));
			memcpy(str_buf3, str_buf1, str1_size);
			memcpy(str_buf3+str1_size, str_buf2, str2_size);

			printf("str_buf3 = %s\n", str_buf3);

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
