#include <stdio.h>
#include <string.h>

#define BUF_SIZ		9	

typedef struct _ringQ {
	int buf[BUF_SIZ];
	int *head;
	int *tail;
} ringQ;

void ringQ_init(ringQ* ring_buf)
{
	ring_buf->head = ring_buf->buf;
	ring_buf->tail = ring_buf->buf;

	memset(ring_buf->buf, 0, BUF_SIZ*sizeof(int));
}

void ringQ_set(ringQ* ring_buf, int data)
{
	static int cnt = 0;

	if( (ring_buf->head == ring_buf->tail) && *(ring_buf->head) ) {
		printf("ring buffer is Full\n");	
		return;
	}

	*(ring_buf->head) = data;
	printf("setData = %d\n", *(ring_buf->head));

	ring_buf->head += 1;
	cnt++;
	
	if(cnt == BUF_SIZ) {
		ring_buf->head = ring_buf->buf; 
		cnt = 0;
	}
}

void ringQ_set_block(ringQ* ring_buf, int *data, int size)
{
	static int cnt = 0;

	while(size--)
	{
		if( (ring_buf->head == ring_buf->tail) && *(ring_buf->head) ) {
			printf("ring buffer is Full\n");	
			return;
		}

		*(ring_buf->head) = *data;
		printf("setData = %d\n", *(ring_buf->head));

		data += 1;
		ring_buf->head += 1;
		cnt++;
		
		if(cnt == BUF_SIZ) {
			ring_buf->head = ring_buf->buf; 
			cnt = 0;
		}
	}
}

void ringQ_get(ringQ* ring_buf)
{
	int data;
	static int cnt = 0;

	if( (ring_buf->head == ring_buf->tail) && !(*(ring_buf->tail)) ) {
		printf("ring buffer is Empty\n");
		return;
	}

	data = *(ring_buf->tail);
	printf("getData = %d\n", data);

	*(ring_buf->tail) = 0;
	ring_buf->tail += 1;
	cnt ++;
	
	if(cnt == BUF_SIZ) {
		ring_buf->tail = ring_buf->buf;
		cnt = 0;
	}
}

void ringQ_get_block(ringQ* ring_buf, int size)
{
	int data;
	static int cnt = 0;

	while(size--)
	{
		if( (ring_buf->head == ring_buf->tail) && !(*(ring_buf->tail)) ) {
			printf("ring buffer is Empty\n");
			return;
		}

		data = *(ring_buf->tail);
		printf("getData = %d\n", data);

		*(ring_buf->tail) = 0;
		ring_buf->tail += 1;
		cnt ++;
		
		if(cnt == BUF_SIZ) {
			ring_buf->tail = ring_buf->buf;
			cnt = 0;
		}
	}
}

int main(void)
{
	int i;
	int data[] = {10, 20, 30, 40, 
	              50, 60, 70, 80, 
				  90, 100, 110, 120};
	ringQ ring_buf;

	ringQ_init(&ring_buf);

#if 0
	for(i = 0; i < BUF_SIZ+1; i++) {
		ringQ_set(&ring_buf, data[i]);
	}

	for(i = 0; i < BUF_SIZ+1; i++) {
		ringQ_get(&ring_buf);
	}
#else
	ringQ_set_block(&ring_buf,data,sizeof(data));
	ringQ_get_block(&ring_buf,sizeof(data));
#endif

	
	return 0;
}
