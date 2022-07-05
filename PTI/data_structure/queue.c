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
	// 재귀호출
	enqueue_data(&(*head)->link, data);
}

void dequeue_data(queue **head)
{
	if(*head)
	{
		queue* tmp = *head;
		printf("dequeue data = %d\n", (*head)->data);
		*head = (*head)->link;
		free(tmp);
	}
	else
	{
		printf("Queue is already empty!\n");
	}
}

void print_queue(queue *head)
{
	if(head)
	{
		printf("queue head = %d\n", head->data);
		head = head->link;
	}
}

void enqueue_data_idx(queue **head, int data, int idx)
{
	// 중간에 넣는 경우
	if(!idx)	// 맨앞은 0 이니까 index가 0이 되서 참이 된다. 
	{
		queue *tmp = create_queue_node();

		tmp->data = data;
		tmp->link = *head;

		*head = tmp;
		return;
	}

	// 예외 핸들링
	if(!(*head))
	{
		printf("작업이 불가 합니다.\n");
		return;

	}

	enqueue_data_idx(&(*head)->link, data, --idx);
}


// 재귀호출 없이 enqueue
// 함수를 호출 할 때 마다의 스택프레임 생성 절차가 사라져서 속도가 더 빠를 수 있다.

void nr_enqueue_data_idx(queue **head, int data, int idx)
{
	queue *loop = *head;
	queue *backup = NULL;

	//숏컷으로 예외처리 대신함
	//앞에서 미리 판단 한다는 의미임. && 연산자는 모두가 참 이여야 참 이므로
	//loop가 참일 지라도 index가 거짓이면 거짓
	while(loop && idx)
	{
		idx--;
		backup = loop;
		loop = loop->link;
	}

	if(!idx)
	{
		queue *tmp = create_queue_node();

		tmp->data = data;
		tmp->link = loop;


		// 제일 앞 일 경우 back이 null 이므로 이러한 if문 구성
		if(!backup)
		{
			*head = tmp;
		}
		else
		{
			backup->link = tmp;
		}
		return;
	}

	printf("처리가 불가능한 작업 입니다!\n");
}


void dequeue_data_idx(queue **head, int idx)
{
	if(!idx)	// 맨앞은 0 이니까 index가 0이 되서 참이 된다. 
	{
		queue *tmp = *head;

		//tmp->data = data;
		(*head) = (*head)->link;

		free(tmp);

		
		return;
	}

	// 예외 핸들링
	if(!(*head))
	{
		printf("작업이 불가 합니다.\n");
		return;

	}

	dequeue_data_idx(&(*head)->link, --idx);
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

	
// #if 0 는 주석 넣는 방법 중 하나

	for (i = 0; i < 5; i++)
	{
		dequeue_data(&head);
	}

	for (i = 0; i < 4; i++)
	{
		enqueue_data(&head, data[i]);
	}

	// 특정 인덱스에 값 넣기
	enqueue_data_idx(&head, 33, 2);



#if 0

	// 특정 인덱스 값 빼기
	dequeue_data_idx(&head, 0);

	print_queue(head);
#endif

	return 0;
}
