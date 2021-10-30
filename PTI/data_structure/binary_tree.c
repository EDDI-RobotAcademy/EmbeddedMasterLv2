#include <stdlib.h>
#include <stdio.h>

typedef struct _tree tree;
struct _tree
{
	int data;
	struct _tree *left;
	struct _tree *right;
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
/*
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
*/

void print_tree(tree *root)
{
	if(root)
	{
		printf("tree root = %d\n", root->data);
		print_tree(root->left);
		print_tree(root->right);
	}
}

//find 함수, delete를 만들 때 find를 사용하면 훨씬 구현이 용이해 진다. insert랑 비슷.
//싱글 호출은 재귀호출 된것의 리턴 값을 받게끔 해야 한다.
//만약 return 타입을 그냥 root로 하려면 반환형이 tree**이 되면 된다. 아래 구현.
/*tree *find_tree_data(tree **root, int data)
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
		int max = proc_left_max(root);
		//int min = proc_right_min(root);
	}


	free(tmp);
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
	delete_tree_data(&root, 12);
	print_tree(root);

	printf("10삭제\n");
	delete_tree_data(&root, 10);
	print_tree(root);

	return 0;
}
