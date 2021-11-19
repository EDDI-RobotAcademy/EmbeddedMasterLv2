#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define Guide_Code 1

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
		(*root)->level = (*root)->level;
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

void RR_rotation(avl **root)
{
	avl *top, *mid, *bot;

	top = *root;
	mid = (*root)->right;
	bot = (*root)->right->right;

	if(bot->left)
		top->right = bot->left;
	else
		top->right = NULL;

	mid->left = top;
	*root = mid;
}

void RL_rotation(avl **root)
{
	avl *top, *mid, *bot;

	top = *root;
	mid = (*root)->right;
	bot = (*root)->right->left;

	if(bot->right)
		mid->left = bot->right;
	else
		mid->left = NULL;

	mid->level--;
	bot->right = mid;
	(*root)->right = bot;
}

void LL_rotation(avl **root)
{
	printf("root = %d\t", (*root)->data);
	printf("root left = %d\t", (*root)->left->data);
	printf("root right = %d\n", (*root)->right->data);

	avl *top, *mid, *bot;
	avl **grand_parent = ((*root)->parent) ? &(*root)->parent : root;

	top = (grand_parent == root) ? *grand_parent : (*grand_parent)->left;
	mid = top->left;
	bot = mid->left;

	if(mid->right)
		mid->right->parent = top;

	top->left = mid->right;

	mid->parent = top->parent;
	top->parent = mid;
	mid->right = top;

	if(*grand_parent == *root)
		*root = mid;
	else
		(*grand_parent)->left = mid;
}

void LR_rotation(avl **root)
{
	avl *top, *mid, *bot;
	avl **grand_parent = ((*root)->parent) ? &(*root)->parent : root;

	top = (grand_parent == root) ? *grand_parent : (*grand_parent)->left;
	mid = top->left;
	bot = mid->right;

	printf("top = %d\t", top->data);
	printf("mid = %d\t", mid->data);
	printf("bot = %d\n", bot->data);

	if(bot->left)
		bot->left->parent = mid;

	mid->right = bot->left;

	mid->level--;
	bot->parent = mid->parent;
	mid->parent = bot;

	bot->left = mid;
	mid = bot;

	LL_rotation(root);
}

void nr_run_rotation(int factor, avl **root, int data)
{
	avl *grand_parent = ((*root)->parent) ? (*root)->parent : *root;
	int dir_rot = decide_rotation(factor, root, data);

	switch(dir_rot)
	{
		case RR:
			printf("RR rotation\n");
			break;

		case RL:
			printf("RL rotation\n");
			break;

		case LL:
			printf("LL rotation\n");
			LL_rotation(root);
			//grand_parent->right->level = grand_parent->right->level - factor;
			//update_level(&grand_parent->left);
			//update_level(&grand_parent);
			break;

		case LR:
			printf("LR rotation\n");
			LR_rotation(root);
			printf("grand_parent->left->right = %d\n", grand_parent->left->right->data);
			grand_parent->left->right->level = grand_parent->left->right->level - factor;
			update_level(&grand_parent->left);
			update_level(&grand_parent);
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
		factor = calc_balance_factor(loop);
		printf("loop %d's factor = %d\n", (*loop)->data, factor);
		if(ABS(factor) > 1)
		{
			printf("unbalance node = %d\n", (*loop)->data);
			nr_run_rotation(factor, loop, data);
			if((*root)->parent)
				*root = *loop;
			break;
		}
		loop = &(*loop)->parent;
	}
}

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
}

int main(void)
{
    avl *root = NULL;
    int i;

#if Guide_Code
    int data[1024];
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
	//int data[3] = {30, 20, 10};
	int len = sizeof(data)/sizeof(int);
	for(i = 0; i < len; i++)
	{
		nr_insert_avl(&root, data[i]);
	}
#endif

	print_avl(root);

    return 0;
}
