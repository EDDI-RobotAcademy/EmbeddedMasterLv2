#include <stdlib.h>
#include <stdio.h>

typedef struct _tree tree;
struct _tree
{
	int data;
	struct _tree *left;
	struct _tree *right;
};

typedef struct _stack stack;
struct _stack
{
	void *data;
	struct _stack *link;
};

tree *create_tree_node(void)
{
	tree *tmp;

	tmp = (tree *)malloc(sizeof(tree));
	tmp->left = 0;
	tmp->right = 0;

	return tmp;
}

void insert_tree_data(tree **root, int data)
{
	if (!(*root)) //주소 값을 받은 것 이므로, *root는 그 주소의 데이터 값을 의미 한다.
	{
		*root = create_tree_node();
		(*root)->data = data;
		return;
	}

	// 언제 왼쪽에 넣어야 하는지
	// 언제 오른쪽에 넣어야 하는지를 판정해야함
	//insert_tree_data(&(*root)->link, data);
	if((*root)->data > data)
	{
		insert_tree_data(&(*root)->left, data);
	}
	else if((*root)->data < data)
	{
		insert_tree_data(&(*root)->right, data);
	}	
}
 
// 재귀호출을 사용하지 않음(nr)

void nr_insert_tree_data(tree **root, int data)
{
	while(*root)
	{
		if((*root)->data > data)
			root = &(*root)->left;
		else if((*root)->data < data)
			root = &(*root)->right;
	}

	*root = create_tree_node();
	(*root)->data = data;
}

// 재귀 호출 하지 않는 print 가이드 코드, stack의 push와 pop을  void* 형으로 반환 받아야 한다.
// stack 을 통해서 복귀 주소를 push 하고 pop 으로 다시 돌아간다.
// push, pop은 구현해 보도록 할 것

/*
stack *create_stack_node(void)
{
	stack *tmp;

	tmp = (stack *)malloc(sizeof(stack));
	tmp->link = NULL;

	return tmp;
}

void push_data(stack **top, int data)
{
	stack *tmp = *top;

	*top = create_stack_node();
	(*top)->data = data;
	(*top)->link = tmp;
}

int pop_data(stack **top)
{
	int data;
	stack *tmp;

	if (!(*top))
	{
		printf("Stack is empty\n");
		return -1;
	}

	tmp = *top;

	data = tmp->data;
	*top = tmp->link;

	free(tmp);

	return data;
}

*/

void nr_print_tree(tree **root)
{
	tree **tmp = root;	// 실직적으로는 이것도 void* 타입으로 써야 의미가 통일 되긴 한다.
	stack *top = NULL;

	push(&top, *tmp);

	while(stack_is_not_empty(top))
	{
		tree *t = (tree *)pop(&top); // 여기서 (tree *) 형 변환을 해준 것.
		tmp = &t;

		printf("data = %d, ", (*tmp)->data);

		if((*tmp)->left)
			printf("left = %d, ", (*tmp)->left->data);
		else
			printf("left = NULL, ");

		if((*tmp)->right)
			printf("right = %d\n", (*tmp)->right->data);
		else
			printf("right = NULL\n");

		push(&top, (*tmp)->right);
		push(&top, (*tmp)->left);
	}
}



//아래는 재귀 호출 스타일 print
void print_tree(tree *root)
{
	if(root)
	{
		printf("tree root = %d\n", root->data);
		print_tree(root->left);
		print_tree(root->right);
	}
}

//find 함수이며 delete를 만들 때 find를 사용하면 훨씬 구현이 용이해 진다. insert랑 비슷.
//싱글 호출은 재귀호출 된것의 리턴 값을 받게끔 해야 한다.
//만약 return 타입을 그냥 root로 하려면 반환형이 tree**이 되면 된다. 아래 구현.
/*
tree *find_tree_data(tree **root, int data)
{
	while(*root)
	{
		if((*root)->data > data)
			root = &(*root)->left;
		else if((*root)->data < data)
			root = &(*root)->right;
		else
			return *root;
	}

	return NULL;
}*/

