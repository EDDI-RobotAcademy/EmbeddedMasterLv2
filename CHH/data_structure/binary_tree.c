#include <stdlib.h>
#include <stdio.h>

#define BINARY_TREE	0
#define STACK_SIZE	100

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


tree *find_tree_data_sp(tree *root, int data)
{
        while (*root)
        {
                if (root->data > data)
                        root = root->left;
                else if (root->data < data)
                        root = root->right;
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
		int max = proc_left_max(root);
		//int min = proc_right_min(root);
	}

	free(tmp);
}

void delete_tree_data_sp(tree *root, int data)
{
	tree *tmp;
	root = find_tree_data_sp(root, data);

	tmp = root;

}

void push_node(void* stack, void* node, int* top)
{
	if (top >= STACK_SIZE || (node == NULL))
	{
		return;
	}

	stack[*top] = node;

	*top++;

}

void* pop_node(void* stack, int* top)
{
	void* result = NULL;

	if ((*top) == 0)
	{
		return NULL;
	}

	result = stack[top];
	*top--;

	return result;

}

void print_tree_node(void **root, int tree_type)
{
	void **tree_root = NULL;
	void *tmp = NULL;
	void *stack[STACK_SIZE] = { NULL, };
	int top = 0;

	if (tree_type == BINARY_TREE)
	{
		tree_root = (tree**)root;
		
		push_node(stack, &tree_root, &top);

		while(top)
		{
			(tree*)tmp = (tree *)pop(stack, &top);
			
			printf("data : %d ", (tree*)tmp->data);

			push_node(stack, (tree*) tmp->right, &top)
			push_node(stack, (tree*) tmp->left, &top)
		}
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

	print_tree(root);

	if (tmp = find_tree_data(&root, 13))
		printf("tmp->data = %d\n", (*tmp)->data);

	if (tmp = find_tree_data(&root, 77))
		printf("tmp->data = %d\n", (*tmp)->data);
	else
		printf("데이터를 찾을 수 없습니다!\n");

	printf("12 삭제\n");
	delete_tree_data(&root, 12);
	print_tree(root);

	printf("10 삭제\n");
	delete_tree_data(&root, 10);
	print_tree(root);

	return 0;
}
