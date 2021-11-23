#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define Guide_Code 1
//#define old 0
//#define new 1

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
	top->parent = mid;
	if(mid->left)
		mid->left->parent = top;

	top->right = mid->left;
	mid->left = top;

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

	update_level(&top);
	update_level(&mid);
	update_level(&bot);
}

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

void LL_rotation(avl **root, avl **cursor)
{
	avl *top, *mid, *bot;
	avl *grand_parent = (*cursor)->parent;

	top = *cursor;
	mid = top->left;
	bot = mid->left;

	mid->parent = grand_parent;
	top->parent = mid;

	if(mid->right)
		mid->right->parent = top;

	top->left = mid->right;
	mid->right = top;

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

	update_level(&top);
	update_level(&mid);
	update_level(&bot);
}

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

	mid->right = bot->left;
	bot->left = mid;
	top->left = bot;

	update_level(&top);
	update_level(&mid);
	update_level(&bot);

	cursor = &bot->parent;

	LL_rotation(root, cursor);
}

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
			nr_run_rotation(factor, root, loop, data);
			printf("root = %d\n", (*root)->data);
			break;
		}
		loop = &(*loop)->parent;
	}
}

/*#if new
void adjust_balance(avl **root, int data)
{
	avl *loop = NULL;
	avl **backup;
	int factor = 0;

	backup = find_tree_data(root, data);
	loop = (*backup)->parent;

	while(loop)
	{
		printf("loop = %d\n", loop->data);
		factor = calc_balance_factor(backup);
		if(ABS(factor) > 1)
		{
			printf("unbalanced node = %d\n", loop->data);
			nr_run_rotation(factor, root, backup, data);
		}
		backup = &loop->parent;
		loop = *backup;
	}
}
#endif*/

void nr_insert_avl(avl **root, int data)
{
	avl **loop = root;
	avl *parent = NULL;

	while(*loop)
	{
		parent = *loop;
		if((*loop)->data > data)
			loop = &(*loop)->left;
		else if((*loop)->data < data)
			loop = &(*loop)->right;
		else
			break;
	}

	*loop = create_avl_node();
	(*loop)->data = data;
	(*loop)->parent = parent;
	(*loop)->level = 1;

	nr_update_level(root, data);
	adjust_balance(root, data);
	//print_avl(*root);
}

int main(void)
{
    avl *root = NULL;
    int i;

#if Guide_Code
    int data[1690];
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);

    for (i = 0; i < len; i++)
    {
		nr_insert_avl(&root, data[i]);
    }
#else
	//int data[] = {500, 50, 1000, 100, 25, 750, 1250, 75, 125, 37, 12, 625, 875, 1125, 1375, 6, 30, 40, 45};
	//int data[] = {72, 194, 173, 161, 133, 158, 200};
	//int data[] = {72, 194, 173};
	int data[] = {173, 133, 194, 72, 161, 158, 200};
	int len = sizeof(data)/sizeof(int);
	for(i = 0; i < len; i++)
	{
		nr_insert_avl(&root, data[i]);
	}
#endif

	print_avl(root);

    return 0;
}
