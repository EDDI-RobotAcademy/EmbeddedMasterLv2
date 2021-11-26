/*
   For now
   1)	stack 구조를 prev, next 이중 연결리스트 형태로 구현 완료
   2)	주어진 Print 출력을 왔다리 갔다리 카운트만큼 읽는것 구현 완료

   To do
   1)	print 출력 구현은 했지만 최적화 할수 있는 방법 피드백 받기?	
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PREV	0
#define NEXT	1

typedef struct _stack {
	int data;	
	struct _stack *prev;
	struct _stack *next;
} stack;

stack* create_stack(void)
{
	stack *tmp = (stack*)malloc(sizeof(stack));	
	tmp->prev = NULL;
	tmp->next = NULL;
	return tmp;
}

#if 1
void push_stack(stack **top, int data)
{
	stack *tmp = *top;

	*top = create_stack();
	if(tmp)
		tmp->next = *top;
	(*top)->data = data;
	(*top)->prev = tmp;
	(*top)->next = NULL;
}
#endif

#if 0
stack* push_stack(stack *top, int data)
{
	stack *tmp = top;

	top = create_stack();
	top->data = data;
	top->link = tmp;

	return top;
}
#endif

bool stack_is_empty(stack *top)
{
#if 1
	if(!(top)){
		printf("Stack is Empty!\n");
		return 1;
	}
	else{
		return 0;	
	}
#endif
#if 0
	return (top? 0:1);
#endif
}

int pop_stack(stack **top)
{
	if(*top)
	{
		stack *tmp = *top;
		int data = (*top)->data;
		*top = (*top)->prev;
		if(*top)
			(*top)->next = tmp;
		
		free(tmp);

		return data;
	}

	return -1;
}

/*
void print_stack_cnt(stack *top, int cnt)

 	To do
	*목표: cnt 30개면 앞으로 10번, 뒤로 9번 다시 앞으로 9번 뒤로 2번 읽어서
		  총 30번 읽도록 구현하는 것

	1)	cnt 갯수만큼만 반복해서 print 하도록 구현 -> O 
	2)	stack의 처음과 끝을 판별하여 반대 방향으로 읽도록 구현 -> O
	=>	dir flag 처리가 필요함
	3)	prev->data, next->data 같은 경우 prev,next가 존재하지 않으면 -> O
		printf("%d",top->prev_data) 로 읽을시 에러발생하니 주의 -> O
	4)	Queue 구조에서도 사용 가능하도록 구현 -> O
	5)	3,4 번 코드가 2번보다 뒤에 와서 처음 top이 prev, next로 대입되어버려서 읽지 못하는 문제 발생 -> o
	6)	print는 단순 출력이므로 main에 top을 바꾸는일이 없도록 단일포인터 사용이 바람직해 보임 -> o 
 */
void print_stack_cnt(stack *top, int cnt)
{
	bool dir; 
	//1
	while(cnt--)
	{
		//3,4 ; 2번과 합쳐서 뭔가 최적화 할 방법 고민 필요
		printf("Stack Data = %-4d ", top->data);
		if(top->prev)
			printf("Prev = %-4d ", top->prev->data);
		else
			printf("Prev = NULL ");
		if(top->next)
			printf("Next = %-4d ", top->next->data);
		else
			printf("Next = NULL ");

		//2
		if(top->prev && !(top->next))
			dir = (bool)PREV;
		else if(!(top->prev) && top->next)
			dir = (bool)NEXT;

		if(dir == (bool)PREV)
			top = top->prev;
		else
			top = top->next;


		printf("\n");

	}
}

void print_stack(stack *top)
{
	while(top)
	{
		printf("stack data = %d\n", top->data);
		top = top->prev;
	}
	
	printf("\n");
}

int main(void)
{
	stack *top = NULL;
	int data[] = {10, 20, 30, 40, 50, 60, 70};
	int data_cnt = sizeof(data)/sizeof(int);

	for(int i = 0; i <data_cnt; i++) {
		push_stack(&top, data[i]);
	}

	print_stack_cnt(top, 30);
//	print_stack_cnt(top, 4);

	printf("\n");

#if 1
	while(!stack_is_empty(top))
	{
		print_stack(top);
		pop_stack(&top);
	}
#endif
	return 0;
}
