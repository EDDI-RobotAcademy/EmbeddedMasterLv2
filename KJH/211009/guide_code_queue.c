#include <stdlib.h>
#include <stdio.h>

typedef struct _queue queue;
struct _queue
{
	int data;
	struct _queue *link;
};

void enqueue_data(queue **head, int data)
{
	if (!(*head))
	{
		*head = create_queue_node();
		(*head) -> data = data;
		return;
	}

	//요기서 뭘해야 할까요?????
}

void print_queue9queue *head)
{


int main(void)
{
	int i;
	queue *head = NULL;
	int data[] = { 10, 20, 30, 40 };

	for (i = 0 ; i < 4 ; i++)
	{
		enqueue_data(&head, data[i]);
	}

	print_queue(head);
#if 0
	for (i = 0; i< 5; i++)
	{
		dequeue_data(&head);
	}

	for (i = 0 ; i < 4 ; i++)
	{
		enqueue_data(&head, data[i]);
	}

	// 틍정 인덱스에 값 넣기
	
	enqueue_data(&head,

	// 특정 인덱스에 값 빼기
#endif
	return 0;
}

