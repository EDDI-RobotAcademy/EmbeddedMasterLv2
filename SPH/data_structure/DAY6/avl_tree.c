#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define Guide_Code 1
#define Debug 2
//#define Debug_2 3

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
	avl *high, *mid, *low;

	high = *root;
	mid = (*root)->right;
	low = (*root)->right->right;

	if(low->left)
		high->right = low->left;
	else
		high->right = NULL;

	mid->left = high;
	*root = mid;
}

void RL_rotation(avl **root)
{
	avl *high, *mid, *low;

	high = *root;
	mid = (*root)->right;
	low = (*root)->right->left;

	if(low->right)
		mid->left = low->right;
	else
		mid->left = NULL;

	mid->level--;
	low->right = mid;
	(*root)->right = low;
}

void LL_rotation(avl **root)
{
	avl *high, *mid, *low;

	high = *root;
	mid = (*root)->left;
	low = (*root)->left->left;

	if(mid->right)
		high->left = mid->right;
	else
		high->left = NULL;

	mid->right = high;

	*root = mid;
}

void LR_rotation(avl **root)
{
	avl *high, *mid, *low;

	high = *root;
	mid = (*root)->left;
	low = (*root)->left->right;

	if(low->left)
		mid->right = low->left;
	else
		mid->right = NULL;

	mid->level--;
	low->left = mid;
	(*root)->left = low;
}

