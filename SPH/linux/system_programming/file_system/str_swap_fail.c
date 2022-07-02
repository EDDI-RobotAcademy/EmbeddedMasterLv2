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
	int fd, buf_len;
	int str1_len, str2_len;
	int f_tot_size, f_cur_size;
	char buf[BUDDY_PAGE_SIZE] = "";
	char *str_buf;
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
			//파일 커서를 파일의 맨 마지막 부터 파일의 전체 사이즈 - 0 ~ 변경할 문자열 사이즈에 위치시킴
			//위 의미는 문자열1 다음 위치에 커서가 위치하도록 함
			//f_tot_size는 lseek를 통해 SEEK_END까지 계산하면 문자열 총 사이즈+1(NULL문자)까지 계산됨
			//따라서 f_tot_size에서 1을 빼야 순수한 문자열 사이즈를 의미한다
			//#8 : 문자열1의 다음 내용을 버퍼에 저장한다
			f_cur_size = (f_tot_size - 1) - lseek(fd, (off_t)0, SEEK_CUR); 
			str_buf = (char *)malloc(sizeof(char)*f_cur_size);
			read(fd, str_buf, f_cur_size);

			printf("f_tot_size = %d\n", f_tot_size);
			printf("f_cur_size = %d\n", f_cur_size);
			printf("str_buf = %s\n", str_buf);

			//#9 : read로 인해 변경된 seek_pointer의 위치를 문자열1의 시작 위치로 변경한다
			lseek(fd, (off_t)i, SEEK_SET);

			//#10 : 문자열1의 시작 위치에서 문자열2를 문자열2 사이즈 만큼 write한다
			write(fd, argv[3], str2_len);
			//#11 : seek_pointer가 문자열2 다음 위치로 변경 되었으므로 이어서 변경전 문자열1 다음 내용을 write한다
			write(fd, str_buf, f_cur_size);
			free(str_buf);
		}
	}

#if 1
	lseek(fd, (off_t)0, SEEK_SET);
	read(fd, buf, BUDDY_PAGE_SIZE);
	printf("after change buf = %s\n", buf);
#endif
	close(fd);

	return 0;
}
