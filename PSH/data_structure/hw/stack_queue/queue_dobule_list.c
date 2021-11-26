/* 
   For now
   1)   enqueue_data, dequeue_data 이중 연결리스트 구현완료
   2)	enquque_idx_data(비재귀 방식) 이중 연결 리스트 구현완료
   3)	print_queue Prev, next의 값도 표현하도록 구현  

   To do
   1)   dequeue_idx_data(비재귀 방식) 이중 연결 리스트 구현
   2)   enqueue_idx_data(재귀 방식) 이중 연결 리스트 구현
   3)   dequeue_idx_data(재귀 방식) 이중 연결 리스트 구현
   4)   print 오르락 내리락 읽는거 구현
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _queue {
	int data;
	struct _queue *prev;
	struct _queue *next;
} queue;

queue* create_queue(void)
{
	queue *tmp = (queue*)malloc(sizeof(queue));
	tmp->prev = NULL;
	tmp->next = NULL;
	return tmp;
}
/*
void enqueue_data(queue **node, int data)

	To do	
	1)  현재 queue의 prev의 값을 어떻게 쓸지 고민 필요

	Debug
	1)  node->next, node->prev와 같은 값을 넘기다보니 접근 못하는 곳이 있어서 해결못하고 끙끙함
    =>  수업시간에 배운 주소를 넘겨서 처리하는 방법의 필요성을 다시한번 깨닫게 됨 

*/

void enqueue_data(queue **node, int data)
{
	queue *tmp = *node;
	
	if(!(*node)) {
		*node = create_queue();
		(*node)->data = data;
	}
	else {
		enqueue_data(&(*node)->next, data);
		(*node)->next->prev = tmp;
	}
}

/*
void enqueue_idx_data(queue **node, int idx, int data)

   To do
   1)  queue 삽입 위치를 처음/중간/끝인 경우 코드가 문제 없도록 할 것
   2)  가급적 한 코드로 처음/중간/끝을 모두 커버하도록 구현해보기(방법의 차이)

   Debug
   1)  idx == 0 인 경우 맨 첫 위치에 queue를 삽입하므로 main의 node가 가리키는 위치 변경이 필요
   2)  하다보니 이중 포인터가 더 간단하다는 생각을 자연스럽게 하게 됨
       하지만 이전 노드의 값만 잘 저장해준다면 코드 구현에는 무리가 없다고 판단함
   3)  하다보니 node -> prev 와 같은 일반적인 참조에서 node가 null 인경우 문제가 발생하는데 놓치는 경우가 
	   종종 있다 보니 주의가 필요함
*/

void enqueue_idx_data(queue **node, int idx, int data)
{
	queue *tmp = *node;
	queue *prev_node = NULL;
	queue *next_node = *node;
	queue *target_node = NULL;

	while((--idx) > 0)
	{
		if(!tmp) {
			printf("enqueue is error\n");
			return;
		}

		prev_node = tmp;
		tmp = tmp->next;
		next_node = tmp;
	}

	//
	target_node = create_queue();
	target_node->data = data;
	target_node->next = next_node;
	target_node->prev = prev_node;

	//삽입한 노드 전/후 노드의 prev, next를 수정
	if(next_node) next_node->prev = target_node;
	if(prev_node) prev_node->next = target_node;

	//최초 idx = 0 으로 queue 맨 앞에 데이터를 넣는 경우main의 node 변경해야 하는 예외 처리 필요 
	if(idx == -1) *node = target_node; 
}

void dequeue_data(queue **node)
{
	if(*node){
		queue *tmp = *node;
		int data = (*node)->data;
		*node = (*node)->next;
		if(*node) (*node)->prev = NULL;
		free(tmp);
		printf("dequeue data = %d\n", data);
	}
}

bool queue_is_empty(queue *node)
{
	return ((!node)? 1 : 0);
}
/*
void print_queue(queue *node)

	To do
	1) node->prev , node->next에서 node의 데이터가 없을수 있음을 주의

	Debug
	1) 아래 방법이 stack에서도 그렇고 좋아보이지 않음 최적화 방법이 뭐가 있을까???
*/
void print_queue(queue *node)
{
	while(node)
	{
		printf("queue data = %d ", node->data);
		if(node->prev)
			printf("prev = %d ", node->prev->data);
		else
			printf("prev = NULL ");
		if(node->next)
			printf("next = %d\n", node->next->data);
		else
			printf("next = NULL\n");
		node = node->next;
	}
}

int main(void)
{
	queue *node; 
	int data[] = {10, 20, 30, 40};

	for(int i=0; i<4; i++)
		enqueue_data(&node, data[i]);

	print_queue(node);	

	//enqueue_idx_data(&node, 0, 50);
	//enqueue_idx_data(&node, 2, 50);
	//enqueue_idx_data(&node, 5, 50);
	enqueue_idx_data(&node, 7, 50);

	print_queue(node);	

#if 0
	while(!queue_is_empty(node))
	{
		dequeue_data(&node);
		print_queue(node);	
	}
#endif

	return 0;
}
