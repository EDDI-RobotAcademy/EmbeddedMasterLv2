#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _avl avl;
struct _avl
{
	int data;
	struct _avl *left;
	struct _avl *right;
	struct _avl *parent;
	int level;
};

avl *create_avl_node(void)
{
	avl *tmp;

	tmp = (avl *)malloc(sizeof(avl));
	tmp->left = 0;
	tmp->right = 0;
	tmp->parent = 0;
	tmp->level = 1;

	return tmp;
}

void adjust_avl_level(avl **root)
{
	int right, left;

	while(*root)
	{
		if ((*root)->right)
			right = (*root)->right->level;
		else
			right = 0;

		if ((*root)->left)
			left = (*root)->left->level;
		else
			left = 0;

		if (right > left)
			(*root)->level = right + 1;
		else
			(*root)->level = left + 1;

		root = &(*root)->parent;
	}
}

void nr_insert_avl_data(avl **root, int data)
{
	avl *backup = NULL;

	while(*root)
	{
		backup = *root;
		if((*root)->data > data)
			root = &(*root)->left;
		else if((*root)->data < data)
			root = &(*root)->right;
	}

	*root = create_avl_node();
	(*root)->data = data;
	(*root)->parent = backup;

	adjust_avl_level(&(*root)->parent);
}

void print_tree(avl *root)
{
	if (root)
	{
		printf("data = %d,\tlevel = %d,\t", root->data, root->level);

		if(root->parent)
			printf("parent = %d\t", root->parent->data);
		else
			printf("parent = NULL\t");

		if(root->left)
			printf("left = %d,\t", root->left->data);
		else
			printf("left = NULL,\t");

		if(root->right)
			printf("right = %d\n", root->right->data);
		else
			printf("right = NULL\n");

		print_tree(root->left);
		print_tree(root->right);
	}
}


int main(void)
{
	int i;
	avl *root = NULL;
	int data[] = { 34, 17, 55, 10, 13, 12, 53, 57 };

	for (i = 0; i < 8; i++)
		nr_insert_avl_data(&root, data[i]);

	print_tree(root);

	return 0;
}
