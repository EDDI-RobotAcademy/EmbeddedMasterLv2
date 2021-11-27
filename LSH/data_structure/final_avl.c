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
        printf("%4d,", data[i]);

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

#if 0
	// 1
	parent->parent = grand_parent->parent;
#endif

	if (!relative_root_parent)
		*root = parent;
	else if (relative_root_parent->left == grand_parent)
		relative_root_parent->left = parent;
	else
		relative_root_parent->right = parent;

#if 1
	// 1
	parent->parent = grand_parent->parent;
#endif

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

#if 1
void rr_rotation(avl **root, avl **term_node)
{
	avl *grand_parent = *term_node;
	avl *parent = (*term_node)->right;
	avl *child = parent->right;

	avl *relative_root_parent = (*term_node)->parent;

	//printf("root = %d, term_node = %d\n", (*root)->data, (*term_node)->data);

#if 0
	// 1
	parent->parent = grand_parent->parent;
#endif

	if (!relative_root_parent)
		*root = parent;
	else if (relative_root_parent->left == grand_parent)
		relative_root_parent->left = parent;
	else
		relative_root_parent->right = parent;

#if 1
	// 1
	parent->parent = grand_parent->parent;
#endif

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
#endif

#if 0
void rr_rotation(avl **root, avl **term_node)
{
	avl **grand_parent = term_node;
	avl **parent = &(*term_node)->right;
	avl **child = &(*parent)->right;

	avl *relative_root_parent = (*term_node)->parent;

	if (!relative_root_parent)
		*root = *parent;
	else if (relative_root_parent->left == *grand_parent)
		relative_root_parent->left = *parent;
	else
		relative_root_parent->right = *parent;

	// 1
	(*parent)->parent = (*grand_parent)->parent;

	// 2
	(*grand_parent)->parent = *parent;

	// 3
	if ((*parent)->left)
		(*parent)->left->parent = *grand_parent;

	// 4
	(*grand_parent)->right = (*parent)->left;

	// 5
	(*parent)->left = *grand_parent;

	level_update(grand_parent);
	level_update(parent);
	level_update(child);
}
#endif

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
	child->left = grand_parent;

	// 10
	child->right = parent;

	level_update(&grand_parent);
	level_update(&parent);
	level_update(&child);
}

int chk_level(avl **root)
{
	int right = 0, left = 0;

	if ((*root)->right)
		right = (*root)->right->level;
	//else if ((*root)->left)
	if ((*root)->left)
		left = (*root)->left->level;

	return right - left;
}

//void rotation(avl **parent, vl **root, int method)
void rotation(avl **real_root, avl **term_node, int method)
{
	switch(method)
	{
		case LL:
			printf("LL Rotation\n");
			ll_rotation(real_root, term_node);
			break;

		case LR:
			printf("LR Rotation\n");
			lr_rotation(real_root, term_node);
			break;

		case RR:
			printf("RR Rotation\n");
			rr_rotation(real_root, term_node);
			break;

		case RL:
			printf("RL Rotation\n");
			rl_rotation(real_root, term_node);
			break;
	}
}

int decision_rotation(avl **root, int gap, int data)
{
#if 0
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
#endif
	if (gap <= LL)
	{
		//if ((chk_level(&(*root)->left) > -1) && (*root)->left->data < data)
		//if ((chk_level(&(*root)->left) >= -1) && (*root)->left->data < data)
		//if ((chk_level(&(*root)->left) <= -1) && (*root)->left->data < data)
		//if (chk_level(&(*root)->left) <= 1)
		if (chk_level(&(*root)->left) >= 1)
			return LR;

		return LL;
	}
	else
	{
		//if ((chk_level(&(*root)->right) < 1) && (*root)->right->data > data)
		//if ((chk_level(&(*root)->right) <= 1) && (*root)->right->data > data)
		//if ((chk_level(&(*root)->right) >= 1) && (*root)->right->data > data)
		//if (chk_level(&(*root)->right) >= -1)
		if (chk_level(&(*root)->right) <= -1)
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
	//avl **backup;
	avl **backup = term_node;

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
			//rotation(root, term_node, decision_rotation(backup, right - left, data));
			//rotation(root, term_node, decision_rotation(term_node, right - left, data));
			rotation(root, term_node, decision_rotation(term_node, right - left, (*term_node)->data));
		else if (right > left)
			(*term_node)->level = right + 1;
		else
			(*term_node)->level = left + 1;

		backup = term_node;

#if 0
		if (*root == *term_node)
			break;
#endif

		// term_node가 0일 수도 있음
		if (*term_node)
			term_node = &(*term_node)->parent;
	}
}
#endif

