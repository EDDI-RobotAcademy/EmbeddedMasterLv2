#include <stdlib.h>
#include <stdio.h>

typedef struct _dl_list dl_list;
struct _dl_list
{
	int data;
	struct _dl_list *prev;
	struct _dl_list *next;
};

dl_list *create_dl_list_node(void)
{
	dl_list *tmp;

	tmp = (dl_list *)malloc(sizeof(dl_list));
	tmp->prev = NULL;
	tmp->next = NULL;

	return tmp;
}

void enqueue_data(dl_list **head, int data)
{
	if (!(*head))
	{
		*head = create_dl_list_node();
		(*head)->data = data;
		return;
	}

	enqueue_data(&(*head)->next, data);

	(*head)->next->prev = *head;
}

void push_data(dl_list **top, int data)
{
	dl_list *tmp = *top;

	*top = create_dl_list_node();
	(*top)->data = data;
	(*top)->next = tmp;
	(*top)->next->prev = *top;
}

void dequeue_data(dl_list **head)
{
	if (*head)
	{
		dl_list *tmp = *head;
		printf("dequeue data = %d\n", (*head)->data);
		*head = (*head)->next;

		if (*head)
			(*head)->prev = NULL;

		free(tmp);
	}
	else
	{
		printf("Queue is empty!!!\n");
	}
}

void pop_data(dl_list **top)
{
	// 사실상 dequeue나 pop이나 동작 구조가 동일함
	// 실제 top/head에 배치되는 내용은 queue로 입력한 경우에 head의 후반부에 배치됨
	// 그러므로 먼저 들어온 초반부의 head 데이터가 빠져나감
	// stack의 경우엔 마지막에 배치한 것이 초반부에 배치됨
	// 결국 값을 빼는 파트는 동일하게 구성되므로 dequeue_data와 pop_data의 구조가 동일함
}

#if 0
void nr_enqueue_data_idx(dl_list **head, int data, int idx)
{
	dl_list *loop = *head;
	dl_list *backup = NULL;

	// 숏컷으로 예외처리 대신함
	while (loop && idx)
	{
		idx--;
		backup = loop;
		loop = loop->next;
	}

	if (!idx)
	{
		dl_list *tmp = create_dl_list_node();

		tmp->data = data;
		tmp->next = loop;

		if (!backup)
		{
			*head = tmp;
		}
		else
		{
			backup->next = tmp;
		}
		return;
	}

	printf("처리가 불가능한 작업입니다!!!\n");
}

void enqueue_data_idx(dl_list **head, int data, int idx)
{
	// 중간에 넣는 경우
	if (!idx)
	{
		dl_list *tmp = create_dl_list_node();

		tmp->data = data;
		tmp->next = *head;

		*head = tmp;
		return;
	}

	// 예외 핸들링
	if (!(*head))
	{
		printf("작업이 불가합니다!\n");
		return;
	}

	enqueue_data_idx(&(*head)->next, data, --idx);
}
#endif

void print_queue(dl_list *head)
{
	while (head)
	{
		if (head->prev)
			printf("prev = %d\t", head->prev->data);
		else
			printf("prev = NULL\t");

		printf("data = %d\t", head->data);

		if (head->next)
			printf("next = %d\n", head->next->data);
		else
			printf("next = NULL\n");

		head = head->next;
	}

	printf("\n");
}

int main(void)
{
	int i;
	dl_list *head = NULL;
	int data[] = { 10, 20, 30, 40 };
	int pdata[] = { 15, 25, 35, 45 };

	for (i = 0; i < 4; i++)
	{
		enqueue_data(&head, data[i]);
		push_data(&head, pdata[i]);
	}

	print_queue(head);

	for (i = 0; i < 5; i++)
	{
		dequeue_data(&head);
	}

	print_queue(head);

#if 1
	for (i = 0; i < 4; i++)
	{
		enqueue_data(&head, pdata[i]);
	}
#endif

	// 특정 인덱스에 값 넣기
	//enqueue_data_idx(&head, 33, 2); - pass
	//enqueue_data_idx(&head, 33, 4); - pass
	//enqueue_data_idx(&head, 33, 0); - pass
	//enqueue_data_idx(&head, 33, 5);
	//nr_enqueue_data_idx(&head, 33, 2); - pass
	//nr_enqueue_data_idx(&head, 33, 0); - pass
	//nr_enqueue_data_idx(&head, 33, 4); - pass
	//nr_enqueue_data_idx(&head, 33, 5);
	print_queue(head);

	for (i = 0; i < 3; i++)
	{
		//pop_data(&head);
		dequeue_data(&head);
	}

	print_queue(head);

#if 0
	// 특정 인덱스 값 빼기
	dequeue_data_idx(&head, 0);

	print_queue(head);
#endif

	return 0;
}
