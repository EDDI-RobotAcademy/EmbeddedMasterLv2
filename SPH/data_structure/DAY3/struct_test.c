#include <stdio.h>

typedef struct _struct stack;

struct _struct
{
	int data;
	stack *link1;
	stack *link2;
};

int main(void)
{
	stack *head = NULL;
	stack stack1;

	stack1.data = 10;
	stack1.link1 = NULL;
	stack1.link2 = NULL;

	head = &stack1;

	printf("stack1 address = %p\n", &stack1);
	printf("stack1.data address = %p\n", &stack1.data);
	printf("stack1.link1 address = %p\n", &stack1.link1);
	printf("stack1.link2 address = %p\n", &stack1.link2);

	return 0;
}

