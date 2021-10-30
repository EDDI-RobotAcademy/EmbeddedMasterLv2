#include <stdio.h>
#include <stdlib.h>

typedef struct _stack stack;
struct _stack
{
	int data;
	struct _stack *link;
};

stack *create_stack_node(void)
{
	stack *tmp;

	tmp = (stack *)malloc(sizeof(stack));
	tmp->link = NULL;

	return tmp;
}
/*
void push_data(stack **top, int data)
{
	if (!(*top))
	{
		*top = create_stack_node():
		(*top)->data = data;
		return;
	}
}
*/

void push_data(stack **top, int data)
{
	stack *tmp = *top;

	*top = create_stack_node();
	(*top)->data = data;
	(*top)->link = tmp;
}

int pop_data(stack **top)
{
	int data;
	stack *tmp;

	if(!(*top))
	{
		printf("Stack is Empty\n");
		return -1;
	}

	tmp = *top;

	data = tmp->data;
	*top = tmp->link;

	free(tmp);

	return data;
}

void push_stack_data(stack *top)
{
	stack *tmp = top;

	while (tmp)
	{
		printf("data = %d\n", tmp->data);
		tmp = tmp->link;
	}
	/*

	while(top)
	{
		printf("data = %d\n", top->data);
		top = top->link;
	}

	*/
}

int main(void)
{
	stack *top = NULL;
	int data[] = { 10, 20, 30, 40, 50, 60, 70 };
	int i;

	for (i = 0 ; i<7 ; i++)
	{
		push_data(&top, data[i]);
	}

	push_stack_data(top);
	printf("\n");
	
	for (i = 0 ; i < 8 ; i++)
	{
		printf("pop = %d\n", pop_data(&top));
		push_stack_data(top);
		printf("\n");
	}

	return 0;
}