void chk_balance(avl **real_root, avl **root)
{
	int left = 0, right = 0, gap;

	if (!(*root))
		return;

	if ((*root)->left)
		left = (*root)->left->level;
	if ((*root)->right)
		right = (*root)->right->level;

	gap = right - left;

	if (ABS(gap) > 1)
		rotation(real_root, root, decision_rotation(root, gap, (*root)->data));
}

void nr_insert_avl_data(avl **root, int data)
{
	avl *parent = NULL;
	avl **tmp = root;
	avl **real_root = root;

	while (*tmp)
	{
		parent = *tmp;

		if ((*tmp)->data > data)
			tmp = &(*tmp)->left;
		else if ((*tmp)->data < data)
			tmp = &(*tmp)->right;
	}

	*tmp = create_avl_node();
	(*tmp)->data = data;
	(*tmp)->parent = parent;

	while (*tmp)
	{
		level_update(tmp);
		chk_balance(real_root, &(*tmp)->parent);

		if (*tmp)
			tmp = &(*tmp)->parent;
	}

	//adjust_avl_level(root, &(*tmp)->parent, data);
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

		//usleep(50000);

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

	if(root->left)
	{
		root->left->parent = root->parent;
		root = root->left;
	}
	else if(root->right)
	{
		root->right->parent = root->parent;
		root = root->right;
	}
	else
	{
		root = NULL;
	}

	free(tmp);

	return root;
}

//void find_left_max(avl **real_root, avl **root)
void find_left_max(avl **root)
{
	//avl *backup = *root;
	avl **backup = root;
	avl **current = root;
	avl **parent = &(*current)->parent;

	if ((*current)->left)
		current = &(*current)->left;

	while (*current)
	{
		if ((*current)->right)
			current = &(*current)->right;
		else
		{
			parent = &(*current)->parent;
			(*backup)->data = (*current)->data;
			*current = chg_node(*current);
			break;
		}
	}

#if 0
	do
	{
		if (*parent)
		{
			level_update(parent);
			chk_balance(&(*parent)->parent, parent);
			parent = &(*parent)->parent;
		}
	}
	while(backup != *parent);
#endif

	while(*backup != *parent)
	{
		level_update(parent);
		chk_balance(&(*parent)->parent, parent);
		parent = &(*parent)->parent;
	}

	if (*parent)
	{
		level_update(parent);
		//chk_balance(&(*parent)->parent, parent);
		//chk_balance(real_root, parent);
		chk_balance(backup, parent);
		parent = &(*parent)->parent;
	}
}

#if 0
void find_max(avl **root, int *data)
{
#if 0
	avl **backup = root;
#else
	avl *backup = *root;
#endif
	avl **current = root;
	avl **parent = &(*root)->parent;

	while(*root)
	{
		if((*root)->right)
		{
			root = &(*root)->right;
		}
		else
		{
			current = &(*root)->parent;
			//*data = (*root)->data;
			*root = chg_node(*root);
			break;
		}
	}

#if 0
	while(*backup != *current)
	{
		level_update(current);
		//chk_balance(real_root, &(*tmp)->parent);
		//chk_balance(root, &(*current)->parent);
		chk_balance(&(*current)->parent, current);
		current = &(*current)->parent;
	}
#else
	while(backup != *current)
	{
		level_update(current);
		chk_balance(&(*current)->parent, current);
		current = &(*current)->parent;
	}
#endif
}
#endif

