#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#define Guide_Code 1
//#define old 0
#define new 1

#define ABS(x) ((x > 0) ? (x) : -(x))

#define RR 1
#define RL 2
#define LL 3
#define LR 4

typedef struct _avl avl;
struct _avl
{
	int data;
	struct _avl *left;
	struct _avl *right;
	int level;
	struct _avl *parent;
};

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

        if (i % 7 == 6)
        {
            printf("\n");
        }
    }

    printf("\n");
}

void print_avl(avl *tree)
{
    avl *tmp = tree;

    if (tmp)
    {
        print_avl(tmp->left);

        printf("data = %4d\t", tmp->data);
		if(tmp->parent)
			printf("parent = %4d\t", tmp->parent->data);
		else
			printf("parent = None\t");

        if (tmp->left)
        {
            printf("left = %4d\t", tmp->left->data);
        }
        else
        {
            printf("left = NULL\t");
        }

        if (tmp->right)
        {
            printf("right = %4d\t", tmp->right->data);
        }
        else
        {
            printf("right = NULL\t");
        }

        printf("level = %2d\n", tmp->level);
        print_avl(tmp->right);
    }
}

avl *create_avl_node(void)
{
    avl *tmp = (avl *)malloc(sizeof(avl));

    if (!tmp)
    {
        printf("Failed malloc\n");
    }

    tmp->left = NULL;
    tmp->right = NULL;

    return tmp;
}

avl **find_tree_data(avl **root, int data)
{
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
}

void update_level(avl **root)
{
	//왼쪽, 오른쪽 자식노드가 둘 다 없는 경우
	if(!(*root)->left && !(*root)->right)
		(*root)->level = 1;
	//오른쪽 자식노드만 있는 경우
	else if(!(*root)->left && (*root)->right)
		(*root)->level = (*root)->right->level + 1;
	//왼쪽 자식노드만 있는 경우
	else if((*root)->left && !(*root)->right)
		(*root)->level = (*root)->left->level + 1;
	//둘 다 있거나 같은 경우
	else
		(*root)->level = ((*root)->left->level >= (*root)->right->level) ? (*root)->left->level+1 : (*root)->right->level+1;
}

void nr_update_level(avl **root, int data)
{
	avl **loop = NULL;

	loop = find_tree_data(root, data);

	while(loop)
	{
		update_level(loop);
		//parent가 NULL인 경우 예외처리
		if((*loop)->parent)
			loop = &(*loop)->parent;
		else
			loop = NULL;
	}
}

int calc_balance_factor(avl **root)
{
	int factor;
	int left_level;
	int right_level;

	if((*root)->left)
		left_level = (*root)->left->level;
	else
		left_level = 0;

	if((*root)->right)
		right_level = (*root)->right->level;
	else
		right_level = 0;

	factor = left_level - right_level;

	return factor;
}

int decide_rotation(int factor, avl **root, int data)
{
	printf("decide_rot\n");
	printf("root = %d\n", (*root)->data);
	printf("factor = %d\n", factor);
	printf("data = %d\n", data);
	if(factor < 0)
	{
		if((*root)->right->data < data)
			return RR;
		else
			return RL;
	}
	else
	{
		if((*root)->left->data > data)
			return LL;
		else
			return LR;
	}
}

void RR_rotation(avl **root, avl **cursor)
{
	avl *top, *mid, *bot;
	avl *grand_parent;

	top = *cursor;
	mid = top->right;
	bot = mid->right;

	grand_parent = (*cursor)->parent;

	mid->parent = grand_parent;

#if old
	if(!grand_parent)
	{
		*root = mid;
	}
	else
	{
		if(grand_parent->data > mid->data)
			grand_parent->left = mid;
		else
			grand_parent->right = mid;
		update_level(&grand_parent);
	}
#endif

#if new
	if(!grand_parent)
		*root = mid;
	else if(grand_parent->left == top)
		grand_parent->left = mid;
	else
		grand_parent->right = mid;
#endif

	top->parent = mid;
	if(mid->left)
		mid->left->parent = top;
	top->right = mid->left;

	mid->left = top;

	update_level(&top);
	update_level(&mid);
	update_level(&bot);

	if(grand_parent)
		update_level(&grand_parent);
}

