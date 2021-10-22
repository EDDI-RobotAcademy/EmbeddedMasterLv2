#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define LEFT_LINK		1
#define RIGHT_LINK		0

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
	
	bool checkNode = (data < (*root)->data)? LEFT_LINK : RIGHT_LINK;

	switch (checkNode)
	{
		case LEFT_LINK:
			insert_tree_data(&(*root)->left, data);	
			break;

		case RIGHT_LINK:
			insert_tree_data(&(*root)->right, data);	
			break;		
	}

	// 언제 왼쪽에 넣어야 하는지
	// 언제 오른쪽에 넣어야 하는지를 판정해야함
	// insert_tree_data(&(*root)->link, data);
}

void print_tree(tree *root)
{
	while (root)
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
	int data[] = { 34, 17, 55, 10, 13, 12, 53, 57 };

	for (i = 0; i < 8; i++)
	{
		insert_tree_data(&root, data[i]);
	}

	print_tree(root);

	return 0;
}
