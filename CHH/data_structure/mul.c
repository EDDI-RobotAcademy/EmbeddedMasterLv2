#include <stdio.h>

int mult(int n1, int n2)
{
	return n1 * n2;
}

int main(void)
{
	int num1 = 3;
	int num2 = 2;
	int res;


	res = mult(num1, num2);
	printf("res = %d\n", res);

	return 0;
}
