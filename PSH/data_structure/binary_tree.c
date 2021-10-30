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
	if (!(*root))
	{
		*root = create_tree_node();
		(*root)->data = data;
		return;
	}

	// 언제 왼쪽에 넣어야 하는지
	// 언제 오른쪽에 넣어야 하는지를 판정해야함
	if ((*root)->data > data)
	{
		insert_tree_data(&(*root)->left, data);
	}
	else if ((*root)->data < data)
	{
		insert_tree_data(&(*root)->right, data);
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

void print_tree(tree *root)
{
	if (root)
	{
		printf("tree root = %d\n", root->data);
		print_tree(root->left);
		print_tree(root->right);
	}
}

tree **find_tree_data(tree **root, int data)
{
	while (*root)
	{
		if ((*root)->data > data)
			root = &(*root)->left;
		else if ((*root)->data < data)
			root = &(*root)->right;
		else
			return root;
	}

	return NULL;
}

int proc_left_max(tree **root)
{
	root = &(*root)->left;
	while((*root)->right)
	{
		root = &(*root)->right;
	}
	int data = (*root)->data;
	tree *tmp = *root;
	*root = 0;
	free(tmp);
	return data;
}

int proc_right_min(tree **root)
{
	root = &(*root)->right;	
	while((*root)->left)
	{
		root = &(*root)->left;
	}
	int data = (*root)->data;
	tree *tmp = *root;
	*root = 0;
	free(tmp);
	return data;
}

void delete_tree_data(tree **root, int data)
{
	tree *tmp;
	root = find_tree_data(root, data);

	tmp = *root;

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
		int max = proc_left_max(root);
		(*root)->data = max;
		printf("max = %d\n", max);

//		int min = proc_right_min(root);
//		(*root)->data = min;
//		printf("min = %d\n", min);
	}

}

int main(void)
{
	int i;
	tree *root = NULL;
	tree **tmp = NULL;
	int data[] = { 34, 17, 55, 10, 13, 12, 53, 57, 54, 51, 52 };

	for (i = 0; i < 11; i++)
	{
		//insert_tree_data(&root, data[i]);
		nr_insert_tree_data(&root, data[i]);
	}

	print_tree(root);

	if (tmp = find_tree_data(&root, 13))
		printf("tmp->data = %d\n", (*tmp)->data);

	if (tmp = find_tree_data(&root, 77))
		printf("tmp->data = %d\n", (*tmp)->data);
	else
		printf("데이터를 찾을 수 없습니다!\n");

//	printf("12 삭제\n");
//	delete_tree_data(&root, 12);
//	print_tree(root);
//
//	printf("10 삭제\n");
//	delete_tree_data(&root, 10);
//	print_tree(root);

	printf("55 삭제\n");
	delete_tree_data(&root, 55);
	print_tree(root);

	return 0;
}
