#include "../include/linear_algebra/mat3.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// (float [3] *) A
void init_mat(float (*A)[3])
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			A[i][j] = rand() % 4;
}

void print_mat(float (*R)[3])
{
	int i, j;

	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
			printf("%10.4f", R[i][j]);
		printf("\n");
	}
	printf("\n");
}

void add_mat(float (*A)[3], float (*B)[3], float (*R)[3])
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			R[i][j] = A[i][j] + B[i][j];
}

void sub_mat(float (*A)[3], float (*B)[3], float (*R)[3])
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			R[i][j] = A[i][j] - B[i][j];
}

void scale_mat(float scale_factor, float (*A)[3], float (*R)[3])
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			R[i][j] = scale_factor * A[i][j];
}

#if 0
A[0][0]	A[0][1]	A[0][2]		B[0][0]	B[0][1]	B[0][2]
A[1][0]	A[1][1]	A[1][2]		B[1][0]	B[1][1]	B[1][2]
A[2][0]	A[2][1]	A[2][2]		B[2][0]	B[2][1]	B[2][2]

A[0][0]*B[0][0]+A[0][1]*B[1][0]+A[0][2]*B[2][0]		A[0][0]*B[0][1]+A[0][1]*B[1][1]+A[0][2]*B[2][1]		A[0][0]*B[0][2]+A[0][1]*B[1][2]+A[0][2]*B[2][2]
A[1][0]*B[0][0]+A[1][1]*B[1][0]+A[1][2]*B[2][0]		A[1][0]*B[0][1]+A[1][1]*B[1][1]+A[1][2]*B[2][1]		A[1][0]*B[0][2]+A[1][1]*B[1][2]+A[1][2]*B[2][2]
A[2][0]*B[0][0]+A[2][1]*B[1][0]+A[2][2]*B[2][0]		A[2][0]*B[0][1]+A[2][1]*B[1][1]+A[2][2]*B[2][1]		A[2][0]*B[0][2]+A[2][1]*B[1][2]+A[2][2]*B[2][2]
#endif

void mul_mat(float (*A)[3], float (*B)[3], float (*R)[3])
{
	// for 문 사용은 권장하지 않음
	// CPU Pipeline이 branch instruction이 발생하면 깨지게됨
	// 3단계 - Fetch -> Decode -> Exectuion
	// branch가 발생을 하면 기존에 Fetch, Decode단계에 있던 정보들이 소실됨
	// Fetch, Decode를 branch된 위치에서 새롭게 받아오게 되기 때문에
	// 코드가 짧은 경우엔 성능상의 이유로 하드코딩을 권장합니다.
	R[0][0] = A[0][0]*B[0][0]+A[0][1]*B[1][0]+A[0][2]*B[2][0];
	R[0][1] = A[0][0]*B[0][1]+A[0][1]*B[1][1]+A[0][2]*B[2][1];
	R[0][2] = A[0][0]*B[0][2]+A[0][1]*B[1][2]+A[0][2]*B[2][2];

	R[1][0] = A[1][0]*B[0][0]+A[1][1]*B[1][0]+A[1][2]*B[2][0];
	R[1][1] = A[1][0]*B[0][1]+A[1][1]*B[1][1]+A[1][2]*B[2][1];
	R[1][2] = A[1][0]*B[0][2]+A[1][1]*B[1][2]+A[1][2]*B[2][2];

	R[2][0] = A[2][0]*B[0][0]+A[2][1]*B[1][0]+A[2][2]*B[2][0];
	R[2][1] = A[2][0]*B[0][1]+A[2][1]*B[1][1]+A[2][2]*B[2][1];
	R[2][2] = A[2][0]*B[0][2]+A[2][1]*B[1][2]+A[2][2]*B[2][2];
}

#if 0
    a[0][0] 	a[0][1]		a[0][2]
    ax			ay			az
    bx			by			bz

    i(x축 벡터) * (ay * bz) - (az * by)
    j(y축 벡터) * (az * bx) - (ax * bz)
    k(z축 벡터) * (ax * by) - (ay * bx)
#endif

float det_mat(float (*A)[3])
{
	return A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1]) +
		   A[0][1] * (A[1][2] * A[2][0] - A[1][0] * A[2][2]) +
		   A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);
}

void trans_mat(float (*A)[3], float (*R)[3])
{
	R[0][0] = A[0][0];
	R[1][1] = A[1][1];
	R[2][2] = A[2][2];

	R[0][1] = A[1][0];
	R[1][0] = A[0][1];

	R[0][2] = A[2][0];
	R[2][0] = A[0][2];

	R[2][1] = A[1][2];
	R[1][2] = A[2][1];
}