#if old
void RL_rotation(avl **root, avl **cursor)
{
	avl *top, *mid, *bot;

	top = *cursor;
	mid = top->right;
	bot = mid->left;

	bot->parent = top;
	mid->parent = bot;

	if(bot->right)
		bot->right->parent = mid;

	mid->left = bot->right;

	bot->right = mid;
	top->right = bot;

	update_level(&top);
	update_level(&mid);
	update_level(&bot);

	cursor = &bot->parent;

	RR_rotation(root, cursor);
}
#endif

#if new
void RL_rotation(avl **root, avl **cursor)
{
	printf("cursor = %d\n", (*cursor)->data);
	avl *top, *mid, *bot, *grand_parent;

	top = *cursor;
	mid = top->right;
	bot = mid->left;

	grand_parent = (*cursor)->parent;

	bot->parent = grand_parent;

	if(!grand_parent)
		*root = bot;
	else if(grand_parent->left == top)
		grand_parent->left = bot;
	else
		grand_parent->right = bot;

	top->parent = bot;
	if(bot->left)
		bot->left->parent = top;
	top->right = bot->left;

	mid->parent = bot;
	if(bot->right)
		bot->right->parent = mid;
	mid->left = bot->right;

	bot->right = mid;
	bot->left = top;

	update_level(&top);
	update_level(&mid);
	update_level(&bot);

	if(grand_parent)
		update_level(&grand_parent);
}
#endif

void LL_rotation(avl **root, avl **cursor)
{
	avl *top, *mid, *bot;
	avl *grand_parent = (*cursor)->parent;

	top = *cursor;
	mid = top->left;
	bot = mid->left;

	mid->parent = grand_parent;

#if old
	if(!grand_parent)
	{
		*root = mid;
	}
	else
	{
		if(grand_parent->data < mid->data)
			grand_parent->right = mid;
		else
			grand_parent->left = mid;
		update_level(&grand_parent);
	}
#endif

#if new
	if(!grand_parent)
		*root = mid;
	else if(grand_parent->left == top)
		grand_parent->left = mid;
	else
		grand_parent->right = mid;
#endif

	top->parent = mid;
	if(mid->right)
		mid->right->parent = top;

	top->left = mid->right;

	mid->right = top;

	update_level(&top);
	update_level(&mid);
	update_level(&bot);

	if(grand_parent)
		update_level(&grand_parent);
}

#if old
void LR_rotation(avl **root, avl **cursor)
{
	avl *top, *mid, *bot;

	top = *cursor;
	mid = top->left;
	bot = mid->right;

	bot->parent = top;
	mid->parent = bot;

	if(bot->left)
		bot->left->parent = mid;

	mid->right = (bot->left) ? bot->left : NULL;
	//mid->right = bot->left;

	bot->left = mid;
	top->left = bot;

	update_level(&top);
	update_level(&mid);
	update_level(&bot);

	cursor = &bot->parent;

	LL_rotation(root, cursor);
}
#endif

#if new
void LR_rotation(avl **root, avl **cursor)
{
	printf("cursor = %d\n", (*cursor)->data);
	avl *top, *mid, *bot, *grand_parent;

	top = *cursor;
	assert(top != NULL);

	mid = top->left;
	assert(mid != NULL);

	bot = mid->right;
	assert(bot != NULL);


	grand_parent = (*cursor)->parent;
	//assert(grand_parent != NULL);


	bot->parent = grand_parent;

	if(!grand_parent)
		*root = bot;
	else if(grand_parent->left == top)
		grand_parent->left = bot;
	else
		grand_parent->right = bot;

	top->parent = bot;
	if(bot->right)
		bot->right->parent = top;
	top->left = bot->right;

	mid->parent = bot;
	if(bot->left)
		bot->left->parent = mid;
	mid->right = bot->left;

	bot->right = top;
	bot->left = mid;

	update_level(&top);
	update_level(&mid);
	update_level(&bot);

	if(grand_parent)
		update_level(&grand_parent);
}
#endif
void nr_run_rotation(int factor, avl **root, avl **cursor, int data)
{
	int dir_rot = decide_rotation(factor, cursor, data);

	switch(dir_rot)
	{
		case RR:
			printf("RR rotation\n");
			RR_rotation(root, cursor);
			break;

		case RL:
			printf("RL rotation\n");
			RL_rotation(root, cursor);
			break;

		case LL:
			printf("LL rotation\n");
			LL_rotation(root, cursor);
			break;

		case LR:
			printf("LR rotation\n");
			LR_rotation(root, cursor);
			break;
	}
}

