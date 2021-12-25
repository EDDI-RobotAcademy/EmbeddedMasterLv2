#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX			7

int front = -1;
int rear = -1;

bool is_cqueue_full(void)
{
	if ((front == rear + 1) || (front == 0 && rear == MAX - 1))
		return true;

	return false;
}

bool is_cqueue_empty(void)
{
	if (front == -1)
		return true;

	return false;
}

void cqueue_ins_data(int *cqueue, int data)
{
	if (is_cqueue_full())
		printf("허용할 수 있는 범주를 초과함\n");
	else
	{
		if (front == -1)
			front = 0;

		rear = (rear + 1) % MAX;
		cqueue[rear] = data;
		printf("%d 입력 완료\n", data);
	}
}

int cqueue_rm_data(int *cqueue)
{
	int elem;

	if (is_cqueue_empty())
	{
		printf("Queue가 비었음\n");
		return -1;
	}
	else
	{
		elem = cqueue[front];

		if (front == rear)
		{
			front = -1;
			rear = -1;
		}
		else
			front = (front + 1) % MAX;
	}

	printf("%d 제거 완료\n", elem);

	return elem;
}

void print_cqueue(int *cqueue)
{
	int i;

	if (is_cqueue_empty())
		printf("cqueue가 비었음\n");
	else
	{
		printf("front: %d\n", front);

		for (i = front; i != rear; i++)
			printf("cqueue[%d] = %d\n", i, cqueue[i]);

		printf("cqueue[%d] = %d\n", i, cqueue[i]);
		printf("rear: %d\n", rear);
	}
}

int main(void)
{
	int i, j;
	int cqueue[MAX] = { 0 };
	int ten_data = 10;

	for (i = 0; i < 10; i++)
		cqueue_ins_data(cqueue, ten_data * (i + 1));

	print_cqueue(cqueue);

	for (i = 0; i < 10; i++)
		cqueue_rm_data(cqueue);

	print_cqueue(cqueue);

	for (i = 0; i < 10; i++)
	{
		cqueue_ins_data(cqueue, i);
		printf("데이터 입력 완료\n\n");

		for (j = i; j >= 6; j--)
		{
			cqueue_rm_data(cqueue);
			printf("데이터 제거 완료\n\n");
		}
	}

	print_cqueue(cqueue);

	return 0;
}