#if 0
	[0][0]		[0][1]		[0][2]
	[1][0]		[1][1]		[1][2]
	[2][0]		[2][1]		[2][2]


	R[0][1] = A[1][2] * A[2][0] - A[1][0] * A[2][2];
	R[0][2] = A[1][0] * A[2][1] - A[1][1] * A[2][0];

	R[1][0] = A[0][2] * A[2][1] - A[0][1] * A[2][2];
	R[1][2] = A[0][1] * A[2][0] - A[0][0] * A[2][1];

	R[2][0] = A[0][1] * A[1][2] - A[0][2] * A[1][1];
	R[2][1] = A[0][2] * A[1][0] - A[0][0] * A[1][2];

	Transpose
	trans(A) =
	[0][0]	[0][1]	[0][2]
	[1][0]	[1][1]	[1][2]
	[2][0]	[2][1]	[2][2]
#endif

// 수반 행렬 계산시 전치되는 부분을 고려해야함
void adj_mat(float (*A)[3], float (*R)[3])
{
#if 1
	float Trans[3][3];
	//A의 전치행렬 구하기
	trans_mat(A, Trans);
	//수반행렬 계산하기
	R[0][0] = Trans[1][1]*Trans[2][2] - Trans[1][2]*Trans[2][1];
	R[0][1] = Trans[1][0]*Trans[2][2] - Trans[1][2]*Trans[2][0];
	R[0][2] = Trans[1][0]*Trans[2][1] - Trans[1][1]*Trans[2][0];

	R[1][0] = Trans[0][1]*Trans[2][2] - Trans[0][2]*Trans[2][1];
	R[1][1] = Trans[0][0]*Trans[2][2] - Trans[0][2]*Trans[2][0];
	R[1][2] = Trans[0][0]*Trans[2][1] - Trans[0][1]*Trans[2][0];

	R[2][0] = Trans[0][1]*Trans[1][2] - Trans[0][2]*Trans[1][1];
	R[2][1] = Trans[0][0]*Trans[1][2] - Trans[0][2]*Trans[1][0];
	R[2][2] = Trans[0][0]*Trans[1][1] - Trans[0][1]*Trans[1][0];
#endif

#if 0
	R[0][0] = A[1][1] * A[2][2] - A[1][2] * A[2][1];
	R[0][1] = A[0][2] * A[2][1] - A[0][1] * A[2][2];
	R[0][2] = A[0][1] * A[1][2] - A[0][2] * A[1][1];

	R[1][0] = A[1][2] * A[2][0] - A[1][0] * A[2][2];
	R[1][1] = A[0][0] * A[2][2] - A[0][2] * A[2][0];
	R[1][2] = A[0][2] * A[1][0] - A[0][0] * A[1][2];

	R[2][0] = A[1][0] * A[2][1] - A[1][1] * A[2][0];
	R[2][1] = A[0][1] * A[2][0] - A[0][0] * A[2][1];
	R[2][2] = A[0][0] * A[1][1] - A[0][1] * A[1][0];
#endif
}

bool inv_mat(float (*A)[3], float (*R)[3])
{
	float det;

	det = det_mat(A);

	if(det == 0.0)
		return false;

	adj_mat(A, R);
#ifdef __DEBUG__
	printf("Adjoint Matrix\n");
	print_mat(R);
#endif
	scale_mat(1.0 / det, R, R);

	return true;
}

void molding_mat(float (*A)[3], float *ans, int idx, float (*R)[3])
{
	int i, j;

#if 0
	00		01		02
	10		11		12
	20		21		22
#endif

	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
		{
			if(j == idx)
				continue;
			R[i][j] = A[i][j];
		}

		R[i][idx] = ans[i];
	}
}

void crammer_formula(float (*A)[3], float *ans, float *xyz)
{
	float detA, detX, detY, detZ;
	float R[3][3] = {};

	detA = det_mat(A);

	molding_mat(A, ans, 0, R);
#ifdef __DEBUG__
	print_mat(R);
#endif
	detX = det_mat(R);

	molding_mat(A, ans, 1, R);
#ifdef __DEBUG__
	print_mat(R);
#endif
	detY = det_mat(R);

	molding_mat(A, ans, 2, R);
#ifdef __DEBUG__
	print_mat(R);
#endif
	detZ = det_mat(R);

	xyz[0] = detX / detA;
	xyz[1] = detY / detA;
	xyz[2] = detZ / detA;
}

void print_vec3(float *vec)
{
	int i;

	for(i = 0; i < 3; i++)
		printf("%10.4f", vec[i]);

	printf("\n");
}

void create_3x4_mat(float (*A)[3], float *ans, float (*R)[4])
{
	int i, j;

	for(i = 0; i < 3; i++)
	{
		for(j = 0; j < 3; j++)
			R[i][j] = A[i][j];

		R[i][3] = ans[i];
	}
}

void print_3x4_mat(float (*R)[4])
{
	int i, j;

	for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 4; j++)
            printf("%10.4f", R[i][j]);
        printf("\n");
    }
    printf("\n");
}

