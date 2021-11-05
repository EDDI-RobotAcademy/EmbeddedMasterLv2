#include <stdlib.h>
#include <stdio.h>

typedef struct _queue queue;
struct _queue
{
	int data;
	struct _queue *link;
};

queue *create_queue_node(void)
{
	queue *tmp;

	tmp = (queue *)malloc(sizeof(queue));
	tmp->link = 0;

	return tmp;
}

void enqueue_data(queue **head, int data)
{
	if (!(*head))
	{
		*head = create_queue_node();
		(*head)->data = data;
		return;
	}

	// 요기서 뭘 해야할까요 ?
	enqueue_data(&(*head)->link, data);
}

void dequeue_data(queue **head)
{
	if (*head)
	{
		queue *tmp = *head;
		printf("dequeue data = %d\n", (*head)->data);
		*head = (*head)->link;
		free(tmp);
	}
	else
	{
		printf("Queue is empty!!!\n");
	}
}

void nr_enqueue_data_idx(queue **head, int data, int idx)
{
	queue *loop = *head;
	queue *backup = NULL;

	// 숏컷으로 예외처리 대신함
	while (loop && idx)
	{
		idx--;
		backup = loop;
		loop = loop->link;
	}

	if (!idx)
	{
		queue *tmp = create_queue_node();

		tmp->data = data;
		tmp->link = loop;

		if (!backup)
		{
			*head = tmp;
		}
		else
		{
			backup->link = tmp;
		}
		return;
	}

	printf("처리가 불가능한 작업입니다!!!\n");
}

void enqueue_data_idx(queue **head, int data, int idx)
{
	// 중간에 넣는 경우
	if (!idx)
	{
		queue *tmp = create_queue_node();

		tmp->data = data;
		tmp->link = *head;

		*head = tmp;
		return;
	}

	// 예외 핸들링
	if (!(*head))
	{
		printf("작업이 불가합니다!\n");
		return;
	}

	enqueue_data_idx(&(*head)->link, data, --idx);
}

void print_queue(queue *head)
{
	while (head)
	{
		printf("queue head = %d\n", head->data);
		head = head->link;
	}
}

int main(void)
{
	int i;
	queue *head = NULL;
	int data[] = { 10, 20, 30, 40 };

	for (i = 0; i < 4; i++)
	{
		enqueue_data(&head, data[i]);
	}

	print_queue(head);

	for (i = 0; i < 5; i++)
	{
		dequeue_data(&head);
	}

	for (i = 0; i < 4; i++)
	{
		enqueue_data(&head, data[i]);
	}

	// 특정 인덱스에 값 넣기
	//enqueue_data_idx(&head, 33, 2); - pass
	//enqueue_data_idx(&head, 33, 4); - pass
	//enqueue_data_idx(&head, 33, 0); - pass
	enqueue_data_idx(&head, 33, 5);
	//nr_enqueue_data_idx(&head, 33, 2); - pass
	//nr_enqueue_data_idx(&head, 33, 0); - pass
	//nr_enqueue_data_idx(&head, 33, 4); - pass
	nr_enqueue_data_idx(&head, 33, 5);
	print_queue(head);

#if 0
	// 특정 인덱스 값 빼기
	dequeue_data_idx(&head, 0);

	print_queue(head);
#endif

	return 0;
}
