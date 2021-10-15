# include <stdio.h>

int mult(int n1, int n2)
{
	return n1 * n2;
}

int main(void)
{
	int num = 3, num2 = 2;
	int res;

	res = mult(num, num2);
	printf("res = %d\n", res);

	return 0;
}