void nr_delete_avl_data(avl **root, int data)
{
	int num;
	avl **parent;
	avl **real_root = root;

	root = find_tree_data(root, data);
	if (!(*root))
		return;

	parent = &(*root)->parent;

	// 양쪽 노드
	if((*root)->left && (*root)->right)
	{
		//find_max(&(*root)->left, &num);
		//find_left_max(real_root, root);
		find_left_max(root);
		//(*root)->data = num;

#if 0
		if (*parent)
			adjust_avl_level(top, root, (*parent)->data);
#endif
	}
	else	// 단일 자식 및 홑 노드
	{
		(*root) = chg_node(*root);

		if (*parent)
		{
			if ((*parent)->left)
				level_update(&(*parent)->left);
			if ((*parent)->right)
				level_update(&(*parent)->right);
			level_update(parent);
		}

#if 0
		if (*parent)
			adjust_avl_level(top, parent, (*parent)->data);
#endif
	}

	while(*parent)
	{
		level_update(parent);
		//chk_balance(real_root, &(*parent)->parent);
		chk_balance(real_root, parent);

		if (*parent)
			parent = &(*parent)->parent;
	}
}

int main(void)
{
	int i;
	avl *root = NULL;

	// 이중 회전 발생 데이터 셋
	//int data[] = { 400, 300, 500, 200, 350, 450, 600, 100, 250, 325, 375, 425, 475, 650, 50, 150, 225, 275, 310, 340, 360, 390, 410, 210 };

	// 2^14 - 1 = 2^0 + 2^1 + ... + 2^13: pass
	//int data[16384] = { 6, 3, 8, 1, 5, 7, 9, 2, 4, 10 };
	//int del_data[16384] = { 3, 5, 1, 7 };

#if 0
	/* First Set */
	int data[16384] = {
		9, 5, 15, 3, 7, 12, 19, 2, 4, 6,
		8, 10, 14, 17, 20, 1, 11, 13, 16, 18
	};
	int del_data[16384] = { 12, 19, 6, 14, 20, 9 };
#endif

#if 0
	/* Second Set */
	int data[16384] = {
		10, 4, 13, 2, 6, 11, 17, 1, 3, 5,
		8, 12, 15, 19, 7, 9, 14, 16, 18, 20
	};
	int del_data[16384] = { 8, 13, 20, 6, 2, 16, 19, 14, 18, 12, 15, 7 };
#endif

#if 0
	/* Third Set */
	int data[16384] = {
		7, 3, 15,  1,  5, 11, 17, 2,  4,  6,
		9, 13, 16, 19, 8, 10, 12, 14, 18, 20
	};
	int del_data[16384] = {
		18, 16, 1, 6, 13, 20, 2, 3, 8
	};
#endif

#if 0
	/* Fourth Set */
	int data[16384] = {
		14, 6, 18, 4, 10, 16, 19, 2, 5, 8,
		12, 15, 17, 20, 1, 3, 7, 9, 11, 13
	};
	int del_data[16384] = {
		15, 5, 18, 16, 10, 9, 14, 7
	};
#endif

#if 0
	/* Fifth Set */
	int data[16384] = {
		11, 6, 17, 2, 8, 15, 19, 1, 4, 7,
		9, 13, 16, 18, 20, 3, 5, 10, 12, 14
	};
	int del_data[16384] = {
		1, 8
	};
#endif

#if 0
	/* Sixth Set */
	int data[16384] = {
		11, 7, 17, 3, 9, 14, 19, 2, 5, 8,
		10, 12, 16, 18, 20, 1, 4, 6, 13, 15
	};
	int del_data[16384] = {
		16, 18, 5, 14, 15
	};
#endif

#if 0
	/* 7th Set */
	int data[16384] = {
		9, 6, 15, 3, 7, 11, 17, 2, 4, 8,
		10, 13, 16, 19, 1, 5, 12, 14, 18, 20
	};
	int del_data[16384] = {
		16, 2, 10, 12, 13
	};
#endif

#if 0
	/* 8th Set */
	int data[16384] = {
		91, 13, 62, 14, 98, 29, 11, 3, 43, 81
	};
	int del_data[16384] = {
	};
#endif

#if 0
	/* 9th Set */
	int data[16384] = {
		88, 75, 6
	};
	int del_data[16384] = {
	};
#endif

#if 0
	/* 10th Set */
	int data[16384] = {
		11, 97, 12
	};
	int del_data[16384] = {
	};
#endif

#if 0
	/* 11th Set */
	int data[16384] = {
		13,  20,   6,  17,   8,  11,  10,   9,   3,  19,
   2,   1,  12,  15,   5,  14,  18,   4,  16,   7
	};
	int del_data[16384] = {
		19,  11,   9,  18,   2,  17,   3,   1,  16,   5,
   7,  14,  20,   8,   6,  15,   4,  10,  13,  12
	};
#endif

#if 0
	/* 11th Set */
	int data[16384] = {
		2,  19,   1,  13,  17,  16,  15,   5,   8,   4,
   7,  10,  18,   3,  20,  11,  14,   9,   6,  12
	};
	int del_data[16384] = {
		15,   9,  20,   2,   8,   6,  18,   5,  16,  17,
   7,  10,  11,   4,  14,  13,  12,  19,   3,   1
	};
#endif

#if 0
	/* 12th Set */
	int data[16384] = {
		11,  17,  19,   3,  13,  12,   6,   5,  10,   2,
  16,   4,  20,   1,   9,   7,  18,  14,   8,  15
	};
	int del_data[16384] = {
		12,   8,   9,  15,   7,  11,  19,   3,   6,   4,
  16,  17,   2,  20,  13,  14,  10,  18,   5,   1
	};
#endif

#if 1
	/* 13th Set */
	int data[16384] = {
		3,  40,  32,  22,  33,  93,  77,   1,  76,  21,
  58,  24,  92,   2,  85, 100,  81,  23,  46,  73,
  94,  57,  70,  35,  38,   4,  29,  87,  13,  61,
  79,  42,  55,  14,  30,  71,  88,  28,  89,  47,
  11,  97,  86,  43,  74,  60,  26,  66,  91,  56,
  96,  48,   9,  34,  37,  99,  52,  39,  72,  20,
  80,  10,  45,  75,  49,  41,  84,  50,  59,  19,
  63,  54,  82,  67,  51,  98,  44,  64,  16,  27,
  65,   7,   8,   6,  25,  95,  15,  17,  83,  68,
  90,  69,  36,   5,  18,  12,  31,  78,  53,  62
	};
	int del_data[16384] = {
		11,  95,  80,  69,  24,  75,   9,  29,  73,  51,
   1,  65,  63,  20,  64,  70,  54,  68,   3,  31,
   8,  23,  83,  81, 100,  97,  30,  87,   7,  91,
  50,  22,  18,  76,  48,  79,  57,  37,  71,  46,
  40,  62,  14,  92,  21,   2,  59,  33,  94,  77,
  93,  55,  35,  43,  10,  34,  49,  89,  32,  26,
  44,  56,  36,  16,  52,  58,  82,   4,  90,  61,
  13,  98,  27,  96,   5,  47,  72,  74,  19,  67,
  45,  38,  41,  42,  15,  99,  85,  86,  28,  78,
  39,  17,  25,  53,  12,   6,  60,  84,  88,  66
	};
#endif


	//int len = sizeof(data) / sizeof(int);
	int len = 10000;

	srand(time(NULL));

#if 1
	init_data(data, len);
	init_data(del_data, len);
	printf("입력: \n");
	print_arr(data, len);
	printf("삭제: \n");
	print_arr(del_data, len);
#endif

	for (i = 0; i < len; i++)
	{
		//printf("입력 데이터: %d\n", data[i]);
		nr_insert_avl_data(&root, data[i]);
		//print_tree(root);
		//printf("\n");
	}

	//printf("del_data[0] = %d\n", del_data[0]);
	print_tree(root);
	//sleep(1);
#if 1
	for (i = 0; i < len; i++)
	{
		//printf("지우는 값 = %d\n", del_data[i]);
		nr_delete_avl_data(&root, del_data[i]);
		//print_tree(root);
		//printf("\n");
	}
#else	
	//nr_delete_avl_data(&root, 650);
	//nr_delete_avl_data(&root, 11);
#endif

	print_tree(root);

	return 0;
}
