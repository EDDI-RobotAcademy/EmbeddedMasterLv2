/*
 *	ring buffer -2
 *	ring buffer 의 head, tail을 포인터 주소가 아닌 index 방식으로 변경함
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUF_SIZ		10	

typedef struct _ringQ {
	int buf[BUF_SIZ];
	int head;
	int tail;
} ringQ;

void ringQ_init(ringQ* ringq)
{
	ringq->head = 0;
	ringq->tail = 0;

	memset(ringq->buf, 0, BUF_SIZ*sizeof(int));
}

bool ringQ_is_Full(ringQ* ringq)
{
	if( (ringq->head + 1)%BUF_SIZ == ringq->tail ) {
		return true;
	}
	return false;
}

bool ringQ_is_Empty(ringQ* ringq)
{
	if( ringq->head == ringq->tail ) {
		return true;
	}
	return false;
}

void ringQ_set(ringQ* ringq, int data)
{
	if( ringQ_is_Full(ringq) ) {
		printf("ring buffer is Full\n");	
		return;
	}

	ringq->head = (++ringq->head)%BUF_SIZ;
	ringq->buf[ringq->head] = data;
	printf("ringq_set[%d] = %d\n", ringq->head, ringq->buf[ringq->head]);
}

void ringQ_set_block(ringQ* ringq, int *data, int size)
{
	static int cnt = 0;

	while(size--)
	{
		if(ringQ_is_Full(ringq)) {
			printf("ring buffer is Full\n");	
			return;
		}
		ringQ_set(ringq, *data++);
	}
}

int ringQ_get(ringQ* ringq)
{
	int data;

	if( ringQ_is_Empty(ringq) ) {
		printf("ring buffer is Empty\n");	
		return -1;
	}

	ringq->tail = (++ringq->tail)%BUF_SIZ;
	data = ringq->buf[ringq->tail];
	printf("ringq_get[%d] = %d\n", ringq->tail, data);

	return data;
}

int ringQ_get_block(ringQ* ringq, int size)
{
	int data;

	while(size--)
	{
		if( ringQ_is_Empty(ringq) ) {
			printf("ring buffer is Empty\n");	
			return -1;
		}

		data = ringQ_get(ringq);
	}
	return data;	
}

int main(void)
{
	int i;
	int data[] = {10, 20, 30, 40, 
	              50, 60, 70, 80, 
				  90, 100, 110, 120};
	int rtnData;
	ringQ ringq;

	ringQ_init(&ringq);

#if 0
	for(i = 0; i < 5; i++) {
		ringQ_set(&ringq, data[i]);
	}

	for(i = 0; i < 3; i++) {
		rtnData = ringQ_get(&ringq);
	}

	for(i = 0; i < 9; i++) {
		ringQ_set(&ringq, data[i]);
	}

	for(i = 0; i < 10; i++) {
		rtnData = ringQ_get(&ringq);
	}
#else
	ringQ_set_block(&ringq,data,15);
	ringQ_get_block(&ringq,15);
#endif

	
	return 0;
}