tree **find_tree_data(tree **root, int data)
{
	while(*root)
	{
		if((*root)->data > data)
			root = &(*root)->left;
		else if((*root)->data < data)
			root = &(*root)->right;
		else
			return root;
	}

	return NULL;
}

/* 1차 delete 함수 

void delete_tree_data(tree **root, int data)
{
	tree *tmp;
	root = find_tree_data(root, data);

	tmp = *root;


	// 아래 if, else if는 한쪽만 또는 하나도 자식이 없는 경우
	// 이것은 모두 자식이 있는 경우를 지울 때도 해야 하는 과정이기 때문에 함수화 하는게 좋다.
	if(!(*root)->left)
	{
		*root = (*root)->right;
	}
	else if(!(*root)->right)
	{
		*root = (*root)->left;
	}
	// 양쪽 자식이 모두 존재 할 때
	// 여기서 root는 삭제 할 대상을 찾은 것임.
	else
	{
		//int max = proc_left_max(root);
		//int min = proc_right_min(root);
	}

	free(tmp);
}
*/
// 21.10.30 추가
// 양쪽 모두의 node가 있는 tree 삭제 하기
// find로 max 값 찾고, node change 후 nr_delete

tree *chg_node(tree *root)
{
	tree *tmp = root;

	if(!root->right)
		root = root->left;
	else if(!root->left)
		root = root->right;

	free(tmp);

	return root;
}

void find_max(tree **root, int *data)
{
	tree **tmp = root; // 이건 없어도 됨

	while(*tmp)
	{
		if((*tmp)->right)
			tmp = &(*tmp)->right;

		else
		{
			*data = (*tmp)->data;
			*tmp = chg_node(*tmp);
			break;
		}
	}
}

void nr_delete_tree(tree **root, int data)
{
	tree **tmp = root;
	int num;

	while(*tmp)
	{
		if((*tmp)->data > data)
			tmp = &(*tmp)->left;
		else if((*tmp)->data < data)
			tmp = &(*tmp)->right;
		else if((*tmp)->left && (*tmp)->right)
		{
			find_max(&(*tmp)->left, &num);
			(*tmp)->data = num;
			return;
		}
		else
		{
			(*tmp) = chg_node(*tmp);
			return;
		}
	}

	printf("Not Found\n");
}

//아래는 결국 delete 3가지 경우의 수를 모두 포함하는 통합형 delete 함수
void delete_tree_data(tree **root, int data)
{
	int num;
	root = find_tree_data(root, data);

#if 0    
아래 주석 부분은 위의 수정 과정 이었기에 남겨 둔 것임.

	if (!(*root)->left)
	{
		*root = (*root)->right;
		free(tmp);
	}
	else if (!(*root)->right)
	{
		*root = (*root)->left;
		free(tmp);
	}
	else
	{
		//int max = proc_left_min(root);
		//int min = proc_right_min(root);
	}
	free(tmp);
#endif

	if((*root)->left && (*root)->right)
	{
		find_max(&(*root)->left, &num);
		(*root)->data = num;
	}
	else
	{
		(*root) = chg_node(*root);
	}
}



int main(void)
{
	int i;
	tree *root = NULL;
	tree **tmp = NULL; // find 함수 추가 하면서 추가됨
	int data[] = { 34, 17, 55, 10, 13, 12, 53, 57 };

	for (i = 0; i < 8; i++)
	{
		insert_tree_data(&root, data[i]);
	}

	print_tree(root);

	// 밑에 3줄은 find 코드, (if if else 문)
	if(tmp = find_tree_data(&root, 13))
		printf("tmp->data = %d\n", (*tmp)->data);
	
	if(tmp = find_tree_data(&root, 77))
		printf("tmp->data = %d\n", (*tmp)->data);
	else
		printf("데이터를 찾을 수 없습니다!\n");

	// 밑은 삭제에 대한 구현
	printf("12삭제\n");
	nr_delete_tree(&root, 12);
	print_tree(root);

	printf("10삭제\n");
	nr_delete_tree(&root, 10);
	print_tree(root);

        nr_insert_tree_data(&root, 54);
	printf("55 삭제\n");
	//delete_tree_data(&root, 55);
	nr_delete_tree(&root, 55);
	print_tree(root);
	

	return 0;
}
