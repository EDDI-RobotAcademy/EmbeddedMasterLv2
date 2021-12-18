#include <stdlib.h>
#include <stdio.h>

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

void enqueue_data(queue **head,int data)
{
	if(!(*head))
	{
		*head=create_queue_node();
		(*head)->data = data;
		return;
	}

	enqueue_data(&(*head)->link,data);
}

void dequeue_data(queue **head)
{
	if(*head)
	{
		queue *tmp = *head;
		printf("dequeue_data = %d\n",(*head)->data);
		*head = (*head)->link;
		free(tmp);
	}
	else
	{
		printf("Queue is empty!\n");
	}
}

void print_queue(queue *head)
{
	while(head)
	{
		printf("queue head = %d\n",head->data);
		head = head->link;
	}
}


int main(void)
{
	int i;
	int data[]={10, 20, 30, 40};
	queue *head = NULL;

	for(i=0; i<4; i++)
	{
		enqueue_data(&head,data[i]);
	}

	print_queue(head);

	for(i=0; i<5; i++)
	{
		dequeue_data(&head);
	}
	

	return 0;
}

