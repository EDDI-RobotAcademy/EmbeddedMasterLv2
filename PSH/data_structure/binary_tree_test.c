#include <stdio.h>
#include <stdlib.h>

typedef struct _tree 
{
	int data;
	struct _tree *left;
	struct _tree *right;
}tree;

tree* create_root_node(void)
{
	tree *tmp = (tree*)malloc(sizeof(tree));
	tmp->left = 0;
	tmp->right = 0;

	return tmp;
}

void nr_insert_tree_data(tree **root, int data)
{
	while(*root)
	{
		if((*root)->data > data)
		{
			root = &(*root)->left;
		}
		else if((*root)->data < data)
		{
			root = &(*root)->right;
		}
	}
	
	*root = create_root_node();
	(*root)->data = data;
}

void insert_tree_data(tree** root, int data)
{
	if((*root))
	{
		if((*root)->data > data)
		{
			insert_tree_data(&(*root)->left, data);
		}
		else if((*root)->data < data)
		{
			insert_tree_data(&(*root)->right, data);
		}
	}
	else
	{
		*root = create_root_node();
		(*root)->data = data;
	}
}
tree* find_tree_node(tree** root, int data)
{
	while(*root)
	{
		if((*root)->data == data)
		{
			return *root;
		}
		else if((*root)->data > data)
		{
			root = &(*root)->left;
		}
		else if((*root)->data < data)
		{
			root = &(*root)->right;
		}
	}
}

void delete_tree_node(tree **root, int data)
{

}

void print_tree(tree *root)
{
	if(root)
	{
		printf("tree data = %d\n", root->data);
		print_tree(root->left);
		print_tree(root->right);
	}
}

int main(void)
{
	tree *root = NULL;
	int data[] = { 34, 17, 55, 10, 13, 12, 53, 57, 54, 51, 52};

	for(int i = 0; i < 11; i++)
	{
		//nr_insert_tree_data(&root, data[i]);
		insert_tree_data(&root, data[i]);
	}
	print_tree(root);

	tree *tmp = find_tree_node(&root, 13);
	//tree **tmp = find_tree_node(&root, 13);
	printf("find data = %d\n", tmp->data);

	return 0;
}
