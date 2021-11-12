#include <stdbool.h>
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
	while (*root)
	{
		if ((*root)->data > data)
			root = &(*root)->left;
		else if ((*root)->data < data)
			root = &(*root)->right;
	}

	*root = create_tree_node();
	(*root)->data = data;
}

/*
 *
 */

stack *create_stack_node(void)
{
	stack *tmp;

	tmp = (stack *)malloc(sizeof(stack));
	tmp->link = NULL;

	return tmp;
}

// push(&top, data[i]);
void push(stack **top, tree *link)
{
	stack *tmp = *top;
	if (link)
	{
		*top = create_stack_node();
		(*top)->data = link;
		(*top)->link = tmp;
	}
}

// pop(&top)
tree *pop(stack **top)
{
	void *data;
	stack *tmp = *top;
	tree *tree = NULL;

	if (!(*top))
	{
		printf("Stack is empty\n");
		return -1;
	}

	printf("tmp->data : %d\n", tmp->data);
	tree = tmp->data;
	//free(tmp);

	return tree;
}

int stack_is_not_empty(stack **top)
{
	printf("(*top)->link) :%d", (*top)->link);
	//if (!((*top)->link))
	//	return -1;

	return top;
}

void nr_print_tree(tree **root)
{
	tree **tmp = root;
	stack *top = NULL;

	push(&top, *tmp);

	printf("&top : %d\n", &top);

	while (stack_is_not_empty(top))
	{
		//printf("*** %d\n", stack_is_not_empty(top));
		tree *t = (tree *)pop(&top);
		tmp = &t;

 		printf("data = %d, ", (*tmp)->data);

		if ((*tmp)->left)
			printf("left = %d, ", (*tmp)->left->data);
		else
			printf("left = NULL, ");

		if ((*tmp)->right)
			printf("right = %d\n", (*tmp)->right->data);
		else
			printf("right = NULL\n");


		push(&top, (*tmp)->right);
		push(&top, (*tmp)->left);
		
	}
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

tree *chg_node(tree *root)
{
	tree *tmp = root;

	if (!root->right)
		root = root->left;
	else if (!root->left)
		root = root->right;

	free(tmp);

	return root;
}

void find_max(tree **root, int *data)
{
	while (*root)
	{
		if ((*root)->right)
			root = &(*root)->right;
		else
		{
			*data = (*root)->data;
			*root = chg_node(*root);
			break;
		}
	}
}

void nr_delete_tree(tree **root, int data)
{
	int num;

	while (*root)
	{
		if ((*root)->data > data)
			root = &(*root)->left;
		else if ((*root)->data < data)
			root = &(*root)->right;
		else if ((*root)->left && (*root)->right)
		{
			find_max(&(*root)->left, &num);
			(*root)->data = num;
			return;
		}
		else
		{
			(*root) = chg_node(*root);
			return;
		}
	}

	printf("Not Found\n");
}

void delete_tree_data(tree **root, int data)
{
	int num;
	root = find_tree_data(root, data);

#if 0
	if (!(*root)->left)
	{
		*root = (*root)->right;
	}
	else if (!(*root)->right)
	{
		*root = (*root)->left;
	}
	else
	{
		//int max = proc_left_min(root);
		//int min = proc_right_min(root);
	}
	free(tmp);
#endif

	if ((*root)->left && (*root)->right)
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
	tree **tmp = NULL;
	int data[] = { 34, 17, 55, 10, 13, 12, 53, 57 };

	for (i = 0; i < 8; i++)
	{
		//insert_tree_data(&root, data[i]);
		nr_insert_tree_data(&root, data[i]);
	}

	//print_tree(root);
	nr_print_tree(&root);

	if (tmp = find_tree_data(&root, 13))
		printf("tmp->data = %d\n", (*tmp)->data);

	if (tmp = find_tree_data(&root, 77))
		printf("tmp->data = %d\n", (*tmp)->data);
	else
		printf("데이터를 찾을 수 없습니다!\n");

	printf("12 삭제\n");
	delete_tree_data(&root, 12);
	//nr_delete_tree(&root, 12);
	//print_tree(root);
	nr_print_tree(&root);

	printf("10 삭제\n");
	delete_tree_data(&root, 10);
	//nr_delete_tree(&root, 10);
	//print_tree(root);
	nr_print_tree(&root);

	nr_insert_tree_data(&root, 54);
	printf("55 삭제\n");
	delete_tree_data(&root, 55);
	//nr_delete_tree(&root, 55);
	//print_tree(root);
	nr_print_tree(&root);

	return 0;
}