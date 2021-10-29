#include <stdio.h>
#include <stdlib.h>

typedef struct _stack
{
	int data;
	struct _stack *link;
}stack;

stack* create_Stack_node()
{
	
}

void push_Stack(stack **top, int data)
{
	stack *tmp;

	tmp = *top;
	*top = (stack*)malloc(sizeof(stack));
	(*top)->data = data;
	(*top)->link = tmp;
}

int pop_Stack(stack **top)
{
	stack *tmp;
	int data;

	if(!(*top))
	{
		printf("Stack is Empty\n");
		return -1;
	}

	tmp = *top;

	data = tmp->data;
	*top = tmp->link;

	return data;
}

void print_Stack(stack *top)
{
	while(top){
		printf("%d\n", (top)->data);
		top = (top)->link;
	}
	printf("\n");
}

int main(void)
{
	int push_data[] = {10,20,30,40};
	int i;
	stack *top = NULL;
	
	for(i = 0; i < 4; i++){
		push_Stack(&top, push_data[i]);
	}
	print_Stack(top);

	for(i=0; i < 5; i++){
		printf("pop_data : %d\n", pop_Stack(&top));
		print_Stack(top);
	}

	return 0;
}
