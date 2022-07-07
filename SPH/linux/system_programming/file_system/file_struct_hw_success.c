#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define BUDDY_PAGE_SIZE 4096
#define SLAB_SIZE 64

#define ARRAY_MAX 100
#define DATA_MAX 100

#define LF_SIZE 1
#define STR_ONEBYTE 1

#define ASCII_ZERO 48
#define RADIX_DEC 10

#define abs(x) (x < 0) ? -x : x

typedef struct _file_queue file_queue;
struct _file_queue{
	int freq;
	int data;
	file_queue *next;
	file_queue *prev;
};

/*
   TODO
   1. 전달 받은 숫자를 절댓 값 계산
   2. 전달 받은 숫자가 0인 경우 자릿수를 1로 고정
   3. 0보다 작은 경우 루프 하한선을 1로 설정, 버퍼의 첫 번째 값은 -문자로 설정
   4. 0보다 큰 경우 자리수를 계산한다
   5. 배열의 최대 값(len)에 NULL문자 삽입, 계산된 자리수를 버퍼에 저장
   6. 정수를 문자로 변환
   7. 자리수 버퍼 반환
*/
int my_itoa(int num, char *str_buf)
{
	int i, tmp, num_size;
	int len = 0, min = 0;

	//1. 
	tmp = abs(num);

	//2. 
	if(num == 0)
		len = 1;
	//3.
	else if(num < 0)
	{
		min = 1;
		str_buf[0] = '-';
	}

	//4.
	while(num > 0)
	{
		num /= RADIX_DEC;
		len++;
	}

	//5.
	num_size = len;
	str_buf[len] = '\0';

	//6.
	while(len > min)
	{
		len--;
		str_buf[len] = (char)(tmp % RADIX_DEC + ASCII_ZERO);
		tmp /= RADIX_DEC;
	}

	//7.
	return num_size;
}

/*
   TODO
   1. 첫 번째 문자가 '-'이면 부호 플래그 false, 배열 주소 1증가
   2. 배열이 NULL이 될때 까지 문자를 숫자로 변환
   *개행 문자 발견시 루프 탈출
   3. 부호 플래그가 false면 -를 곱하여 반환
*/
int my_atoi(char *str_buf)
{
	int num = 0;
	bool sign = true;

	//1.
	if(*str_buf == '-')
	{
		sign = false;
		str_buf++;
	}

	//2.
	while(*str_buf)
	{
		//*
		if(*str_buf == '\n')
			break;
		num = num * 10 + (int)(*str_buf - ASCII_ZERO);
		str_buf++;
	}

	//3.
	return (sign) ? num : -num;
}

int *set_fd_size(int f_num)
{
	int *tmp;
	tmp = (int *)malloc(sizeof(int) * f_num);
	return tmp;
}

/*
   TODO
   1. 랜덤 숫자를 문자열로 변경한다
   2. 변경된 문자열을 전달 받은 파일디스크립터를 통해 파일에 쓴다
   3. 문자열을 세로로 정렬한다
*/
void write_rand_data(char **f_ptr, int f_num)
{
	int i, j= 0;
	char str[SLAB_SIZE];
	char lf = '\n';
	int len;
	int *fd;

	fd = set_fd_size(f_num);

	for(i = 0; i < f_num; i++)
	{
		fd[i] = open(f_ptr[i], O_WRONLY|O_CREAT, 0644);

		if(fd[i] <= 0)
			exit(1);

		for(j = 0; j < ARRAY_MAX; j++)
		{
			len = my_itoa((int)rand() % DATA_MAX, str);
			write(fd[i], str, len);
			write(fd[i], &lf, LF_SIZE);
		}
		close(fd[i]);
	}
	free(fd);
}

/*
   TODO
   1. 파일을 RDONLY로 open한다
   2. open 통해 전달받은 fd를 통해 파일의 내용을 읽는다
   3. 개행문자로 구분된 파일의 데이터 읽기
*/
void f_enqueue(char **f_name, int f_num, file_queue **head)
{
	int *fd;
	int i, j;
	int num, f_tot_size;
	int cur_pos = 0, cur_size = 0;

	char buf[BUDDY_PAGE_SIZE];
	char *str_buf;

	fd = set_fd_size(f_num);

	for(i = 0; i < f_num; i++)
	{

		fd[i] = open(f_name[i], O_RDONLY, 0644);

		if(fd[i] == 0)
		{
			printf("file open fail\n");
			exit(1);
		}

		f_tot_size = lseek(fd[i], (off_t)0, SEEK_END);
		lseek(fd[i], (off_t)0, SEEK_SET);

		for(j = 0; j < f_tot_size; j++)
		{
			read(fd[i], &buf[j], STR_ONEBYTE);
			cur_size++;
			if(buf[j] == '\n')
			{
				lseek(fd[i], (off_t)cur_pos, SEEK_SET);
				str_buf = (char *)malloc(sizeof(char) * (cur_size));
				read(fd[i], str_buf, cur_size);
				num = my_atoi(str_buf);
				printf("str_buf = %snum = %d\n", str_buf, num);
				cur_pos = lseek(fd[i], (off_t)0, SEEK_CUR);
				cur_size = 0;
				free(str_buf);
			}
		}
		cur_pos = 0;
		close(fd[i]);
	}
	free(fd);
}

/*
	1. 파일을 create, rdwr모드로 연다
	2. 파일에 랜덤 숫자를 쓴다
	3. 파일에서 숫자를 읽는다
	3. 파일 큐에 enqueue한다
*/
int main(void)
{
	file_queue *f_queue = NULL;
	char *f_name[2] = {"test1.txt", "test2.txt"};

	write_rand_data(f_name, 2);
	f_enqueue(f_name, 2, NULL);

	return 0;
}
