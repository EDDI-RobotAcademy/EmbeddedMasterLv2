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

int pop_data(stack **top)
{
	stack *tmp;
	int data;

	if(!(*top))
	{
		printf("Stack is empty \n");
		return -1;
	}
	
	tmp = *top;

	data = (*top)->data;
	*top = (*top)->link;

	free(tmp);

	return data;
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
	int temp_data;

	printf("stack structure size : %d \n",sizeof(stack));
       	printf("stack structure pointer size : %d \n",sizeof(top));

	for (i = 0; i < 7; i++)
	{
		push_data(&top, data[i]);
	}

	print_stack_data(top);

	for (i = 0; i < 8;  i++)
	{
		temp_data = pop_data(&top);
		printf("pop data : %d \n",temp_data);
	}

	if(top != NULL)
	{
		printf("remained node \n");
	}

	return 0;
}
