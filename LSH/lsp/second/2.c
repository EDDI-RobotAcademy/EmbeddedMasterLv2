#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define FREQ_START		1

int buf_cnt;

#define SLABSIZE		64

typedef struct _queue queue;

struct _queue
{
	int data;
	int freq;
	struct _queue *link;
};

// create(AVL)
// create(TREE)
// create(RB)
// create(Q)
// create(STACK)
queue *create_queue_node(void)
{
	queue *tmp;

	tmp = (queue *)malloc(sizeof(queue));
	tmp->freq = FREQ_START;
	tmp->link = NULL;

	return tmp;
}

bool check_data_already_set(queue *head, int data)
{
	while (head)
	{
		if (head->data == data)
		{
			head->freq++;
			return true;
		}
		head = head->link;
	}

	return false;
}

queue *enqueue(queue *head, int data)
{
	queue *origin = head;
	queue *tmp;

	if (check_data_already_set(head, data))
	{
		return origin;
	}
	else
	{
		tmp = create_queue_node();
		tmp->data = data;
	}

	if (!head)
	{
		return tmp;
	}

	while (head->link)
	{
		head = head->link;
	}

	head->link = tmp;

	return origin;
}

void print_queue(queue *head)
{
	while (head)
	{
		printf("queue data: %d, freq: %d\n", head->data, head->freq);
		head = head->link;
	}
}

int *extract_buf(char *buf)
{
	int i;
	int start_idx = 0;
	// 실제로는 이 부분도 상황에 따라서 가변적이게 구성해야함
	int *tmp = (int *)malloc(sizeof(int) * SLABSIZE);
	char tmp_buf[SLABSIZE] = { 0 };

	for (i = 0; buf[i]; i++)
	{
		if (!strncmp(&buf[i], "\n", 1))
		{
			strncpy(tmp_buf, &buf[start_idx], i - start_idx);
			tmp[buf_cnt++] = atoi(tmp_buf);
			start_idx = i + 1;
			memset(tmp_buf, 0x00, SLABSIZE);
		}
	}

	return tmp;
}

void print_buf(int *buf)
{
	int i;

	for (i = 0; buf[i]; i++)
	{
		printf("%4d", buf[i]);
	}
	printf("\n");
}

int main(void)
{
	int i;
	queue *head = NULL;

	char first_buf[SLABSIZE] = { 0 };
	char second_buf[SLABSIZE] = { 0 };

	int *first_data = NULL;
	int *second_data = NULL;

	int first = open("test1.txt", O_RDONLY);
	int second = open("test2.txt", O_RDONLY);

	read(first, first_buf, SLABSIZE);
	read(second, second_buf, SLABSIZE);

	first_data = extract_buf(first_buf);
	print_buf(first_data);

	for (i = 0; i < buf_cnt; i++)
	{
		head = enqueue(head, first_data[i]);
	}
	buf_cnt = 0;

	second_data = extract_buf(second_buf);
	print_buf(second_data);

	for (i = 0; i < buf_cnt; i++)
	{
		head = enqueue(head, second_data[i]);
	}

	print_queue(head);

	return 0;
}
