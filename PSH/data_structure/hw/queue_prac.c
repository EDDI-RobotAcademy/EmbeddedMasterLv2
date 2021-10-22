#include <stdio.h>
#include <stdlib.h>

typedef struct _queue queue;
struct _queue
{
	int data;
	struct _queue *link;
};

queue* create_queue_node(void)
{
	queue *tmp = (queue*)malloc(sizeof(queue));
	tmp->link = 0;
	
	return tmp;
}

void enqueue_data(queue **head, int data) 
{ 
	if(!(*head)) 
	{
		*head = create_queue_node();
		(*head)->data = data;
		return;
	}
	
	enqueue_data(&(*head)->link, data);
}

void enqueue_data_idx(queue **head, int data, int idx)
{
	if(!idx)
	{
		queue *tmp = create_queue_node();
		tmp->data = data;
		tmp->link = *head;
		*head = tmp;
		return;
	}

	if(!(*head))
	{
		printf("작업 불가\n");  
		return;
	}
	
	enqueue_data_idx(&(*head)->link, data, --idx);
}

void dequeue_data(queue **head)
{
	if(!(*head))
	{
		printf("queue is empty!\n");
		return;
	}

	queue *tmp = *head;
	printf("dequeue data = %d\n", (*head)->data); 
	*head = (*head)->link;	
	free(tmp);
}

void dequeue_data_idx(queue **head, int idx)
{
	if(!idx)
	{
		queue *tmp = *head;
		printf("dequeue data = %d\n", (*head)->data);
		*head = (*head)->link;
		free(tmp);
		return;
	}
	
	if(!(*head))
	{
		printf("작업 불가\n");
		return;
	}

	dequeue_data_idx(&(*head)->link, --idx);
}

void print_queue(queue* head)
{
	while(head)
	{
		printf("queue data = %d\n", head->data);
		head = head->link;
	}
	
	printf("\n"); 	
}

int main(void)
{
	queue *head = NULL;
	int i;
	int data[] = {10, 20, 30, 40};
	
	for(i = 0; i < 4; i++)
	{
		enqueue_data(&head, data[i]);
	}

	print_queue(head);

	for(i = 0; i < 5; i++)
	{
		dequeue_data(&head);
		print_queue(head);
	}

	for(i = 0; i < 4; i++)
	{
		enqueue_data(&head, data[i]);
	}
	
//	enqueue_data_idx(&head, 35, 0);
	enqueue_data_idx(&head, 33, 2);
//	enqueue_data_idx(&head, 34, 5);

	print_queue(head);

//	dequeue_data_idx(&head, 0);	
//	dequeue_data_idx(&head, 2);	
	dequeue_data_idx(&head, 5);	

	print_queue(head);

	return 0;
}

