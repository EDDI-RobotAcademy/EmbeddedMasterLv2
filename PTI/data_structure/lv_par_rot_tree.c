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

#define ABS(x)		((x > 0) ? (x) : -(x))

#define LL			-2
#define RR			2

#define LR			-3
#define RL			3

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

void level_update(avl **root)
{
	int right, left;

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
}

void ll_rotation(avl **root, avl **term_node)
{
}

void lr_rotation(avl **root, avl **term_node)
{
#if 1
	avl *grand_parent = *term_node;
	avl *parent = (*term_node)->left;
	avl *child = parent->right;

	avl *relative_root_parent = (*term_node)->parent;

	// 1
	child->parent = grand_parent->parent;

	if (!relative_root_parent)
		*root = child;
	else if (relative_root_parent->left == grand_parent)
		relative_root_parent->left = child;
	else
		relative_root_parent->right = child;

	// 2
	grand_parent->parent = child;
	// 3
	parent->parent = child;

	// 4 & 8
	if (child->left)
		child->left->parent = parent;
	parent->right = child->left;

	// 5
	child->left = parent;

	// 6 & 9
	if (child->right)
		child->right->parent = grand_parent;
	grand_parent->left = child->right;

	// 7
	child->right = grand_parent;

	// level update
	level_update(&grand_parent);
	level_update(&parent);
	level_update(&child);
#else
	avl **grand_parent = root;
	avl **parent = &(*root)->left;
	avl **child = &(*parent)->right;
	avl *relative_root_parent = (*root)->parent;
	(*child)->parent = (*grand_parent)->parent;
	if (!relative_root_parent)
		;
		//*root = *child;
	else if (relative_root_parent->left == *grand_parent)
		relative_root_parent->left = *child;
	else
		relative_root_parent->right = *child;
	(*grand_parent)->parent = *child;
	(*parent)->parent = *child;
	// 4 & 8
	(*child)->left->parent = *parent;
	(*parent)->right = (*child)->left;
	// 5
	(*child)->left = *parent;
	// 6 & 9
	(*child)->right->parent = *grand_parent;
	(*grand_parent)->left = (*child)->right;
	// 7
	(*child)->right = *grand_parent;
#endif
}

void rr_rotation(avl **root, avl **term_node)
{
}

void rl_rotation(avl **root, avl **term_node)
{
}

//void rotation(avl **parent, vl **root, int method)
void rotation(avl **root, avl **term_node, int method)
{
	switch(method)
	{
		case LL:
			printf("LL Rotation\n");
			ll_rotation(root, term_node);
			break;

		case LR:
			printf("LR Rotation\n");
			lr_rotation(root, term_node);
			break;

		case RR:
			printf("RR Rotation\n");
			rr_rotation(root, term_node);
			break;

		case RL:
			printf("RL Rotation\n");
			rl_rotation(root, term_node);
			break;
	}
}

int decision_rotation(avl **root, int gap, int data)
{
	if (gap == LL)
	{
		if ((*root)->data < data)
			return LR;

		return LL;
	}
	else	// RR
	{
		if ((*root)->data > data)
			return RL;

		return RR;
	}
}

#if 0
void adjust_avl_level(avl **root, int data)
{
	int right, left, gap;
	avl **backup;

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

		gap = right - left;

		if (ABS(gap) > 1)
			rotation(root, decision_rotation(backup, right - left, data));

		if (right > left)
			(*root)->level = right + 1;
		else
			(*root)->level = left + 1;

		backup = root;
		root = &(*root)->parent;
	}
}
#else
void adjust_avl_level(avl **root, avl **term_node, int data)
{
	int right, left, gap;
	avl **backup;

	while(*term_node)
	{
		if ((*term_node)->right)
			right = (*term_node)->right->level;
		else
			right = 0;

		if ((*term_node)->left)
			left = (*term_node)->left->level;
		else
			left = 0;

		gap = right - left;

		if (ABS(gap) > 1)
			rotation(root, term_node, decision_rotation(backup, right - left, data));
		else if (right > left)
			(*term_node)->level = right + 1;
		else
			(*term_node)->level = left + 1;

		backup = term_node;
		term_node = &(*term_node)->parent;
	}
}
#endif

void nr_insert_avl_data(avl **root, int data)
{
	avl *backup = NULL;
#if 0
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

	//adjust_avl_level(&(*root)->parent, data);
	adjust_avl_level(root, data);
#else
	avl **tmp = root;

	while (*tmp)
	{
		//backup = *root;
		backup = *tmp;

		if ((*tmp)->data > data)
			tmp = &(*tmp)->left;
		else if ((*tmp)->data < data)
			tmp = &(*tmp)->right;
	}

	*tmp = create_avl_node();
	(*tmp)->data = data;
	(*tmp)->parent = backup;

	adjust_avl_level(root, &(*tmp)->parent, data);
#endif
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
	//int data[] = { 34, 17, 55, 10, 13, 12, 53, 57 };
	//int data[] = { 34, 17, 55, 10, 13 };
	
	// 제어노드가 root인 복잡한 LR
	//int data[] = { 50, 100, 25, 75, 125, 37, 12, 6, 30, 40, 45 };
	// 단순한 LR
	//int data[] = { 5, 3, 4 };
	// 제어노드가 root가 아닌 복잡한 LR
	int data[] = { 500, 50, 1000, 100, 25, 750, 1250, 75, 125, 37, 12, 625, 875, 1125, 1375, 6, 30, 40, 45 };
	int len = sizeof(data) / sizeof(int);

	for (i = 0; i < len; i++)
		nr_insert_avl_data(&root, data[i]);

	print_tree(root);

	return 0;
}
