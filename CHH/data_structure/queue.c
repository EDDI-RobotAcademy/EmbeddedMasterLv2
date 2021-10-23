#include <stdlib.h>

#include <stdio.h>

 

typedef struct _queue queue;

struct _queue
{
	int data;
	struct _queue* link;
};

 

queue* create_queue_node(void)

{
	queue* tmp;

	tmp = (queue*)malloc(sizeof(queue));

	tmp->link = 0;

	return tmp;
}

 

void enqueue_data(queue** head, int data)

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

 
int dequeue_data(queue** head)
{

	queue* tmp;

	int data;

	if ((*head) == NULL)
	{
		printf("queue is empty !!\n");
		return -1;
	}

 
	tmp = (*head);

	data = (*head)->data;

	(*head) = (*head)->link;

	free(tmp);

	return data;
}

 


void enqueue_data_idx(queue** head, int data, int idx)
{
    int index = idx;
    queue* tmp;

    if (index == 0)
    {
        if ((*head) == NULL)
        {
            (*head) = create_queue_node();
            (*head)->data = data;
            return;
        }

        tmp = (*head);
        (*head) = create_queue_node();
        (*head)->data = data;
        (*head)->link = tmp;
        return;
    }

    enqueue_data_idx(&(*head)->link, data, --index);
}

void nr_enqueue_data_idx(queue** head, int idx)
{
    queue* loop = *head;
    queue* backup = NULL;

    while (loop && idx)
    {

    }
    if(!(*head))
    {
        printf("invalid index \n");
        return -1;
    }

    dequeue_data_idx(&(*head)->link,--idx);
}


int dequeue_data_idx(queue** head, int idx)
{
    int data;
    queue* tmp;

    if(!idx)
    {
        data = (*head)->data;
        tmp = (*head);
        (*head) = (*head)->link;
        free(tmp);
        return data;
    }

    if(!(*head))
    {
        printf("invalid index \n");
        return -1;
    }

    dequeue_data_idx(&(*head)->link,--idx);
}



 

void print_queue(queue* head)

{

	while (head)

	{

		printf("queue head = %d\n", head->data);

		head = head->link;

	}

	printf("\n");
}

 

int main(void)

{

	int i;

	queue* head = NULL;

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

        print_queue(head);


	// 특정 인덱스에 값 넣기
/*
	enqueue_data_idx(&head, 33, 3);

    print_queue(head);
*/

	// 특정 인덱스 값 빼기

	dequeue_data_idx(&head, 3);
 
	print_queue(head);


	return 0;

}
