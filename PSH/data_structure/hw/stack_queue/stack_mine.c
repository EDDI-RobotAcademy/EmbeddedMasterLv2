/*
	For now
	1)	수업시간의 기억이 남아있지만 처음부터 안보고 짜보기 완료 

	To do
	x
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _stack {
	int data;	
	struct _stack *link;
} stack;

stack* create_stack(void)
{
	stack *tmp = (stack*)malloc(sizeof(stack));	
	tmp->link = NULL;
	return tmp;
}

#if 1
void push_stack(stack **top, int data)
{
	stack *tmp = *top;

	*top = create_stack();
	(*top)->data = data;
	(*top)->link = tmp;
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
		*top = (*top)->link;
		
		free(tmp);

		return data;
	}

	return -1;
}

void print_stack(stack *top)
{
	while(top)
	{
		printf("stack data = %d\n", top->data);
		top = top->link;
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
		print_stack(top);
	}

	printf("\n");

	while(!stack_is_empty(top))
	{
		pop_stack(&top);
		print_stack(top);
	}

	return 0;
}
