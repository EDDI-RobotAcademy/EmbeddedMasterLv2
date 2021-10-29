#include <stdio.h>
#include <stdlib.h>

typedef struct _stack stack;

struct _stack
{
	int data;
	stack *link;
};

stack *create_stack_node(stack *top, int data)
{
	stack *tmp = NULL;
	tmp = (stack *)malloc(sizeof(stack)); //malloc의 return값은 할당된 메모리의 주소값이다.
	tmp->data = data;
	tmp->link = top;
	return tmp;
}

void print_stack_data(stack *top)
{
	while(top)
	{
		printf("pushed data = %d\n", top->data);
		top = top->link;
	}
	printf("\n");
}

stack *push_data(stack *top, int data)
{
	stack *tmp = top;
	top = create_stack_node(tmp, data);
	return top;
}

stack *pop_data(stack *top)
{
	stack *tmp = top;
	int data;

	if(!tmp)
	{
		return NULL;
	}
	data = tmp->data;
	top = tmp->link;

	free(tmp);

	printf("Poped Data = %d\n", data);

	return top;
}

int main(void)
{
	int i;
	stack *top = NULL;
	int data[] = {10, 20, 30, 40, 50};

	for(i = 0; i < 5; i++)
	{
		top = push_data(top, data[i]);
	}

	print_stack_data(top);

	for(i = 0; i < 6; i++)
	{
		if(!top) printf("Stack is empty\n");
		top = pop_data(top);
		print_stack_data(top);
	}

	return 0;
}
