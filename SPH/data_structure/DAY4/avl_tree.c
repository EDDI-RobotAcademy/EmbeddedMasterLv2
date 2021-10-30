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

tree **find_child_node(tree **root)
{
	if (!(*root)->left)//우측에만 자식노드가 있는 경우
	{
		return &(*root)->right;
	}
	else if (!(*root)->right)//좌측에만 자식노드가 있는 경우
	{
		return &(*root)->left;
	}
	else //자식노드가 없는 경우 & 자식노드가 둘 다 있는 경우
	{
		return root;
	}
}

int proc_left_max(tree **root)
{
	int max;
	tree *tmp = (*root)->left; //삭제하려는 노드의 왼쪽 노드를 백업

	//삭제하려는 노드의 왼쪽노드에 오른쪽 값이 있다면 
	//오른쪽 노드에서 최대값을 찾는다.
	if(tmp->right) 
	{
		while(tmp)
		{
			max = tmp->data;
			tmp = tmp->right;
		}
	}
	else
	{
		max = tmp->data;
	}

	printf("max data = %d\n", max);

	return max;
}

int proc_right_min(tree **root)
{
	int min;
	tree *tmp = (*root)->right;

	if(tmp->left)
	{
		while(tmp)
		{
			min = tmp->data;
			tmp = tmp->left;
		}
	}
	else
	{
		min = tmp->data;
	}

	printf("min data = %d\n", min);

	return min;
}

int calc_left_node_height(tree **root)
{
	int left_height = 0;
	tree *tmp = (*root)->left;

	//노드의 자식노드가 1개라도 있는 경우 높이 카운트
	while(tmp)
	{
		left_height++;
		printf("data = %d\n", tmp->data);
		tmp = (tmp->left) ? tmp->left : tmp->right;
	}

	return left_height;
}

int calc_right_node_height(tree **root)
{
	int right_height = 0;
	tree *tmp = (*root)->right;

	//노드의 자식노드가 1개라도 있는 경우 높이 카운트
	while(tmp)
	{
		right_height++;
		printf("data = %d\n", tmp->data);
		tmp = (tmp->right) ? tmp->right : tmp -> left;
	}
	return right_height;
}

void balance_tree_node(tree **root)
{
	int left_height, right_height;
	tree *tmp = *root;

	left_height = calc_left_node_height(root);
	//right_height = calc_right_node_height(root);

	printf("left node height = %d\n", left_height);
	//printf("right node height = %d\n", right_height);
}

void delete_tree_node(tree **root, int data)
{
#if Guide_Code
	tree *tmp;
	root = find_tree_data(root, data);

	tmp = *root;

	if (!(*root)->left)
	{
		*root = *find_child_node(root);
	}
	else if (!(*root)->right)
	{
		*root = *find_child_node(root);
	}
	else
	{
		int max = proc_left_max(root);
		delete_tree_node(root, max);
		(*root)->data = max;

		/*int min = proc_right_min(root);
		delete_tree_node(root, min);
		(*root)->data = min;*/
		return;
	}
	free(tmp);
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
	int data[] = {20, 15, 3, 12, 5, 11, 6, 40, 25, 18};

	for (i = 0; i < 10; i++)
	{
		insert_tree_data(&root, data[i]);
	}

	print_tree(root);

	printf("\nCalculate node height\n");
	balance_tree_node(&root);

	return 0;
}
