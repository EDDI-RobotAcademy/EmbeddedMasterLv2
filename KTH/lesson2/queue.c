#include <stdlib.h>
#include <stdio.h>


#define recursive 0
#define not_recursive 1

typedef struct _queue queue;
struct _queue
{
	int data;
	struct _queue *link;
};

queue* create_queue_node(void)
{
	queue* tmp = malloc(sizeof(queue));
	tmp->link = NULL;
	return tmp;
}

#if recursive
void enqueue_data(queue **head, int data)
{
	if(!(*head))
	{
		*head = create_queue_node();
		(*head)->data = data;
		return;
	}
	enqueue_data(&(*head)->link,data);
	// TODO
	//
}
#endif

#if not_recursive
void enqueue_data(queue **head, int data)
{
	queue* tmp = *head;
	if(!(*head))
	{
		*head = create_queue_node();
		(*head)->data = data;
		return;
	}
	
	while(tmp->link)
	{
		tmp=tmp->link;
	}
	tmp->link = create_queue_node();
	(tmp->link)->data = data;
}
#endif

void print_queue(queue *head)
{
	if(!head)
	{
		printf("Empty queue\n");
		return;
	}
	while(head)
	{
		printf("queue head = %d\n", head->data);
		head = head->link;
	}
	
}

void dequeue_data(queue** head)
{
	if(*head)
	{
		queue* tmp = *head;
		*head=(*head)->link;
		free(tmp);
	}
	else
		printf("Cannot dequeue - Already Empty queue\n");
}

void enqueue_data_idx(queue** head, int data, int index)
{
	queue* tmp = *head;
	queue* tmp_prev = NULL;
	if(!index)
	{
		*head = create_queue_node();
		(*head)->data = data;
		(*head)->link = tmp;
		return;
	}
	while(index)
	{
		if(tmp)
		{
			tmp_prev = tmp;
			tmp=tmp->link;
			index--;
		}
		else
		{
			printf("index is over the range\n");
			return;
		}
	}
	tmp_prev->link = create_queue_node();
	tmp_prev->link->data = data;
	tmp_prev->link->link = tmp;
}

void dequeue_data_idx(queue** head, int index)
{
	queue* tmp = *head;
	queue* tmp_prev = NULL;
	if(!index)
	{
		*head = (*head)->link;
		free(tmp);
		return;
	}
	while(index)
	{
		if(tmp)
		{
			tmp_prev=tmp;
			tmp=tmp->link;
			index--;
		}
		else
		{
			printf("index is over the range\n");
			return;
		}
	}
	tmp_prev->link = tmp_prev->link->link;
	free(tmp);
}


int main(void)
{
	int i;
	queue *head = NULL;
	int data[] = { 10, 20, 30, 40 };

	for(i=0;i<4;i++)
	{
		enqueue_data(&head, data[i]);
	}

	print_queue(head);


	for(i=0; i<5;i++)
	{
		dequeue_data(&head);
	}

	print_queue(head);

	for(i=0;i<4;i++)
	{
		enqueue_data(&head, data[i]);
	}

	// 특정 인덱스에 값 넣기
	enqueue_data_idx(&head, 33, 2);
	printf("insert 33 at index 2\n");
	print_queue(head);
	enqueue_data_idx(&head, 1561, 0);
	printf("insert 1561 at index 0\n");
	print_queue(head);
	printf("insert 113 at index 112\n");
	enqueue_data_idx(&head, 113, 112);
	print_queue(head);
	// index is starting from 0

	// 특정 인덱스 값 빼기
	printf("delete node at index 0(first)\n");
	dequeue_data_idx(&head, 0);
	print_queue(head);

	printf("delete node at index 4(last)\n");
	dequeue_data_idx(&head, 4);
	print_queue(head);

	printf("delete node at index 1(middle)\n");
	dequeue_data_idx(&head, 1);
	print_queue(head);

	printf("delete node at index 100\n");
	dequeue_data_idx(&head,100);
	print_queue(head);
	return 0;
}