#if old
void adjust_balance(avl **root, int data)
{
	avl **loop = NULL;
	int factor = 0;

	loop = find_tree_data(root, data);

	while(*loop)
	{
		printf("loop = %d\n", (*loop)->data);
		factor = calc_balance_factor(loop);
		if(ABS(factor) > 1)
		{
			printf("unbalanced node = %d\n", (*loop)->data);
			printf("root = %d\n", (*root)->data);
			nr_run_rotation(factor, root, loop, data);
			printf("after rotation root = %d\n", (*root)->data);
			//print_avl(*root);
			break;
		}
		loop = &(*loop)->parent;
	}
}
#endif

#if new
void adjust_balance(avl **root, int data)
{
	avl *loop = NULL;
	int factor = 0;

	loop = (*find_tree_data(root, data))->parent;

	while(loop)
	{
		printf("loop = %d\n", loop->data);
		factor = calc_balance_factor(&loop);
		if(ABS(factor) == 2)
		{
			printf("unbalanced node = %d\n", loop->data);
			printf("root = %d\n", (*root)->data);
			nr_run_rotation(factor, root, &loop, data);
			printf("after rotation root = %d\n", (*root)->data);
		}
		loop = loop->parent;
	}
}
#endif

void nr_insert_avl(avl **root, int data)
{
	avl **loop = root;
	avl *parent = NULL;

	while(*loop)
	{
		//이전 노드를 parent로 잡고 있어서 parent 연결이 끊긴 상황 발생
		parent = *loop;
		if((*loop)->data > data)
			loop = &(*loop)->left;
		else if((*loop)->data < data)
			loop = &(*loop)->right;
		else
			break;
		//parent = *loop;
	}

	*loop = create_avl_node();
	(*loop)->data = data;
	(*loop)->parent = parent;
	(*loop)->level = 1;

	nr_update_level(root, data);
	adjust_balance(root, data);
}

avl *chg_avl_node(avl **root)
{
	avl *tmp = *root;

	if((*root)->left)
	{
		(*root)->left->parent = (*root)->parent;
		(*root) = (*root)->left;
	}
	else if((*root)->right)
	{
		(*root)->right->parent = (*root)->parent;
		(*root) = (*root)->right;
	}
	else
		*root = NULL;

	free(tmp);

	return *root;
}

avl **find_max(avl **cursor)
{
	while(*cursor)
	{
		if((*cursor)->right)
			cursor = &(*cursor)->right;
		else
		{
			return cursor;
		}
	}
}

void nr_re_balance(avl **root, avl **cursor)
{
	//printf("nr re-balancing\n");
	int factor;
	int data;
	int left_lv, right_lv;
	avl **loop;
	avl *child;

	nr_update_level(cursor, (*cursor)->data);

	loop = cursor;

	while(*loop)
	{
		factor = calc_balance_factor(loop);

		printf("re-balance factor = %d\n", factor);

		if(ABS(factor) <= 1)
			break;
		else
		{
			printf("re-balancing node = %d\n", (*loop)->data);
			child = (factor > 0) ? (*loop)->left : (*loop)->right;

			left_lv = (child->left) ? child->left->level : 0;
			right_lv = (child->right) ? child->right->level : 0;

			data = (left_lv > right_lv) ? child->left->data : child->right->data;
			adjust_balance(root, data);
			loop = &(*loop)->parent;
		}
	}
}

