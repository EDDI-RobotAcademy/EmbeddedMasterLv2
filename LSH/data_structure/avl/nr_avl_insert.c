#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct _avl avl;
struct _avl
{
	int data;
	struct _avl *left;
	struct _avl *right;
	struct _avl *parent;
	int level;
};

int loop_cnt = 0;

#define ABS(x)		((x > 0) ? (x) : -(x))

#define LL			-2
#define RR			2

#define LR			-3
#define RL			3

bool check_dup(int *data, int compare, int idx)
{
    int i;

    for (i = 0; i < idx; i++)
    {
        if (data[i] == compare)
        {
            return true;
        }
    }

    return false;
}

void init_data(int *data, int num)
{
	int i, tmp;

	for (i = 0; i < num; i++)
	{
reassign:
		tmp = rand() % num + 1;

		if (check_dup(data, tmp, i))
		{
			goto reassign;
		}

		data[i] = tmp;
	}
}

void print_arr(int *data, int num)
{
    int i;

    for (i = 0; i < num; i++)
    {
        printf("%4d", data[i]);

        if (i % 10 == 9)
        {
            printf("\n");
        }
    }

    printf("\n");
}

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
	avl *grand_parent = *term_node;
	avl *parent = (*term_node)->left;
	avl *child = parent->left;

	avl *relative_root_parent = (*term_node)->parent;

	if (!relative_root_parent)
		*root = parent;
	else if (relative_root_parent->left == grand_parent)
		relative_root_parent->left = parent;
	else
		relative_root_parent->right = parent;

	// 1
	parent->parent = grand_parent->parent;

	// 2
	grand_parent->parent = parent;

	// 3
	if (parent->right)
		parent->right->parent = grand_parent;

	// 4
	grand_parent->left = parent->right;

	// 5
	parent->right = grand_parent;

	level_update(&grand_parent);
	level_update(&parent);
	level_update(&child);
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
	avl *grand_parent = *term_node;
	avl *parent = (*term_node)->right;
	avl *child = parent->right;

	avl *relative_root_parent = (*term_node)->parent;

	if (!relative_root_parent)
		*root = parent;
	else if (relative_root_parent->left == grand_parent)
		relative_root_parent->left = parent;
	else
		relative_root_parent->right = parent;

	// 1
	parent->parent = grand_parent->parent;

	// 2
	grand_parent->parent = parent;

	// 3
	if (parent->left)
		parent->left->parent = grand_parent;

	// 4
	grand_parent->right = parent->left;

	// 5
	parent->left = grand_parent;

	level_update(&grand_parent);
	level_update(&parent);
	level_update(&child);
}

void rl_rotation(avl **root, avl **term_node)
{
	avl *grand_parent = *term_node;
	avl *parent = (*term_node)->right;
	avl *child = parent->left;

	avl *relative_root_parent = (*term_node)->parent;

	// 1
	child->parent = grand_parent->parent;

	// 2
	if (!relative_root_parent)
		*root = child;
	else if (relative_root_parent->left == grand_parent)
		relative_root_parent->left = child;
	else
		relative_root_parent->right = child;

	// 3
	grand_parent->parent = child;

	// 4
	parent->parent = child;

	// 5
	if (child->right)
		child->right->parent = parent;

	// 6
	parent->left = child->right;

	// 7
	if (child->left)
		child->left->parent = grand_parent;

	// 8
	grand_parent->right = child->left;

	// 9
	child->left = parent;

	// 10
	child->right = grand_parent;

	level_update(&grand_parent);
	level_update(&parent);
	level_update(&child);
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
		// term_node가 0일 수도 있음
		if (*term_node)
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
#if 0
		loop_cnt++;
		printf("cnt = %d,\tdata = %d,\tlevel = %d,\t", loop_cnt, root->data, root->level);
#else
		printf("data = %d,\tlevel = %d,\t", root->data, root->level);
#endif

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

avl **find_tree_data(avl **root, int data)
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

	printf("찾는 데이터가 없습니다: %d\n", data);
	return NULL;
}

avl *chg_node(avl *root)
{
	avl *tmp = root;

	if(!root->right)
		root = root->left;
	else if(!root->left)
		root = root->right;

	free(tmp);

	return root;
}

void find_max(avl **root, int *data)
{
	while(*root)
	{
		if((*root)->right)
			root = &(*root)->right;
		else
		{
			*data = (*root)->data;
			*root = chg_node(*root);
			break;
		}
	}
}

void nr_delete_avl_data(avl **root, int data)
{
	int num;
	//
	avl *backup;

	root = find_tree_data(root, data);
	backup = *root;

	if((*root)->left && (*root)->right)
	{
		find_max(&(*root)->left, &num);
		(*root)->data = num;
	}
	else
	{
		//
		//backup = root;
		(*root) = chg_node(*root);
	}

	//adjust_avl_level(root, &(*root)->parent, data);
	//adjust_avl_level(&backup, &backup->parent, data);
	//adjust_avl_level(&backup, &backup->parent, backup->data);
	adjust_avl_level(root, root, backup->data);
}

int main(void)
{
	int i;
	avl *root = NULL;

	// 이중 회전 발생 데이터 셋
	//int data[] = { 400, 300, 500, 200, 350, 450, 600, 100, 250, 325, 375, 425, 475, 650, 50, 150, 225, 275, 310, 340, 360, 390, 410, 210 };

	// 2^14 - 1 = 2^0 + 2^1 + ... + 2^13: pass
	int data[16384] = { 0 };
	//int data[333] = { 0 };
	int del_data[16384] = { 0 };

	//int len = sizeof(data) / sizeof(int);
	int len = 10000;

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);

	for (i = 0; i < len; i++)
		nr_insert_avl_data(&root, data[i]);

	//sleep(1);
#if 0
	for (i = 0; i < len; i++)
	{
		printf("지우는 값 = %d\n", del_data[i]);
		nr_delete_avl_data(&root, del_data[i]);
	}
	nr_delete_avl_data(&root, 650);
#endif

	print_tree(root);

	return 0;
}
