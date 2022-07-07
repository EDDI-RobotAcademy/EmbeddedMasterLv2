#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <string.h>

#include <unistd.h>
#include <fcntl.h>

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
	file_queue *front;
	file_queue *rear;
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

/*
   TODO
   1. 랜덤 숫자를 문자열로 변경한다
   2. 변경된 문자열을 전달 받은 파일디스크립터를 통해 파일에 쓴다
   3. 문자열을 세로로 정렬한다
*/
void write_rand_data(int fd)
{
	int i, j= 0;
	char str[SLAB_SIZE];
	char lf = '\n';
	int len;

	for(j = 0; j < ARRAY_MAX; j++)
	{
		len = my_itoa((int)rand() % DATA_MAX, str);
		write(fd, str, len);
		write(fd, &lf, LF_SIZE);
	}
}

bool check_data_redundancy(int data, file_queue **head)
{
	file_queue *loop = *head;
	bool result = false;

	if(!(*head)->front && (*head)->data == data)
	{
		(*head)->freq++;
		result = true;
		return result;
	}

	while(loop->front)
	{
		if(loop->data == data)
		{
			loop->freq++;
			result = true;
			break;
		}
		loop = loop->front;
	}

	return result;
}

file_queue *create_queue(void)
{
	file_queue *tmp;

	tmp = (file_queue *)malloc(sizeof(file_queue));
	tmp->front = NULL;
	tmp->rear = NULL;
	tmp->freq = 0;

	return tmp;
}
/*
   TODO
   1. head가 null 이면 queue를 생성
   2. 중복된 데이터가 들어오면 해당 데이터가 queue에 있는지 확인한다
   3. front 값을 기준으로 check한다
*/
void enqueue(int data, file_queue **head)
{
	file_queue *front = NULL, *rear = NULL;

	if(!(*head))
	{
		*head = create_queue();
		(*head)->data = data;
		(*head)->freq = 0;
	}
	else
	{
		file_queue *new;
		file_queue *loop = *head;
		file_queue *front = NULL, *rear = NULL;

		while(loop->rear)
		{
			front = loop;
			loop = loop->rear;
		}

		if(check_data_redundancy(data, &loop))
			return;

		new = create_queue();
		new->data = data;
		new->front = front;
		loop->rear = new;
	}
}

/*
   TODO
   1. 파일을 RDONLY로 open한다
   2. open 통해 전달받은 fd를 통해 파일의 내용을 읽는다
   3. 개행문자로 구분된 파일의 데이터 읽기
*/
void read_queue_handler(int fd, file_queue **head)
{
	int i, num, f_tot_size;
	int cur_pos = 0, cur_size = 0;
	char buf[BUDDY_PAGE_SIZE];
	char *str_buf;

	f_tot_size = lseek(fd, (off_t)0, SEEK_END);
	lseek(fd, (off_t)0, SEEK_SET);

	for(i = 0; i < f_tot_size; i++)
	{
		read(fd, &buf[i], STR_ONEBYTE);
		cur_size++;

		if(buf[i] == '\n')
		{
			lseek(fd, (off_t)cur_pos, SEEK_SET);
			str_buf = (char *)malloc(sizeof(char) * (cur_size));
			read(fd, str_buf, cur_size);
			num = my_atoi(str_buf);
			enqueue(num, head);
			cur_pos = lseek(fd, (off_t)0, SEEK_CUR);
			cur_size = 0;
			free(str_buf);
		}
	}
}

void print_queue(file_queue *head)
{
	int i = 0, sum_freq = 0;
	while(head)
	{
		printf("queue data = %d, queue data freq = %d\n", head->data, head->freq);
		sum_freq += head->freq;
		head = head->rear;
		i++;
	}
	printf("total data number = %d\n", i);
	printf("sum freq  = %d\n", sum_freq);
	if(((DATA_MAX*2) - sum_freq) == i)
	{
		printf("DATA MAX * 2 - sum_freq = %d\n", DATA_MAX*2 - sum_freq);
		printf("test pass!\n");
	}
	printf("queue is empty\n");
}

int *set_fd_size(int f_num)
{
	int *tmp;
	tmp = (int *)malloc(sizeof(int) * f_num);
	return tmp;
}
/*
	1. 파일을 create, rdwr모드로 연다
	2. 파일에 랜덤 숫자를 쓴다
	3. 파일에서 숫자를 읽는다
	3. 파일 큐에 enqueue한다
*/
int main(int argc, char **argv)
{
	file_queue *head = NULL;
	int *fd = set_fd_size(argc-1);

	for(int i = 0; i < argc-1; i++)
	{
		fd[i] = open(argv[i+1], O_WRONLY|O_CREAT, 0644);
		write_rand_data(fd[i]);
		close(fd[i]);

		fd[i] = open(argv[i+1], O_RDONLY, 0644);
		read_queue_handler(fd[i], &head);
		close(fd[i]);
	}
	free(fd);

	print_queue(head);

	return 0;
}