void run_rotation(int factor, avl **root, int data)
{
	//avl *high;
	//avl *mid;
	//avl *low;

	int rotation = decide_rotation(factor, root, data);

	switch(rotation)
	{
		case RR:
			RR_rotation(root);
			(*root)->left->level = (*root)->left->level + factor;
			update_level(&(*root)->right);
			update_level(root);
			break;
		case RL:
			RL_rotation(root);
			run_rotation(factor, root, (*root)->right->right->data);
			break;
		case LL:
			LL_rotation(root);
			(*root)->right->level = (*root)->right->level - factor;
			update_level(&(*root)->left);
			update_level(root);
			break;
		case LR:
			LR_rotation(root);
			run_rotation(factor, root, (*root)->left->left->data);
			break;
	}
/*
	//트리가 오른쪽 불균형일 때
	if(factor < 0)
	{
		//RR
		if((*root)->right->data < data)
		{
			high = *root;
			mid = (*root)->right;
			low = (*root)->right->right;
#if Debug_2
			printf("before RR rotation\n");
			printf("root data = %d\t", (*root)->data);
			printf("root left data = %d\t", (*root)->left->data);
			printf("root right data = %d\n", (*root)->right->data);

			printf("high data = %d\t", high->data);
			printf("high left data = %d\t", high->left->data);
			printf("high right data = %d\t", high->right->data);

			printf("mid data = %d\t", mid->data);
			printf("mid left data = %d\t", mid->left->data);
			printf("mid right data = %d\t", mid->right->data);

			printf("low data = %d\t", low->data);
			printf("low left data = %d\t", low->left->data);
			printf("low right data = %d\t", low->right->data);

			printf("root level = %d\t", (*root)->level);
			printf("left level = %d\t", (*root)->left->level);
			printf("right level = %d\n", (*root)->right->level);
#endif
			if(low->left)
				high->right = low->left;
			else
				high->right = NULL;

			high->level = high->level + factor;

			mid->left = high;

			*root = mid;

			(*root)->right = low;
#if Debug_2
			printf("RR rotation\n");
			printf("root data = %d\t", (*root)->data);
			printf("root left data = %d\t", (*root)->left->data);
			printf("root right data = %d\t", (*root)->right->data);

			printf("high data = %d\t", high->data);
			printf("high left data = %d\t", high->left->data);
			printf("high right data = %d\t", high->right->data);

			printf("mid data = %d\t", mid->data);
			printf("mid left data = %d\t", mid->left->data);
			printf("mid right data = %d\t", mid->right->data);

			printf("low data = %d\t", low->data);
			printf("low left data = %d\t", low->left->data);
			printf("low right data = %d\t", low->right->data);

			printf("root level = %d\t", (*root)->level);
			printf("left level = %d\t", (*root)->left->level);
			printf("right level = %d\n", (*root)->right->level);
#endif
			update_level(&(*root)->right);
			update_level(root);
		}
		//RL
		else
		{
			high = *root;
			mid = (*root)->right;
			low = (*root)->right->left;
#if Debug_2
			printf("before RL rotation\n");
			printf("root data = %d\t", (*root)->data);
			printf("root left data = %d\t", (*root)->left->data);
			printf("root right data = %d\n", (*root)->right->data);

			printf("high data = %d\t", high->data);
			printf("high left data = %d\t", high->left->data);
			printf("high right data = %d\t", high->right->data);

			printf("mid data = %d\t", mid->data);
			printf("mid left data = %d\t", mid->left->data);
			printf("mid right data = %d\t", mid->right->data);

			printf("low data = %d\t", low->data);
			printf("low left data = %d\t", low->left->data);
			printf("low right data = %d\t", low->right->data);

			printf("root level = %d\t", (*root)->level);
			printf("left level = %d\t", (*root)->left->level);
			printf("right level = %d\n", (*root)->right->level);
#endif
			if(low->right)
				mid->left = low->right;
			else
				mid->left = NULL;

			mid->level--;

			low->right = mid;

			(*root)->right = low;
#if Debug_2
			printf("RL rotation\n");
			printf("root data = %d\t", (*root)->data);
			printf("root left data = %d\t", (*root)->left->data);
			printf("root right data = %d\t", (*root)->right->data);

			printf("high data = %d\t", high->data);
			printf("high left data = %d\t", high->left->data);
			printf("high right data = %d\t", high->right->data);

			printf("mid data = %d\t", mid->data);
			printf("mid left data = %d\t", mid->left->data);
			printf("mid right data = %d\t", mid->right->data);

			printf("low data = %d\t", low->data);
			printf("low left data = %d\t", low->left->data);
			printf("low right data = %d\t", low->right->data);

			printf("root level = %d\t", (*root)->level);
			printf("left level = %d\t", (*root)->left->level);
			printf("right level = %d\n", (*root)->right->level);
#endif
			rotation(factor, root, (*root)->right->right->data);
		}
	}
	//트리가 왼쪽 불균형 일 때
	else
	{
		//LL
		if((*root)->left->data > data)
		{
			high = *root;
			mid = (*root)->left;
			low = (*root)->left->left;
#if Debug_2
			printf("before LL rotation\n");
			printf("root data = %d\t", (*root)->data);
			printf("root left data = %d\t", (*root)->left->data);
			printf("root right data = %d\n", (*root)->right->data);

			printf("high data = %d\t", high->data);
			printf("high left data = %d\t", high->left->data);
			printf("high right data = %d\t", high->right->data);

			printf("mid data = %d\t", mid->data);
			printf("mid left data = %d\t", mid->left->data);
			printf("mid right data = %d\t", mid->right->data);

			printf("low data = %d\t", low->data);
			printf("low left data = %d\t", low->left->data);
			printf("low right data = %d\t", low->right->data);

			printf("root level = %d\t", (*root)->level);
			printf("left level = %d\t", (*root)->left->level);
			printf("right level = %d\n", (*root)->right->level);
#endif

			if(mid->right)
				high->left = mid->right;
			else
				high->left = NULL;

			high->level = high->level - factor;

			mid->right = high;

			*root = mid;
#if Debug_2
			printf("LL rotation\n");
			printf("root data = %d\t", (*root)->data);
			printf("root left data = %d\t", (*root)->left->data);
			printf("root right data = %d\t", (*root)->right->data);

			printf("high data = %d\t", high->data);
			printf("high left data = %d\t", high->left->data);
			printf("high right data = %d\t", high->right->data);

			printf("mid data = %d\t", mid->data);
			printf("mid left data = %d\t", mid->left->data);
			printf("mid right data = %d\t", mid->right->data);

			printf("low data = %d\t", low->data);
			printf("low left data = %d\t", low->left->data);
			printf("low right data = %d\t", low->right->data);

			printf("root level = %d\t", (*root)->level);
			printf("left level = %d\t", (*root)->left->level);
			printf("right level = %d\n", (*root)->right->level);
#endif
			update_level(&(*root)->left);
			update_level(root);
		}
		//LR
		else
		{
			high = *root;
			mid = (*root)->left;
			low = (*root)->left->right;
#if Debug_2
			printf("before LR rotation\n");
			printf("root data = %d\t", (*root)->data);
			printf("root left data = %d\t", (*root)->left->data);
			printf("root right data = %d\n", (*root)->right->data);

			printf("high data = %d\t", high->data);
			printf("high left data = %d\t", high->left->data);
			printf("high right data = %d\t", high->right->data);

			printf("mid data = %d\t", mid->data);
			printf("mid left data = %d\t", mid->left->data);
			printf("mid right data = %d\t", mid->right->data);

			printf("low data = %d\t", low->data);
			printf("low left data = %d\t", low->left->data);
			printf("low right data = %d\t", low->right->data);

			printf("root level = %d\t", (*root)->level);
			printf("left level = %d\t", (*root)->left->level);
			printf("right level = %d\n", (*root)->right->level);
#endif
			if(low->left)
				mid->right = low->left;
			else
				mid->right = NULL;

			mid->level--;

			low->left = mid;

			(*root)->left = low;
#if Debug_2
			printf("LR rotation\n");
			printf("root data = %d\t", (*root)->data);
			printf("root left data = %d\t", (*root)->left->data);
			printf("root right data = %d\t", (*root)->right->data);

			printf("high data = %d\t", high->data);
			printf("high left data = %d\t", high->left->data);
			printf("high right data = %d\t", high->right->data);

			printf("mid data = %d\t", mid->data);
			printf("mid left data = %d\t", mid->left->data);
			printf("mid right data = %d\t", mid->right->data);

			printf("low data = %d\t", low->data);
			printf("low left data = %d\t", low->left->data);
			printf("low right data = %d\t", low->right->data);

			printf("root level = %d\t", (*root)->level);
			printf("left level = %d\t", (*root)->left->level);
			printf("right level = %d\n", (*root)->right->level);
#endif
			rotation(factor, root, (*root)->left->left->data);
		}
	}*/
}

