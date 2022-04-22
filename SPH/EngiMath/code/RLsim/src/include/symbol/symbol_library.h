#ifndef __SYMBOL_LIB_H__
#define __SYMBOL_LIB_H__

#include "./symbol_library.h"
#include "../global_var.h"

/*
   @variables
   1. num : 리드의 번호
   2. vertex2D vertex : 리드의 2차원 좌표 값
   3. vertex2D *link : 리드의 연결된 와이어 좌표 값을 저장하는 포인터
                       (와이어 연결 기능 구현 후 연결 여부 판별하기 위해 필요함)
*/
typedef struct _symbol_lead symbol_lead;
struct _symbol_lead
{
	unsigned int num;
	vertex2D vertex;
	vertex2D *link;
};

/*
   @variables
   1. vertex2D *pVtx : 심볼형상의 2차원 좌표 값
   2. symbol_lead *lead : 심볼의 리드 좌표 값
*/
typedef struct _symbol2D symbol2D;
struct _symbol2D
{
	vertex2D *pVtx;
	symbol_lead *lead;
};

/*
   @variables
   1. num : 심볼의 번호
   2. sumbol2D symbol : 심볼형상의 좌표 값과 리드의 좌표 값
   3. symbol_queue *list : 다음 심볼을 가리키는 포인터
   4. boundary[2] : 심볼의 선택 영역으로 사각형 형상을 가지며, 2개의 좌표점을 이용한다
                    -----*
					|    | "*"이 영역을 구분하는 좌표점이 된다
					*-----
   5. create_symbol : 심볼의 종류별로 심볼을 생성하기 위한 함수 등록용 함수포인터
*/
typedef struct _symbol_queue symbol_queue;
struct _symbol_queue
{
	unsigned int num;
	symbol2D symbol;
	symbol_queue *list;
	vertex2D boundary[2];

	void (*create_symbol)(symbol2D *);
};

//심볼을 생성한다
symbol_queue *create_symbol(void);
//심볼의 리드선의 좌표를 생성한다
symbol_lead *create_lead(unsigned int num_lead);
//저항 심볼을 그리기 위한 좌표 값을 설정한다
void create_symbol_R(symbol2D *);
/*
   @parameter
   1. symbol_type : 저항인지 인덕터인지 판별하는 파라메터
   2. pSym : 심볼의 좌표값을 설정하는 함수포인터와 생성되는 위치가 저장된 구조체 포인터
      (1) void *create_symbol(symbol2D, vertex2D, int, int);
	  (2) basis
*/
void create_symbol_handler(char, symbol_queue *);
/*
   @parameter 
   1. symbol_type : 저항인지 인덕터인지 판별하는 파라메터
   (1) R : 저항
   2. symbol_queue : symbol구조체 포인터 변수
   3. num : symbol의 번호
*/
extern void enqueue_symbol(char, symbol_queue **, unsigned int);
//extern *find_picked_symbol(symbol_queue **, vertex2D);
extern symbol_queue *find_symbol(symbol_queue **, unsigned int);
#endif