void nr_avl_delete_node(avl **root, int data)
{
	avl **del_node;
	avl *parent;
	avl **left_max;
	avl *max_parent;
	int factor;

	del_node = find_tree_data(root, data);
	parent = (*del_node)->parent;

	if(del_node == NULL)
	{
		printf("tree has no data\n");
		return;
	}

	if((*del_node)->left && (*del_node)->right)
	{
		left_max = find_max(&(*del_node)->left);
		max_parent = (*left_max)->parent;
		(*del_node)->data = (*left_max)->data;

		*left_max = chg_avl_node(left_max);

		nr_re_balance(root, &max_parent);
	}
	else if(!(*del_node)->left && !(*del_node)->right)
	{
		if(!parent)
		{
			*root = chg_avl_node(del_node);
			return;
		}
		else
		{
			if(parent->data < data)
				parent->right = chg_avl_node(del_node);
			else 
				parent->left = chg_avl_node(del_node);

			*del_node = NULL;

			nr_re_balance(root, &parent);
		}
	}
	else
	{
		*del_node = chg_avl_node(del_node);

		if(!parent)
		{
			update_level(del_node);
			return;
		}

		nr_re_balance(root, &parent);
	}
}

int set_del_data_num(int len)
{
	int num;
	int i;

	reassin:
	num = rand() % len + 1;

	if(ABS(num) > len)
		goto reassin;
	else
		return num;
}

void get_rand_idx(int *arr, int len)
{
	int i, j;
	int idx;

	for(i = 0; i < len; i++)
	{
		reassign:
		idx = rand() % len;
		arr[i] = idx;

		for(j = 0; j < i; j++)
		{
			if(arr[i] == arr[j])
				goto reassign;
		}
	}
}

void random_del_node(avl **root, int *data, int len)
{
	int rand_idx = 0;
	int del_data_num = 0;
	int i;

	srand(time(NULL));

	del_data_num = set_del_data_num(len);
	printf("del_data_num = %d\n", del_data_num);

	int *idx = (int *)malloc(sizeof(int) * del_data_num);
	get_rand_idx(idx, del_data_num);

	for(i = 0; i < del_data_num; i++)
	{
		printf("delete data = %d\n", data[idx[i]]);

		nr_avl_delete_node(root, data[idx[i]]);

		//print_avl(*root);
	}
}

int main(void)
{
    avl *root = NULL;
    int i;

#if Guide_Code
    int data[500];
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);

    for (i = 0; i < len; i++)
    {
		nr_insert_avl(&root, data[i]);
    }
#else
	int data[] = {500, 50, 1000, 100, 25, 750, 1250, 75, 125, 37, 12, 625, 875, 1125, 1375, 6, 30, 40, 45};
	//int data[] = {87, 58, 50,  34,   6,  57,  41,  43,  84,  95,   9,  62,  28,  2, 78,  92,  52,   5,  55,  49,  86};
	//int data[] = {72, 194, 173, 161, 133, 158, 200};
	//int data[] = {173, 133, 194, 72, 161, 158, 200};
	//int data[] = {80, 45, 200, 40, 50, 176, 280, 32, 44, 49, 55, 150, 180, 277, 290, 31, 33, 43, 47, 54, 149, 179, 275, 285, 29};
	//int data[] = {55, 32, 60, 31, 40, 58, 65, 29};
	//int data[] = {55, 31, 29};
	int len = sizeof(data)/sizeof(int);
	for(i = 0; i < len; i++)
	{
		nr_insert_avl(&root, data[i]);
	}
#endif

	printf("insert clear!\n");
	print_avl(root);

	printf("\ndelete node\n");
	//nr_avl_delete_node(&root, 173);
	//nr_avl_delete_node(&root, 200);
	//nr_avl_delete_node(&root, 194);
	//nr_avl_delete_node(&root, 45);
	//nr_avl_delete_node(&root, 40);
	//print_avl(root);
	random_del_node(&root, data, len);
	printf("delete clear!\n");
	print_avl(root);

    return 0;
}
