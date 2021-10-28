#include <stdlib.h>
#include <stdio.h>

#define Guide_Code 1

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
	if (!(*root))
	{
		*root = create_tree_node();
		(*root)->data = data;
		return;
	}

	// 언제 왼쪽에 넣어야 하는지
	// 언제 오른쪽에 넣어야 하는지를 판정해야함
	// insert_tree_data(&(*root)->link, data);
	else
	{
		if((*root)->data < data)
		{
			insert_tree_data(&(*root)->right, data);
		}
		else if((*root)->data > data)
		{
			insert_tree_data(&(*root)->left, data);
		}
		else
		{
			return;
		}
	}
}

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

tree **find_tree_data(tree **root, int data)
{
#if Guide_Code
	while(*root)
	{
		if((*root)->data < data)
			root = &(*root)->right;
		else if((*root)->data > data)
			root = &(*root)->left;
		else
			return root;
	}
	return NULL;
#endif
}

void delete_tree_node(tree **root, int data)
{
#if Guide_Code
	tree *tmp; 
	root = find_tree_data(root, data);

	tmp = *root;

	printf("Deleted tree data = %d\n", (*root)->data);
	*root = (!(*root)->left) ? (*root)->right : (*root)->left;

	free(tmp);
#else
	if((*root)->data < data) //현재 노드의 데이터보다 찾고자하는 데이터가 작은 경우
	{
		delete_tree_node(&(*root)->right, data);
	}
	else if((*root)->data > data) //현재 노드의 데이터가 찾고자하는 데이터보다 큰 경우 
	{
		delete_tree_node(&(*root)->left, data);
	}
	else //현재 노드의 데이터와 찾고자하는 데이터가 같은경우
	{
		//노드 삭제
		tree *tmp = *root;
		printf("Deleted root data = %d\n", (*root)->data);
		//*root = (!(*root)->left) ? (*root)->right : (*root)->left;
		if(!(*root)->left)
			*root = (*root)->right;
		else if(!(*root)->right)
			*root = (*root)->left;
		else
		{
		}
		free(tmp);
	}
#endif
}

void print_tree(tree *root)
{
	if(root)
	{
		printf("tree root = %d\n", root->data);
		print_tree(root->left);
		print_tree(root->right);
	}
}

int main(void)
{
	int i;
	tree *root = NULL;
	tree **tmp = NULL;
	int data[] = { 34, 17, 55, 10, 13, 12, 53, 57 };

	for (i = 0; i < 8; i++)
	{
		insert_tree_data(&root, data[i]);
	}

	print_tree(root);

	printf("\nFind Tree Data\n");
	if(tmp = find_tree_data(&root, 13)) 
		printf("tmp->data = %d\n", (*tmp)->data);
	else if(tmp = find_tree_data(&root, 77))
		printf("tmp->data = %d\n", (*tmp)->data);
	else
		printf("데이터를 찾을 수 없습니다.\n");

	printf("\nDelete Tree Node\n");
	delete_tree_node(&root, 13);
	delete_tree_node(&root, 10);
	//delete_tree_node(&root, 55);

	print_tree(root);
	return 0;
}
