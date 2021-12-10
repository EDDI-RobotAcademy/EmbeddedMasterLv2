#include <stdio.h>

#define BUF_SIZ		5

typedef struct _ringQ {
	int buf[BUF_SIZ];
	int *head;
	int *tail;
} ringQ;

void ringQ_set(ringQ* ring_buf, int data)
{
	static int cnt = 0;

	if( (ring_buf->head == ring_buf->tail) && *(ring_buf->head) ) {
		printf("ring buffer is Full\n");	
		return;
	}

	*(ring_buf->head) = data;
	ring_buf->head += 1;
	cnt++;
	
	if(cnt == BUF_SIZ) {
		ring_buf->head = ring_buf->buf; //문법적인 확인 필요
		cnt = 0;
	}
}

int ringQ_get(ringQ* ring_buf)
{
	int data;
	static int cnt = 0;

	if( (ring_buf->head == ring_buf->tail) && !(*(ring_buf->tail)) ) {
		printf("ring buffer is Empty\n");
		return -1;
	}

	data = *(ring_buf->tail);
	ring_buf->tail += 1;
	cnt ++;
	
	if(cnt == BUF_SIZ) {
		ring_buf->tail = ring_buf->buf;
		cnt = 0;
	}
	
	return data;
}

int main(void)
{
	int i;
	int data[] = {10, 20, 30, 40, 50, 60, 70, 80, 90};
	ringQ ring_buf;

	ring_buf.head = ring_buf.buf;
	ring_buf.tail = ring_buf.buf;

	for(i = 0 ; i < BUF_SIZ; i++) {
		ring_buf.buf[i] = 0;
	}

	for(i = 0; i < 9; i++) {
		ringQ_set(&ring_buf, data[i]);
		printf("ringQ_data = %d\n",data[i]);
	}
	
	return 0;
}
