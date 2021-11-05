#include <stdlib.h>
#include <stdio.h>

#define Guide_Code 1

typedef struct _queue queue;
struct _queue
{
	int data;
	struct _queue *link;
};

queue *create_queue_node()
{
	queue *tmp;
	tmp = (queue *)malloc(sizeof(queue));
	tmp->link = NULL;
	return tmp;
}

queue *enqueue_data(queue **head, int data)
{
	queue *tmp = NULL;
#if Guide_Code
	if (!(*head))
	{
		*head = create_queue_node();
		(*head)->data = data;
		tmp = *head;
		return tmp;
	}
	// 요기서 뭘 해야할까요 ?
	tmp = enqueue_data(&(*head)->link, data);
	return tmp;
#else
	if(!(*head))
	{
		*head = create_queue_node();
		(*head)->data = data;
		return;
	}
	else
	{
		queue *backup = *head;
		queue *new = NULL;

		while(backup->link)
		{
			backup = backup->link;
		}
		new = create_queue_node();
		new->data = data;
		backup->link = new;
		return;
	}
#endif
}

int dequeue_data(queue **head)
{
	int data;
	queue *tmp = *head;

	data = (*head)->data;
	(*head) = tmp->link;
	free(tmp);
	return data;
}

void enqueue_data_idx(queue **head, int data, int idx)
{
	//Queue에 중간에 데이터를 넣을 때
	if(!idx)
	{
		queue *tmp = create_queue_node();
		tmp->link = *head;
		tmp->data = data;
		*head = tmp;
		return;
	}
	//예외처리(데이터가 하나도 없는 경우, index값이 queue사이즈보다 큰 경우)
	if(!(*head))
	{
		printf("Can't enqueue data\n");
		return;
	}
	//enqueue_data_idx(&(*head)->link, data, --idx);
	enqueue_data_idx(&(*head)->link, data, idx--); //후위 연산자 사용시 코어 덤프 발생
}

int dequeue_data_idx(queue **head, int idx)
{
	int data;

	//index값이 Queue의 사이즈보다 클 때
	if(!(*head))
	{
		printf("Index is over than queue size\n");
		return -1;
	}
	//Queue 중간의 데이터를 뺄 때
	if(!idx)
	{
		queue *tmp = *head;
		data = tmp->data;
		*head = tmp->link;
		free(tmp);
		return data;
	}
	return dequeue_data_idx(&(*head)->link, --idx);
}

void print_queue(queue *head)
{
	while (head)
	{
		printf("queue = %d\n", head->data);
		head = head->link;
	}
}

int main(void)
{
	int i;
	queue *head = NULL;
	int data[] = { 10, 20, 30, 40 };

#if Guide_Code
	for (i = 0; i < 4; i++)
	{
		head = enqueue_data(&head, data[i]);
	}
	print_queue(head);

	for (i = 0; i < 5; i++)
	{
		if(!head)
		{
			printf("Queue is Empty\n");
			break;
		}
		printf("Dequeue data = %d\n", dequeue_data(&head));
		print_queue(head);
	}
	printf("\n");

	for (i = 0; i < 4; i++)
	{
		enqueue_data(&head, data[i]);
	}
	print_queue(head);

	// 특정 인덱스에 값 넣기
	//enqueue_data_idx(&head, 33, 2);
	enqueue_data_idx(&head, 35, 3);
	//enqueue_data_idx(&head, 50, 5);
	print_queue(head);

	// 특정 인덱스 값 빼기
	//dequeue_data_idx(&head, 0);
	//printf("Dequeue Index Data = %d\n", dequeue_data_idx(&head, 0)); 
	//printf("Dequeue Index Data = %d\n", dequeue_data_idx(&head, 1));
	//printf("Dequeue Index Data = %d\n", dequeue_data_idx(&head, 2));
	//printf("Dequeue Index Data = %d\n", dequeue_data_idx(&head, 3));
	//printf("Dequeue Index Data = %d\n", dequeue_data_idx(&head, 4));
	printf("Dequeue Index Data = %d\n", dequeue_data_idx(&head, 5));
#else
	printf("Start Enqueue!\n");
	for (i = 0; i < 4; i++)
	{
		enqueue_data(&head, data[i]);
	}

	print_queue(head);

	printf("Start Dequeue!\n");
	for (i = 0; i < 5; i++)
	{
		if(!head)
		{
			printf("Queue is Empty\n");
			break;
		}
		printf("Dequeue data = %d\n", dequeue_data(&head));
		print_queue(head);
	}
#endif
	return 0;
}