void adjust_3x4_mat(float (*A)[4], int idx, float (*R)[4])
{
	int i, j;
	float div_factor;

	for(i = idx + 1; i < 3; i++)
	{
		//div_factor = -A[idx][idx] / A[idx + 1][idx];
		//div_factor = -A[idx + 1][idx] / A[idx][idx];
		//div_factor = -A[i][0] / A[idx][0];
		div_factor = -A[i][idx] / A[idx][idx];
		printf("div_factor = %f\n", div_factor);

		for(j = 0; j < 4; j++)
			R[i][j] = A[idx][j] * div_factor + A[i][j];
	}
}

void finalize(float (*R)[4], float *xyz)
{
	xyz[2] = R[2][3] / R[2][2];
	xyz[1] = (R[1][3] - R[1][2] * xyz[2]) / R[1][1];
	xyz[0] = (R[0][3] - R[0][2] * xyz[2] - R[0][1] * xyz[1]) / R[0][0];
}

void gauss_elimination(float (*A)[3], float *ans, float *xyz)
{
	float R[3][4] = {};

	create_3x4_mat(A, ans, R);
#if __DEBUG__
	print_3x4_mat(R);
#endif

	adjust_3x4_mat(R, 0, R);
#if __DEBUG__
	print_3x4_mat(R);
#endif

	adjust_3x4_mat(R, 1, R);
#if __DEBUG__
	print_3x4_mat(R);
#endif

	finalize(R, xyz);
}

void create_3x6_mat(float (*A)[3], float (*R)[6])
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
		{
			R[i][j] = A[i][j];

			if(i == j)
				R[i][j + 3] = 1;
			else
				R[i][j + 3] = 0;
		}
}

void print_3x6_mat(float (*R)[6])
{
	int i, j;

	for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 6; j++)
            printf("%10.4f", R[i][j]);
        printf("\n");
    }
    printf("\n");
}

void adjust_3x6_mat(float (*A)[6], int idx, float (*R)[6])
{
    int i, j;
    float div_factor, scale;
#if 1
	float tmp[3][6];

	//create_3x6_mat(tmp, A);
	for(i = 0; i < 3; i++)
		for(j = 0; j < 6; j++)
			tmp[i][j] = A[i][j];

	scale = tmp[idx][idx];

    for(i = 0; i < 3; i++)
    {
		if(i == idx)
			continue;
        div_factor = tmp[i][idx] / tmp[idx][idx];
        printf("div_factor = %f\n", div_factor);

		if(div_factor == 0.0)
			continue;

        for(j = 0; j < 6; j++)
            R[i][j] = tmp[i][j]-(tmp[idx][j] * div_factor);
    }
	
	/*
	//scale 곱하는건 elimination 후 finalize에서 해야함
	for(j = 0; j < 6; j++)
		R[idx][j] = tmp[idx][j] / scale;
	*/
#endif
/*
   아래 코드의 문제점 :
   main함수(gauss_elimination)에서 mid를 포인터로 받아서 계산을 하면서 
   원본 mid의 값이 변경됨.
*/
#if 0
	scale = A[idx][idx];

    for(i = 0; i < 3; i++)
    {
        //div_factor = -A[idx][idx] / A[idx + 1][idx];
        //div_factor = -A[idx + 1][idx] / A[idx][idx];
        //div_factor = -A[i][0] / A[idx][0];
		if(i == idx)
			continue;
        div_factor = A[i][idx] / A[idx][idx];
        printf("div_factor = %f\n", div_factor);

		if(div_factor == 0.0)
			continue;

        for(j = 0; j < 6; j++)
            R[i][j] = A[i][j]-(A[idx][j] * div_factor);
    }
	
	//scale 곱하는건 elimination 후 finalize에서 해야함
	for(j = 0; j < 6; j++)
		R[idx][j] = A[idx][j] / scale;
#endif
}

void gauss_elim_mat(float (*A)[3], float (*R)[3])
{
	float mid[3][6] = {};
	int i, j;

	create_3x6_mat(A, mid);
#if 1
	print_3x6_mat(mid);
#endif
	//1번째 열의 1행 1열을 제외한 성분을 0으로 만듬
	adjust_3x6_mat(mid, 0, mid);
#if 1
    print_3x6_mat(mid);
#endif
	//2번째 열의 2행 2열을 제외한 성분을 0으로 만듬
	adjust_3x6_mat(mid, 1, mid);
#if 1
    print_3x6_mat(mid);
#endif
	//3번째 열의 3행 3열을 제외한 성분을 0으로 만듬
	adjust_3x6_mat(mid, 2, mid);
#if 1
    print_3x6_mat(mid);
#endif

	// mid 뒷열 3개에 finalize 작업 추가가 필요함
	// 결과 행렬 = 우측 행렬/좌측 행렬의 대각 성분
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			R[i][j] = mid[i][j+3]/mid[i][i];
}