void adjust_balance(avl **root, int data)
{
	int factor = calc_balance_factor(root);
	//printf("Node %d's balance factor = %d\n", (*root)->data, factor);

	if(ABS(factor) > 1)
		run_rotation(factor, root, data);
	else
		return;
}

void insert_avl(avl **root, int data)
{
    if (!(*root))
    {
        *root = create_avl_node();
        (*root)->data = data;
        (*root)->level = 1;
    }

    if ((*root)->data > data)
    {
        insert_avl(&(*root)->left, data);
    }
    else if ((*root)->data < data)
    {
        insert_avl(&(*root)->right, data);
    }

    update_level(root);
    adjust_balance(root, data);
}

int main(void)
{
    avl *root = NULL;
    int i;

#if Guide_Code
    int data[1024] = { 0 };
    int len = sizeof(data) / sizeof(int);

	srand(time(NULL));

	init_data(data, len);
	print_arr(data, len);

    for (i = 0; i < len; i++)
    {
        insert_avl(&root, data[i]);
    }

#else
#if Debug
	int data[] = {500, 50, 1000, 100, 25, 750, 1250, 75, 125, 37, 12, 625, 875, 1125, 1375, 6, 30, 40, 45};
	int len = sizeof(data)/sizeof(int);
	for(i = 0; i < len; i++)
	{
		insert_avl(&root, data[i]);
	}
#else
	int data[9] = {34, 17, 13, 12, 10, 55, 53, 54, 57};
    int len = sizeof(data) / sizeof(int);

	for (i = 0; i < len; i++)
    {
        insert_avl(&root, data[i]);
    }
#endif
#endif

	print_avl(root);

    return 0;
}
