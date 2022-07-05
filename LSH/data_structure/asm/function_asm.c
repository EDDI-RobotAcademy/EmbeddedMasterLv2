#include <stdio.h>

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

// gcc를 활용해서 컴파일을 할 때 -g 옵션을 주면 디버깅 메타 정보를 기록하게 된다.
// 추가적으로 -O0 옵션을 추가하면 어떠한 최적화 기법도 적용되지 않는다.
// -O2는 최대한의 보수성을 지키며 최적화를 수행하며
// -O3는 문제가 발생하던 말던 일단 최고의 효율을 뽑겠다라는 옵션이라 문제가 발생할 우려가 있다.
