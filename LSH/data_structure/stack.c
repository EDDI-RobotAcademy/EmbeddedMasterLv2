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

void push_data(stack **top, int data)
{
	stack *tmp = *top;

	*top = create_stack_node();
	(*top)->data = data;
	(*top)->link = tmp;
}

void print_stack_data(stack *top)
{
	stack *tmp = top;

	while (tmp)
	{
		printf("data = %d\n", tmp->data);
		tmp = tmp->link;
	}
}

int main(void)
{
	stack *top = NULL;
	int data[] = { 10, 20, 30, 40, 50, 60, 70 };
	int i;

	for (i = 0; i < 7; i++)
	{
		push_data(&top, data[i]);
	}

	print_stack_data(top);

	return 0;
}
