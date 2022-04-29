#include "../include/symbol/symbol_library.h"
#include "../include/global_var.h"

#include <stdio.h>
#include <stdlib.h>

/*
   1. 다음과 같은 모양의 저항 심볼을 만든다
   -/\/\/\-
   2. 입력 받아야하는 변수는 시작점과 끝점, x,y축의 변화량(시작점과 끝점은 리드선을 기준으로 그린다)
   3. 첫 번째 대각선, 마지막 대각선의 y축 시작점은 y축 변화량의 절반지점
   4. 홀수번째 대각선의 끝점은 x축의 변화량의 절반 지점
   5. 짝수번째 대각선의 끝점은 x축, y축의 변화량 만큼
   6. 저항 심볼의 대각선 갯수는 6개 정점의 갯수는 7개가 된다
   7. 저항의 리드는 시작점과 끝점에 연결되며, x축 변화량과 절반의 y축 변화량을 가진다

   -> 테스트용으로 10,10지점을 기준으로 그린다
*/
symbol_lead *create_lead(unsigned int num_lead)
{
	symbol_lead *tmp;
	tmp = malloc(sizeof(symbol_lead)*num_lead);
	tmp->link = NULL;
	return tmp;
}

void create_symbol_R(symbol2D *R)
{
	int i;

	if(!(R->lead))
		R->lead = create_lead(2);

	//리드의 시작점을 설정한다
	R->lead[0].vertex.x = basis.x;
	R->lead[0].vertex.y = basis.y + (double)dy/2;
	R->lead[0].num = 1;
	//리드의 끝점을 설정한다
	R->lead[1].vertex.x = basis.x + (5*dx);
	R->lead[1].vertex.y = basis.y + (double)dy/2;
	R->lead[1].num = 2;

	R->pVtx = malloc(sizeof(vertex2D)*7);

	//심볼의 대각선 성분의 좌표를 얻는다
	for(i = 0; i < 7; i++)
	{
		//심볼 대각선의 시작점과 끝점
		if(i == 0)
		{
			R->pVtx[i].x = R->lead[0].vertex.x + dx; 
			R->pVtx[i].y = R->lead[0].vertex.y;
		}
		else if(i == 6)
		{
			R->pVtx[i].x = R->lead[1].vertex.x - dx;
			R->pVtx[i].y = R->lead[1].vertex.y;
		}
		//심볼 대각선의 -방향
		else if(i%2 == 0)
		{
			R->pVtx[i].x = R->pVtx[0].x + (double)dx*(double)i/2;
			R->pVtx[i].y = basis.y;
		}
		//심볼 대각선의 +방향
		else
		{
			R->pVtx[i].x = R->pVtx[0].x + i*(double)dx/2;
			R->pVtx[i].y = basis.y + dy;
		}
	}

#if 0
	//심볼 선택을 위한 사각형 영역의 왼쪽 모서리 좌표 값을 설정한다
	R->boundary[0].x = R->pVtx[0].x;
	R->boundary[0].y = R->pVtx[0].y-dy/2;
	//심볼 선택을 위한 사각형 영역의 오른쪽 모서리 좌표 값을 설정한다
	R->boundary[1].x = R->pVtx[6].x;
	R->boundary[1].y = R->pVtx[6].y+dy/2;
#endif
}

symbol_queue *set_symbol_boundary(symbol_queue **pVtx)
{
	//심볼 선택을 위한 사각형 영역의 왼쪽 모서리 좌표 값을 설정한다
	(*pVtx)->boundary[0].x = (*pVtx)->symbol.pVtx[0].x;
	(*pVtx)->boundary[0].y = (*pVtx)->symbol.pVtx[0].y-dy/2;
	//심볼 선택을 위한 사각형 영역의 오른쪽 모서리 좌표 값을 설정한다
	(*pVtx)->boundary[1].x = (*pVtx)->symbol.pVtx[6].x;
	(*pVtx)->boundary[1].y = (*pVtx)->symbol.pVtx[6].y+dy/2;
}

/*
   @parameter
   1. symbol_type : 저항인지 인덕터인지 판별하는 파라메터
   2. pSym : 심볼의 좌표값을 설정하는 함수포인터와 생성되는 위치가 저장된 구조체 포인터
      (1) void *create_symbol(symbol2D, vertex2D, int, int);
	  (2) basis
*/
void create_symbol_handler(char symbol_type, symbol_queue *pSym)
{
	switch(symbol_type)
	{
		case 'r':
		case 'R':
			pSym->create_symbol = create_symbol_R;
			break;
	}
}

/*
   -마우스로 심볼을 선택 했을 때 선택된 심볼을 찾은 후 해당 심볼을 리턴한다
   * 전달받은 심볼의 종류에 따라 정해진 범위내에 picked 좌표가 있으면 해당 심볼을 리턴한다
*/
#if 0
symbol *find_picked_symbol(symbol_queue **pSym, vertex2D picked)
{
	symbol_queue *tmp = *pSym;

	while(1)
	{
		if(picked.x >= tmp->boundary[0].x && picked.x <= tmp->boundary[1].x)
		{
			if(picked.y >= tmp->boundary[0].y && picked.y <= tmp->boundary[1].y)
				return tmp;
		}
		else 
			return NULL;

		if(tmp->list)
			tmp = tmp->list;
		else
			return NULL;
	}
}
#endif

/*
   -여러개의 심볼이 생성되었을 때 심볼을 그리기 위해 queue에서 심볼을 찾은 후 해당 심볼을 리턴한다
   @parameter
   (1) pSym : 심볼이 생성된 queue
   (2) num : 심볼의 갯수
   * 추가로 구현해야 될 사항
   num을 마우스 클릭 할 때마다 증가시키고 전달 받아 해당 심볼을 리턴한다
*/
symbol_queue *find_symbol(symbol_queue **pSym, unsigned int num)
{
	symbol_queue *tmp = *pSym;

	if(!tmp)
		return NULL;

	while(tmp->num != num)
	{
		tmp = tmp->list;
	}
	return tmp;
}

symbol_queue *create_symbol(void)
{
	symbol_queue *tmp;
	tmp = (symbol_queue *)malloc(sizeof(symbol_queue));
	tmp->list = NULL;
	return tmp;
}

/*
   @parameter 
   1. symbol_type : 저항인지 인덕터인지 판별하는 파라메터
   (1) R : 저항
   2. symbol_queue : symbol구조체 포인터 변수
   3. num : symbol의 번호
*/
void enqueue_symbol(char symbol_type, symbol_queue **pSym, unsigned int num)
{
	if(!(*pSym))
	{
		*pSym = create_symbol();
		(*pSym)->num = num;
		create_symbol_handler(symbol_type, *pSym);
		//(*pSym)->create_symbol(&(*pSym)->symbol, &basis, dx, dy);
		(*pSym)->create_symbol(&(*pSym)->symbol);
		return;
	}
	else
	{
		symbol_queue *backup;
		symbol_queue *new;

		while(backup->list)
			backup = backup->list;

		new = create_symbol();
		new->num = num;
		create_symbol_handler(symbol_type, new);
		//new->create_symbol(&new->symbol, &basis, dx, dy);
		new->create_symbol(&new->symbol);
		backup->list = new;
		return;
	}
}
