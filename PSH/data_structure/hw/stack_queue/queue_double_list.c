/* 
   For now
   1)   enqueue_data, dequeue_data (재귀방식)이중 연결리스트 구현완료
   2)	enquque_idx_data(비재귀 방식) 이중 연결 리스트 구현완료
   3)   dequeue_idx_data(비재귀 방식) 이중 연결 리스트 구현완료
   4)	print_queue Prev, next의 값도 표현하도록 구현완료
   5)   enqueue_idx_data(재귀 방식) 이중 연결 리스트 구현완료

   To do
   1)   dequeue_idx_data(재귀 방식) 이중 연결 리스트 구현
   2)   print 오르락 내리락 읽는거 구현
   3)   enqueue_data 의 dummby로 구현하는 방법 잘 모르겠음 
   4)   모두 구현 후, ref 코드와 비교
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DUMMY_NODE		0
#define NORMAL_NODE		1

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

#if DUMMY_NODE 
/* 
void enqueue_data(queue **node, int data)_방법1
	
	To do
	1)	dummy node를 활용하기, dummy node 및 create로 생성되는 노드의 next가 자기 자신을 가리키는 노드로 구현
	=>  위에서는 *node가 존재하지 않을 때 신규 노드를 생성 했지만 *node == (*node)->next 일 때 신규 노드를 지정하도록 구현

	Debug
    1)	최초 dummy는 한 번 사용되고 그 이후로는 main의 node가 dummy를 가리키지 않게 된다.  
    2)	create_queue 코드도 변경이 필요하며 맨 마지막 노드의 next가 null아 아닌 자기 자신인 단점이 있음
	3)	print문도 null이 아닐때까지 출력이 아니라 *node == (*node)->next 일 때 까지 출력으로 바꿔야 하는 단점 
	=>  print문 해결 못함
	
*/
void enqueue_data(queue **node, int data)
{
	queue *tmp;

	if(*node == (*node)->next)
	{
		tmp = create_queue();
		tmp->data = data;		
		tmp->next = tmp;
		tmp->prev = (*node);
		(*node)->next = tmp;
	}
	else
	{
		enqueue_data(&(*node)->next, data);
	}
}

#else
/*
void enqueue_data(queue **node, int data)_방법2

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
		(*node)->next->prev = tmp; //Prev 구현 전략
	}
}
#endif

/*
void enqueue_idx_data_nr(queue **node, int idx, int data)

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

void enqueue_idx_data_nr(queue **node, int idx, int data)
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
	queue *tmp = NULL;

	if(idx == 1)
	{
		tmp = create_queue();	
		tmp->next = *node;
		(*node)->prev = tmp;
		*node = tmp;
	}
	else if(idx <= 0) //idx == 1이 맨처음 노드라 가정하면 idx <= 0은 존재 x
	{
		printf("enqueue is error\n");
	}
	else
	{
		if(!(*node)){ //idx 가 queue의 노드 갯수를 넘으면 안됨
			printf("enqueue is error\n");
			return;
		}
		enqueue_idx_data(&(*node)->next,--idx, data);
		(*node)->next->prev = *node;
	}
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

/*
int dequeue_idx_data_nr(queue **node, int idx)

   To do
   1)  idx를 감소시켜 해당 위치를 찾는다. 
   2)  해당 위치 전 노드 주소를 기억하고 있는다.  
   3)  맨처음노트 삭제, 맨 마지막 노드 삭제, 중간 노드 삭제, 한 참 뒤 노드 삭제 등 경우의 수 생각
   4)  삭제되는 노드의 전 노드의 next와 후노드의 prev 반영
   5)  맨 처음 queue 삭제시 main node의 위치 변경 필요함을 인지
   6)  free 까먹지 말기

*/

int dequeue_idx_data_nr(queue **node, int idx)
{
	int data;
	queue *prev_node = NULL;
	queue *tmp = *node;

	//맨 처음 노드를 삭제하는 경우 main의 노드를 변경할 필요가 있음
	if(idx == 1)
		*node = (*node)->next; 
	
	while(--idx){
		if(!tmp){
			printf("enqueue is error\n");
			return -1;
		}
		prev_node = tmp;
		tmp = tmp->next;
	}

	data = tmp->data;
	printf("dequeue data = %d\n", data);
	if(prev_node)
		prev_node->next = tmp->next;
	if(tmp->next)
		tmp->next->prev = tmp->prev;
	free(tmp);
	
	return data;
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
/* dummy 노드  사용하면서 추가*/
#if DUMMY_NODE 
	while(node != (node->next))
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
#else 
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
#endif
}

int main(void)
{
#if DUMMY_NODE 
	queue *dummy = create_queue();
	dummy->next = dummy;
	queue *node = dummy;
#else
	queue *node = NULL;
#endif

	int data[] = {10, 20, 30, 40};

	for(int i=0; i<4; i++)
		enqueue_data(&node, data[i]);

	print_queue(node);	

#if 1
	//enqueue_idx_data(&node, 1, 50);
	//enqueue_idx_data(&node, 2, 50);
	//enqueue_idx_data(&node, 5, 50);
	enqueue_idx_data(&node, 7, 50);

	print_queue(node);	
#endif

//	dequeue_idx_data(&node, 1);
//	dequeue_idx_data(&node, 2);
//	dequeue_idx_data(&node, 4);

#if 0
	while(!queue_is_empty(node))
	{
		dequeue_data(&node);
		print_queue(node);	
	}
#endif

	//free(dummy);

	return 0;
}
