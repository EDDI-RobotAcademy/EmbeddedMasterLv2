#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#include <time.h>

#define BUDDY_PAGE_SIZE 4096
#define SLAB_SIZE 64

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
void write_rand_data(int fd, int test_case)
{
	int i, j= 0;
	char str[SLAB_SIZE];
	char lf = '\n';
	int len;

	for(j = 0; j < test_case; j++)
	{
		len = my_itoa((int)rand() % test_case, str);
		write(fd, str, len);
		write(fd, &lf, LF_SIZE);
	}
}

/*
   TODO
   1. 전달 받은 head에 front 연결리스트가 있는지 파악한다
   2. front 연결리스트가 없으면 head하나만 있는 상태로 데이터 중복 체크를 한 후 중복이면 flag = true로 반환
      해당 queue의 freq 값을 1증가 시킨다
   3. front 연결리스트가 있으면 front 연결리스트로 옮기면서 중복 체크를 한다
   4. front 연결리스트의 data와 전달 받은 data가 같으면 front 연결리스트의 freq 값을 1증가
      중복 flag = true 루프를 종료한다 
*/
bool check_data_redundancy(int data, file_queue **head)
{
	file_queue *loop = *head;
	bool result = false;

	//1. & 2.
	if(!(*head)->front && (*head)->data == data)
	{
		(*head)->freq++;
		result = true;
		return result;
	}

	//3.
	while(loop->front)
	{
		//4.
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
	tmp->freq = 1;

	return tmp;
}
/*
   TODO
   1. head가 null 이면 head에 queue를 생성
   2. head가 null이 아니면 루프를 통해 연결리스트 체크
   3. 중복된 데이터가 들어오면 해당 데이터가 queue에 있는지 확인한다
   *중복된 데이터는 enqueue가 필요 없으므로 바로 return
   4. 연결리스트가 없으면 queue를 생성
*/
void enqueue(int data, file_queue **head)
{
	file_queue *front = NULL, *rear = NULL;

	//1.
	if(!(*head))
	{
		*head = create_queue();
		(*head)->data = data;
	}
	else
	{
		file_queue *new;
		file_queue *loop = *head;
		file_queue *front = NULL;

		//2.
		while(loop->rear)
		{
			//3.
			if(check_data_redundancy(data, &loop))
				return;

			front = loop;
			loop = loop->rear;
		}

		//4.
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

void print_queue(file_queue *head, int test_case)
{
	int i = 0, sum_freq = 0;

	printf("test_case = %d\n", test_case);
	while(head)
	{
		printf("queue data = %d, queue data freq = %d\n", head->data, head->freq);
		sum_freq += (head->freq != 1) ? (head->freq - 1) : 0;
		head = head->rear;
		i++;
	}
	printf("total data number = %d\n", i);
	printf("sum freq  = %d\n", sum_freq);

	if((test_case - sum_freq) == i)
	{
		printf("test_case - sum_freq = %d\n", test_case - sum_freq);
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
   TODO
	1. 명령줄 인수로 "실행파일 테스트 케이스 숫자 파일(n 개)"을 입력기
	2. 명령줄 인수 4개 미만인지 확인 후 에러 메시지 및 프로세스 종료
	3. 테스트 케이스 문자열을 정수로 변환
	4. 실행파일, 테스트 케이스를 제외한 파일의 개수 만큼 파일 쓰기, 읽기, enqueue를 실행
	(1) 파일을 쓰기 전용, 생성 모드로 열고 랜덤 데이터를 쓰고 파일을 닫기
	(2) 파일을 읽기 전용으로 열고 랜덤 데이터를 읽어 queue에 저장
	5. queue의 내용과 queue의 전체 데이터 개수를 구하고, 
	   (테스트 케이스 - queue 데이터의 빈도수) == queue의 전체 데이터 개수이면 통과
*/
int main(int argc, char **argv)
{
	file_queue *head = NULL;
	int *fd = set_fd_size(argc-1);
	int test_case;
	int file_num = 0;

	if(argc < 4)
	{
		printf("명령줄 인수가 부족합니다\n");
		printf("\"실행파일 테스트케이스 파일n ...\"로 다시 입력 하세요\n");
		exit(1);
	}

	test_case = my_atoi(argv[1]);

	for(int i = 0; i < argc-2; i++)
	{
		fd[i] = open(argv[i+2], O_WRONLY|O_CREAT, 0644);
		write_rand_data(fd[i], test_case);
		close(fd[i]);

		fd[i] = open(argv[i+2], O_RDONLY, 0644);
		read_queue_handler(fd[i], &head);
		close(fd[i]);

		file_num++;
	}
	free(fd);

	print_queue(head, test_case * file_num);
	free(head);

	return 0;
}
