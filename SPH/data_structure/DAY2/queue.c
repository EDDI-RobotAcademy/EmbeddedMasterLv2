#include <stdlib.h>
#include <stdio.h>

//#define Guide_Code 1

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
	return tmp;
}

void enqueue_data(queue **head, int data)
{
#if Guide_Code
	if (!(*head))
	{
		*head = create_queue_node();
		(*head)->data = data;
		return;
	}
	// 요기서 뭘 해야할까요 ?
	enqueue_data(&(*head)->link, data);
#else
	queue *tmp = *head;
	(*head) = create_queue_node();
	(*head)->data = data;
	(*head)->link = (!tmp) ? NULL : tmp;
#endif
}

int dequeue_data(queue **head) //queue *head는 무한루프 발생 
{
	//이중포인터 사용안할시 무한루프 형성됨
	int data;
	queue *tmp = *head;

	if(!tmp->link)
	{
		data = tmp->data;
		(*head) = tmp->link; //head = head->link는 결국 지역변수에 값을 변경하는것으로 함수 종료시 메모리 해제가 되어 queue의 다음 주소값을 전달못함
		free(tmp);
		return data;
	}
	else
	{
		data = dequeue_data(&(*head)->link);
	}
}

void enqueue_data_idx(queue **head, int data, int idx)
{
}

void dequeue_data_idx(queue **head, int idx)
{

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
		enqueue_data(&head, data[i]);
	}
	print_queue(head);

	for (i = 0; i < 5; i++)
	{
		printf("Dequeue Data = %d\n", dequeue_data(&head));
		print_queue(head);
	}
	// 특정 인덱스에 값 넣기
	//enqueue_data_idx(&head, 33, 2);

	// 특정 인덱스 값 빼기
	//dequeue_data_idx(&head, 0);
#else
	printf("Start Enqueue!\n");
	for (i = 0; i < 4; i++)
	{
		enqueue_data(&head, data[i]);
	}

	print_queue(head);

	enqueue_data_idx(&head, 8, 2);

	printf("Start Dequeue!\n");
	while(head)
	{
		printf("Dequeue data = %d\n", dequeue_data(&head));
		print_queue(head);
	}
#endif
	return 0;
}
