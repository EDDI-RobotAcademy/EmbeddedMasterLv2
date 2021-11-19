#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct _avl avl;
struct _avl
{
	int data;
	struct _avl *left;
	struct _avl *right;
	int level;
};

avl *create_avl_node(void)
{
	avl *tmp;

	tmp = (avl *)malloc(sizeof(avl));
	tmp->left = 0;
	tmp->right = 0;
	tmp->level = 1;

	return tmp;
}

void adjust_avl_level(avl **root, int data, int cnt)
{
	while(*root)
	{
		if ((*root)->level < cnt)
			(*root)->level = cnt;

		cnt--;

		if((*root)->data > data)
			root = &(*root)->left;
		else if((*root)->data < data)
			root = &(*root)->right;
		else
			break;
	}
}

void nr_insert_avl_data(avl **root, int data)
{
	avl **tmp = root;
	int cnt = 1;

	while(*tmp)
	{
		cnt++;

		if((*tmp)->data > data)
			tmp = &(*tmp)->left;
		else if((*tmp)->data < data)
			tmp = &(*tmp)->right;
	}

	*tmp = create_avl_node();
	(*tmp)->data = data;

	adjust_avl_level(root, data, cnt);
}

void print_tree(avl *root)
{
	if (root)
	{
		printf("data = %d,\tlevel = %d,\t", root->data, root->level);

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
